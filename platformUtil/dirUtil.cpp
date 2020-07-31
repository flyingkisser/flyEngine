//
//  dirUtil.cpp
//  lines
//
//  Created by joe on 22/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "dirUtil.h"
#include <unistd.h>

void getCurrentWorkDir(char *szBuf,int bufSize){
    getcwd(szBuf,bufSize);
}

void setCurrentWorkDir(const char* szBuf){
    chdir(szBuf);
}
