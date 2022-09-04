//
//  ssboMgr.h
//  flyEngine
//
//  Created by joe on 31/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef ssboMgr_h
#define ssboMgr_h

#include <stdio.h>
#include <vector>
#include <variant>
#include "defines.h"

USE_NS_FLYENGINE

#ifdef BUILD_MAC
class ssboMgr {
public:
    static void linkSSBOAndBindPoint(int programID,const char* ssboName,int bindPoint);
    static int createSSBO(int bindPointIndex,int ssboSize,const char* ssboName=NULL);
    static void writeData(unsigned int ssbo,int size,void *buf);
};
#endif

#endif /* ssboMgr_h */
