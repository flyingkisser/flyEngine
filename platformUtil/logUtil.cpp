//
//  logUtil.cpp
//  flyEngine
//
//  Created by joe on 29/06/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "logUtil.h"
#include<stdarg.h>

void logUtil::log(const char* szFmt,...){
    va_list args;
    va_start(args,szFmt);
    char szBuf[1024]={0};
    vsprintf(szBuf,szFmt,args);
    printf(szBuf);
}
