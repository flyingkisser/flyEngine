//
//  logUtil.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/6.
//  Copyright © 2021 joe. All rights reserved.
//

#include "logUtil.h"
#include<stdarg.h>

void logUtil::log(const char* szFmt,...){
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
