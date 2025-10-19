#pragma once 

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>


//封装的zk客户端类
class ZkClient
{
public:
    ZkClient();
    ~ZkClient();

    //启动连接zkserver
    void Start();

    //在skserver上根据指定path创建znide节点
    void Create(const char *path,const char * data,int datalen,int  state=0);

    //根据节点路径获取节点的值
    std::string GetData(const char *path);

private:
    // zk的客户端句柄
    zhandle_t *m_zhandle;
};