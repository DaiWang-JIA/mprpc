#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

/*
UserService原来是一个本地服务，提供了两个进程内的本地方法：
Login和GetFriendLists
*/


class UserServices: public  fixbug::UserServiceRpc  //使用在rpc服务发布端（rpc服务提供者）
{
public:
    bool Login(std::string name,std::string pwd)
    {
        std::cout<<"doing local service: Login"<<std::endl;
        std::cout<<"name:"<<name<<"pwd:"<<pwd<<std::endl;

        return true;
    }

    bool Register(uint32_t id,std::string name, std::string pwd)
    {
        std::cout<<"doing Register service: Register"<<std::endl;
        std::cout<<"id:"<<id<<"name:"<<name<<"pwd:"<<pwd<<std::endl;
        return true;
    }

    //重写基类UserServiceRpc的虚函数  下面这些方法都是框架直接调用的
    /*
    1.caller ===> Login(LoginRequest)  =>muduo =>callee
    1.callee ===> Login(LoginRequest)  => 交到下面重写的Login方法上
    */
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        //框架给业务上报了请求参数LoginRequest，应用程序获取相应数据做本地业务
        std::string name=request->name();
        std::string pwd=request->pwd();

        //做本地业务
        bool login_result=Login(name,pwd);  

        //把响应写入  包括错误码，错误消息，返回值
        fixbug::ResultCode* code =response->mutable_result();
        code->set_errcode(1);
        code->set_errmsg("Login do error");
        response->set_success(login_result);

        //执行回调函数
        done->Run();

    }

    void Register(::google::protobuf::RpcController *controller,
                  const ::fixbug::RegisterRequest *request,
                  ::fixbug::RegisterResponse *response,
                  ::google::protobuf::Closure *done)
    {
        //取数据
        uint32_t id=request->id();
        std::string name=request->name();
        std::string pwd=request->pwd();

        bool ret=Register(id,name,pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_success(ret);

        done->Run();

    }
};

int main(int argc,char **argv)
{
    // UserServices us;
    // us.Login("xxx","xxx");


    // 调用框架初始化操作
    MprpcApplication::Init(argc,argv);

    //provider是一个网络服务对象，把UserService对象发布到rpc节点上
    Rpcprovider provider;
    provider.NotifyService(new UserServices());    

    //启动一个rpc服务发布节点  Run以后进入阻塞状态，等待rpc请求
    provider.Run();
    
    return 0;
}