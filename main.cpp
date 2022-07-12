//
//  main.cpp
//
//  Created by Joe on 2017/4/25.
//  Copyright © 2017年 joe. All rights reserved.
//

#include <iostream>
#include "defines.h"
#include "dirUtil.h"
#include "window.h"
#include "world.h"
#include "testMain.h"

USE_NS_FLYENGINE;


int main(int argc, char **argv) {
    char szHomeDir[1024]={0};
    dirUtil::getHomeDir(szHomeDir,sizeof(szHomeDir));
    strcat(szHomeDir,"/Documents/flyEngine/");
    dirUtil::setCurrentWorkDir(szHomeDir);
    printf("main:set current work dir %s\n",szHomeDir);
    
    window::init();
    
    testMainBegin();
    
    world::getInstance()->start_rendering();
    
    return 0;
}

