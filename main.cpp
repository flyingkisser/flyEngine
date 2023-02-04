//
//  main.cpp
//
//  Created by Joe on 2017/4/25.
//  Copyright © 2017年 joe. All rights reserved.
//

#include <iostream>
#include "defines.h"
#include "dirUtil.h"
#include "world.h"
#include "uboMgr.h"
#include "timeUtil.h"
#include "glslUtil.h"

#include "window.h"
#include "gpuUtil.h"

#include "testMain.h"
USE_NS_FLYENGINE

int main(int argc, char **argv) {
    char szHomeDir[1024]={0};
    char szWorkDir[1024]={0};
    dirUtil::getHomeDir(szHomeDir,sizeof(szHomeDir));
    dirUtil::getCurrentWorkDir(szWorkDir,sizeof(szWorkDir));
    strcat(szHomeDir,"/Documents/flyEngine/");
    dirUtil::setCurrentWorkDir(szHomeDir);
    printf("main:set current work dir %s\n",szHomeDir);
    printf("main:engine dir %s\n",szWorkDir);

    timeUtil::init();
    window::init();
    glslUtil::init();
    uboMgr::initAllUbo();
    
    gpuUtil::printGpuInfo();
    
    testMainBegin();
    
    world::getInstance()->start_rendering();
    
    return 0;
}

