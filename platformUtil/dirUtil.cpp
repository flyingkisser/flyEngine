//
//  dirUtil.cpp
//  lines
//
//  Created by joe on 22/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "dirUtil.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void dirUtil::getCurrentWorkDir(char *szBuf,int bufSize){
    getcwd(szBuf,bufSize);
}

void dirUtil::getHomeDir(char* szBuf,int bufSize){
    char *home_path = getenv("HOME");
    int strLen=(int)strlen(home_path);
    if(strLen>bufSize)
        strLen=bufSize;
    strncpy(szBuf,home_path,strLen);
}

void dirUtil::setCurrentWorkDir(const char* szBuf){
    chdir(szBuf);
}
