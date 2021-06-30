//
//  testMain.h
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef testWindow_h
#define testWindow_h

#include <stdio.h>
#include <functional>
#include "defines.h"

extern GLFWwindow* g_window;

void initWindow();

//void testInitWindow2D(const char* szTitle,void (*drawCall)(void));

//void testInitWindow2D(const char* szTitle,void (*drawCall)(void),int displayMode);

void testInitWindow2D(const char* szTitle,std::function<void(void)> drawCall,unsigned int shaderID);
void testInitWindow2D(const char* szTitle,std::function<void(void)> drawCall);


//void testInitWindow3D(const char* szTitle,void (*drawCall)(void));

void testRegMenu(const char* szMenuName,std::function<void(void)>cb);
void testRegKeyboard(std::function<void(unsigned char,int,int)>cb);
void testRegMouseClick(std::function<void(int,int,int,int)>cb);
void testRegMouseClickWithMove(std::function<void(int,int)>cb);
void testRegMouseMove(std::function<void(int,int)>cb);

void windowLoop();
size getWindowSize();

#endif /* testMain_h */
