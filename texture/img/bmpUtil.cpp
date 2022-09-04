//
//  bmpUtil.cpp
//  flyEngine
//
//  Created by joe on 03/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "bmpUtil.h"
#include "defines.h"
using namespace flyEngine;

void bmpUtil::drawBMP(float x,float y,unsigned char* szBuf){
#ifdef BUILD_MAC
    int a=szBuf[0x12];
    int b=szBuf[0x13];
    int c=szBuf[0x14];
    int d=szBuf[0x15];
    int imgWidth=d<<24 ^ c<<16 ^ b <<8 ^ a;
    
    a=szBuf[0x16];
    b=szBuf[0x17];
    c=szBuf[0x18];
    d=szBuf[0x19];
    int imgHigh=d<<24 ^ c<<16 ^ b <<8 ^ a;
    
    unsigned char* bufBMP=(unsigned char*)(szBuf+54);
    
    glColor3f(1,0,0);
    glRasterPos2f(x,y);
    glDrawBuffer(GL_FRONT);
    //width,height,dataFormat,dataType,pixMap
    glDrawPixels(imgWidth,imgHigh,GL_BGR,GL_UNSIGNED_BYTE,bufBMP);
    glFlush();
#endif
}
