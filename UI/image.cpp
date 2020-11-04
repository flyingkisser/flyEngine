//
//  image.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "image.h"



using namespace ui;

image::image(const char *szFileName){
//    _buf=g_textureMgr->getTextureBuf(szFileName);
//    _initBMP(_buf);
                                     
}

void image::_initBMP(unsigned char* szBuf){
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
    _size.width=imgWidth;
    _size.height=imgHigh;
}

void image::draw(){
    if(!_buf)
        return;
    bmpUtil::drawBMP(_pos.x,_pos.y,_buf);
    _2dTrans::reset();
}
