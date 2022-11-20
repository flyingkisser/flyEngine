//
//  window.h
//  flyEngine
//
//  Created by joe on 23/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef window_h
#define window_h

#include <stdio.h>
#include <functional>
#include "defines.h"

extern GLFWwindow* g_window;
extern int g_winWidth;  //device size
extern int g_winHigh;   //device size
extern int g_shadowWidth;  //device size
extern int g_shadowHigh;   //device size
extern int g_screenWidth;   //resilution size
extern int g_screenHigh;    //resilution size

NS_FLYENGINE_BEGIN

class window{
public:
    static void printGpuInfo();
    static void init();
};

NS_FLYENGINE_END

#endif /* window_h */
