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
const char* getGPUBrand();
const char* getGPUVender();
const char* getGLVersion();
const char* getGPUSupportExtensions();
bool isSupportETC1();
bool isSupportPVR();

#endif /* gpuUtil_h */
