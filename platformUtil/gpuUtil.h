//
//  gpuUtil.h
//  flyEngine
//
//  Created by joe on 31/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef gpuUtil_h
#define gpuUtil_h

#include <stdio.h>
class gpuUtil{
public:
    static const char* getGPUBrand();
    static const char* getGPUVender();
    static const char* getGLVersion();
    static const char* getGLSLVersion();
    static const char* getGPUSupportExtensions();
    static bool isSupportETC1();
    static bool isSupportPVR();
    static int getMaxBinding();
};


#endif /* gpuUtil_h */
