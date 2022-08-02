//
//  uboMgr.h
//  flyEngine
//
//  Created by joe on 31/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef uboMgr_h
#define uboMgr_h

#include <stdio.h>
#include <vector>
#include <variant>
#include "defines.h"

USE_NS_FLYENGINE

class uboMgr {
public:
    static void linkUBOAndBindPoint(int programID,const char* uboName,int bindPoint);
    static int createUBO(int bindPointIndex,int uboSize);
    static void writeData(unsigned int ubo,int num,int sizeArr[],void* bufArr[],int firstOffset=0);
};

#endif /* uboMgr_h */
