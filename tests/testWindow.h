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

void testInitWindow2D(const char* szTitle,void (*displayCB)(void));
void testInitWindow2D(const char* szTitle,void (*displayCB)(void),int displayMode);

void testInitWindow3D(const char* szTitle,void (*displayCB)(void));

void testRegMenu(const char* szMenuName,std::function<void(void)>cb);
void testRegKeyboard(std::function<void(unsigned char,int,int)>cb);
void testRegMouseClick(std::function<void(int,int,int,int)>cb);
void testRegMouseClickWithMove(std::function<void(int,int)>cb);
void testRegMouseMove(std::function<void(int,int)>cb);

void windowLoop();

#endif /* testMain_h */
