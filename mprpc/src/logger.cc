#include "logger.h"
#include <string>
#include <time.h>
#include <iostream>

// 获取日志单例
Logger &Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    //启动写日志线程
    std::thread writelogTask([this](){
        for(;;)
        {
            // 先Pop消息（可能阻塞），再获取当前时间，确保时间戳准确
            std::string msg = m_lckQue.Pop();

            // 获取当前时间（Pop之后，时间戳准确）
            time_t now = time(nullptr);
            tm nowtm;
            localtime_r(&now, &nowtm);  // 使用线程安全版本

            // 日志文件名使用前导零，方便排序
            char file_name[128];
            sprintf(file_name, "%d-%02d-%02d-log.txt",
                    nowtm.tm_year + 1900, nowtm.tm_mon + 1, nowtm.tm_mday);

            FILE *pf = fopen(file_name, "a+");
            if (pf == nullptr)
            {
                std::cout << "logger file: " << file_name << " open error!" << std::endl;
                exit(EXIT_FAILURE);
            }

            // 时间戳使用前导零
            char time_buf[128] = {0};
            sprintf(time_buf, "%02d:%02d:%02d => ",
                    nowtm.tm_hour,
                    nowtm.tm_min,
                    nowtm.tm_sec);

            // msg 已经自带 [info] 或 [error] 前缀（由宏传入）
            std::string log_line = std::string(time_buf) + msg + "\n";

            fputs(log_line.c_str(), pf);
            fclose(pf);
        }
    });

    //设置分离线程
    writelogTask.detach();
}

// 写日志  把日志信息写入lockqueue缓冲区（msg已包含级别前缀）
void Logger::Log(std::string msg)
{
    m_lckQue.Push(msg);
}