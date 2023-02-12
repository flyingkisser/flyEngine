//
//  timeUtil.cpp
//  flyEngine
//
//  Created by joe on 19/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "timeUtil.h"
#include <string>

//数量级近似体验
//执行典型指令     　　　　　　　　　　  1/1,000,000,000 秒 =1 纳秒
//从一级缓存中读取数据 　　　 　　　　   0.5 纳秒
//分支预测错误 　　　　　　    　　　　  5 纳秒
//从二级缓存中读取数据 　　　　　　　    7 纳秒
//互斥锁定 / 解锁 　　　　　　 　　　　  25 纳秒
//从主存储器中读取数据 　　    　　　　  100 纳秒
//在 1Gbps 的网络中发送 2KB 数据 　　   20,000 纳秒
//从内存中读取 1MB 数据 　　　　　　     250,000 纳秒
//从新的磁盘位置读取数据 ( 寻轨 ) 　　   8,000,000 纳秒
//从磁盘中读取 1MB 数据 　　　　　　     20,000,000 纳秒
//在美国向欧洲发包并返回 　　　　　　     150 毫秒 =150,000,000 纳秒

//纳秒 nanosecond
long long timeUtil::getTimeNS(){
    long long nsTime = 0;
    #if defined(__APPLE__)  //iOS
        if (__builtin_available(iOS 10.0, *)) {
            struct timespec abstime;
            clock_gettime(CLOCK_REALTIME, &abstime);
            nsTime = ((u_int64_t)abstime.tv_sec) * 1000*1000*1000 + ((u_int64_t)abstime.tv_nsec);
        }
    #else
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        usTime = ((u_int64_t)abstime.tv_sec) * 1000*1000*1000 + ((u_int64_t)abstime.tv_nsec);   //需要强制转long long
    #endif
    return nsTime;
}
//微妙 microsecond
long long timeUtil::getTimeUS(){
    long long usTime = 0;
    #if defined(__APPLE__)  //iOS
        if (__builtin_available(iOS 10.0, *)) {
            struct timespec abstime;
            clock_gettime(CLOCK_REALTIME, &abstime);
            usTime = ((u_int64_t)abstime.tv_sec) * 1000*1000 + ((u_int64_t)abstime.tv_nsec) / 1000;
        }
    #else
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        usTime = ((u_int64_t)abstime.tv_sec) * 1000*1000 + ((u_int64_t)abstime.tv_nsec) / 1000;   //需要强制转long long
    #endif
    return usTime;
}
//毫秒 millisecond
unsigned long timeUtil::getTimeMS(){
     long long msTime = 0;
    #if defined(__APPLE__)  //iOS
        if (__builtin_available(iOS 10.0, *)) {
            struct timespec abstime;
            clock_gettime(CLOCK_REALTIME, &abstime);
            msTime = ((u_int64_t)abstime.tv_sec) * 1000 + ((u_int64_t)abstime.tv_nsec) / 1000000;
        } else {
            struct timeval abstime;
            gettimeofday(&abstime, NULL);
            msTime = ((u_int64_t)abstime.tv_sec) * 1000 + ((u_int64_t)abstime.tv_usec) / 1000;
        }
    #else
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        msTime = ((u_int64_t)abstime.tv_sec) * 1000 + ((u_int64_t)abstime.tv_nsec) / 1000000;   //需要强制转long long
    #endif
    return msTime;
}
//秒
unsigned int timeUtil::getTime(){
    return getTimeMS()/1000;
}

float timeUtil::getTimeFloat(){
    return (float)getTimeMS()/1000.0;
}

static long long s_time_us_first_run=0.0f;
float timeUtil::getTimeFloatSinceRun(){
    if(s_time_us_first_run==0.0f){
        s_time_us_first_run=getTimeUS();   //mirco second value
        return 0;
    }
    return ((float)(getTimeUS()-s_time_us_first_run))/1000000;
}

void timeUtil::init(){
    s_time_us_first_run=getTimeUS();
}

struct tm* timeUtil::getDate()
{
    time_t timep;
    time(&timep);
    struct tm *data = gmtime(&timep);
    data->tm_year += 1900;
    data->tm_mon += 1;
    return data;
}

struct tm* timeUtil::getDateBySec(long long secTime)
{
    time_t timep=secTime;
    struct tm *data = gmtime(&timep);
    data->tm_year += 1900;
    data->tm_mon += 1;
    return data;
}

int timeUtil::getTimeZoneDiff()
{
    time_t now = time(NULL);
    struct tm *gmTime = gmtime(&now);
    if (gmTime)
        return (int)difftime(now, mktime(gmTime));
    return 0;
}

std::string timeUtil::getTimeStrLong(){
    char szBuf[1024]={0};
    std::string s;
    time_t timeValue;
    struct timeval st_timeval={0};
    struct tm st_tm={0};
    gettimeofday(&st_timeval, NULL);
    unsigned int timeMS=st_timeval.tv_usec%1000;
    time(&timeValue);
    gmtime_r(&timeValue, &st_tm);
    snprintf(szBuf,sizeof(szBuf),"%04d-%02d-%02d %02d:%02d:%02d:%03u ",st_tm.tm_year+1900,st_tm.tm_mon+1,st_tm.tm_mday,
              st_tm.tm_hour,st_tm.tm_min,st_tm.tm_sec,timeMS);
    return std::string(szBuf);
}
