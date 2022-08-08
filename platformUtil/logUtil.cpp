//
//  logUtil.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/6.
//  Copyright © 2021 joe. All rights reserved.
//


#include <stdarg.h>
#include <string>

#include "logUtil.h"
#include "timeUtil.h"


void logUtil::log(const char* fmt,...){
    va_list argList;
    char szFmt[1024*10]={0};
    va_start(argList, fmt);  //将ap指向fmt后的第一个参数
    int strLen=(int)strlen(fmt);
    if(strLen<=1024*10-2 &&fmt[strLen-1]!='\n'){
        strcpy(szFmt,fmt);
        szFmt[strLen]='\n';
        vfprintf(stdout,szFmt,argList);
    }else{
        vfprintf(stdout,fmt,argList);
    }
    va_end(argList);//将ap置空
}

void logUtil::logMat4(glm::mat4 m){
    flylog("[%f %f %f %f]\n[%f %f %f %f]\n[%f %f %f %f]\n[%f %f %f %f]\n",
            m[0][0],m[0][1],m[0][2],m[0][3],
            m[1][0],m[1][1],m[1][2],m[1][3],
            m[2][0],m[2][1],m[2][2],m[2][3],
            m[3][0],m[3][1],m[3][2],m[3][3]
           );
}


void flylog(const char* fmt,...){
    va_list argList;
    char szFmt[1024*10]={0};
    va_start(argList, fmt);  //将ap指向fmt后的第一个参数
    int strLen=(int)strlen(fmt);
    std::string strTime=timeUtil::getTimeStrLong();
    if(strLen<=1024*10-2 &&fmt[strLen-1]!='\n'){
        strcpy(szFmt,strTime.c_str());
        strcat(szFmt,fmt);
        szFmt[strlen(szFmt)]='\n';
        vfprintf(stdout,szFmt,argList);
    }else{
        vfprintf(stdout,fmt,argList);
    }
    va_end(argList);//将ap置空
}

void flylogNoTime(const char* fmt,...){
    va_list argList;
    char szFmt[1024*10]={0};
    va_start(argList, fmt);  //将ap指向fmt后的第一个参数
    strcpy(szFmt,fmt);
    szFmt[strlen(szFmt)]='\n';
    vfprintf(stdout,szFmt,argList);
    va_end(argList);//将ap置空
}
