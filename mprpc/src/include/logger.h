#pragma once
#include "lockqueue.h"
#include <string>

enum LogLevel
{
    INFO, // 普通的日志信息
    ERROR, //错误信息
};


//Mprpc框架提供的日志系统
class Logger
{
public:
    //获取日志单例
    static Logger& GetInstance();

    //写日志，携带级别信息，避免数据竞争
    void Log(std::string msg);

private:
    LockQueue<std::string> m_lckQue;  //日志缓冲队列

    //设置单例
    Logger();
    Logger(const Logger&) =delete;
    Logger (Logger&&) =delete;

};


// 修复：日志级别随消息一起传递，避免多线程数据竞争
#define LOG_INFO(logmsgformat, ...) \
    do { \
        Logger &logger = Logger::GetInstance(); \
        char c[1024] = {0}; \
        snprintf(c, sizeof(c), logmsgformat, ##__VA_ARGS__); \
        logger.Log(std::string("[info] ") + c); \
    } while(0)

#define LOG_ERR(logmsgformat, ...) \
    do { \
        Logger &logger = Logger::GetInstance(); \
        char c[1024] = {0}; \
        snprintf(c, sizeof(c), logmsgformat, ##__VA_ARGS__); \
        logger.Log(std::string("[error] ") + c); \
    } while(0)

