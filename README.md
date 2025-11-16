# mprpc - C++ 轻量级分布式 RPC 框架

`mprpc` 是一个基于 C++11 开发的轻量级、高性能、分布式的 RPC (远程过程调用) 通信框架。

框架核心依赖 [Muduo](https://github.com/chenshuo/muduo) 事件驱动网络库，使用 [Google Protobuf](https://github.com/protocolbuffers/protobuf) 作为消息序列化和接口定义语言（IDL），并集成 [Apache ZooKeeper](https://zookeeper.apache.org/) 作为服务注册与发现中心。

## 🚀 技术栈

* **核心语言:** C++11
* **网络通信:** [Muduo](https://github.com/chenshuo/muduo) (基于 Reactor 模型的事件驱动网络库)
* **序列化:** [Google Protobuf](https://github.com/protocolbuffers/protobuf) (用于数据序列化及服务接口定义)
* **服务治理:** [Apache ZooKeeper](https://zookeeper.apache.org/) (用于服务注册、服务发现)
* **构建工具:** CMake
* **多线程:** `std::thread`, `pthread` (用于异步日志、ZooKeeper 回调)

## ✨ 核心功能

1.  **框架初始化**:
    * 通过 `MprpcApplication` 单例类进行框架初始化。
    * 通过 `MprpcConfig` 模块加载和解析自定义的配置文件（例如 `rpcserverip`, `zookeeperip` 等）。

2.  **服务发布 (`Rpcprovider`)**:
    * `Rpcprovider` 负责 RPC 服务的发布。
    * 通过 Protobuf 的反射机制解析 `google::protobuf::Service` 对象，提取服务名和方法名。
    * 启动 Muduo `TcpServer` 进行网络监听，并设置 `OnMessage` 回调处理网络I/O。
    * 在 `Run()` 时自动连接 ZooKeeper，将服务和方法注册到 ZK。

3.  **服务调用 (`MprpcChannel`)**:
    * `MprpcChannel` 实现了 `google::protobuf::RpcChannel` 接口。
    * 重写 `CallMethod` 方法，所有通过 `Stub` (桩类) 发起的 RPC 调用都会走到这里。
    * 在 `CallMethod` 内部自动连接 ZooKeeper，根据服务名和方法名查询服务提供方的 `ip:port` 地址。
    * 使用原生 C Socket API (`socket`, `connect`, `send`, `recv`) 与 RPC 服务端进行 TCP 网络通信。

4.  **服务注册与发现 (ZooKeeper)**:
    * **服务注册**: `Rpcprovider` 启动时，会在 ZK 上创建永久的服务节点（如 `/UserServiceRpc`），并在服务节点下创建临时的 `ip:port` 方法节点（如 `/UserServiceRpc/Login`，值为 `"127.0.0.1:8000"`）。使用临时节点 (`ZOO_EPHEMERAL`) 可以确保服务宕机后节点自动删除，实现健康检查。
    * **服务发现**: `MprpcChannel` 调用时，会向 ZK 查询对应方法路径（如 `/UserServiceRpc/Login`），获取 `GetData` 返回的 `ip:port` 地址，然后发起网络连接。
    * `zookeeperutil.cc` 封装了 ZK 的客户端 C API，并使用 `sem_t` 信号量处理 `zookeeper_init` 的异步连接回调，确保连接成功后再返回。

5.  **自定义网络协议**:
    * 为了解决 TCP 粘包/拆包问题，框架定义了自定义的应用层协议。
    * **协议格式**: `[4 字节 header_size] + [RpcHeader] + [args]`
    * `header_size`: 4 字节的整数，表示 `RpcHeader` 的长度。
    * `RpcHeader`: (来自 `rpcheader.proto`) 包含 `service_name`, `method_name`, `args_size`。
    * `args`: 真正的 RPC 参数（`request`）序列化后的字符串。
    * `Rpcprovider::OnMessage` 和 `MprpcChannel::CallMethod` 共同实现了该协议的封包与解包。

6.  **异步日志模块**:
    * `Logger` 采用单例模式。
    * 使用**生产者-消费者模型**实现异步日志：业务线程（生产者）调用 `Log` 方法，仅将日志消息推入一个线程安全的队列。
    * 一个独立的后台线程（消费者）负责从队列中取出日志，并将其写入当天的日志文件（`xxxx-xx-xx-log.txt`），避免了日志 I/O 阻塞业务线程。

7.  **RPC 控制器**:
    * `MprpcController` 实现了 `google::protobuf::RpcController` 接口。
    * 主要用于管理 RPC 调用过程中的状态，如 `Failed()`, `ErrorText()` 和 `SetFailed()`，方便客户端获取调用失败的原因。

## 📋 如何使用

使用 Protobuf 定义服务（如 `user.proto`），并开启 `option cc_generic_services=true;`。

### 1. 服务提供方 (Provider) 示例

实现 `proto` 中定义的服务接口，并将其发布。

```cpp
// 摘自 userservice.cc
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

// 继承 protoc 生成的 UserServiceRpc
class UserServices : public fixbug::UserServiceRpc
{
public:
    // 重写 Login 方法
    void Login(::google::protobuf::RpcController* controller,
               const ::fixbug::LoginRequest* request,
               ::fixbug::LoginResponse* response,
               ::google::protobuf::Closure* done)
    {
        // 1. 从 request 中获取参数
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 2. 执行本地业务
        bool login_result = (name == "zhang san" && pwd == "123456");

        // 3. 填充 response
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        // 4. 执行回调
        done->Run();
    }
};

int main(int argc, char** argv)
{
    // 1. 初始化框架
    MprpcApplication::Init(argc, argv);

    // 2. 创建 Provider
    Rpcprovider provider;

    // 3. 发布服务
    provider.NotifyService(new UserServices());

    // 4. 启动服务，等待 RPC 调用
    provider.Run();

    return 0;
}


测试：       
<img width="683" height="793" alt="image" src="https://github.com/user-attachments/assets/90bc6d19-aeea-4d02-bb04-f05ea538263a" />


<img width="1518" height="657" alt="image" src="https://github.com/user-attachments/assets/a0278650-a8f4-492a-a342-df9058cfd322" />




                    
