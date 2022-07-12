//
//  timeUtil.cpp
//  flyEngine
//
//  Created by joe on 19/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "timeUtil.h"
#include <string>

long long timeUtil::getTimeMS(){
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

long long timeUtil::getTime(){
    return getTimeMS()/1000;
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
    int timeMS=0;
    struct tm gmt;
    time_t timep;
    time(&timep);
    gmtime_r(&timep, &gmt);
    snprintf(szBuf,sizeof(szBuf),"%04d-%02d-%02d %02d:%02d:%02d:%03d ",gmt.tm_year+1900,gmt.tm_mon+1,gmt.tm_mday,
              gmt.tm_hour,gmt.tm_min,gmt.tm_sec,timeMS);
    return std::string(szBuf);
}
