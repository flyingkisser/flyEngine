//
//  textureBuffer.cpp
//  flyEngine
//
//  Created by joe on 22/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#include "textureBuffer.h"
#include "logUtil.h"

USE_NS_FLYENGINE

textureBuffer::textureBuffer(float* dataBuf,unsigned int width,unsigned int height,unsigned int internalFormat,unsigned int format,unsigned int dataType,unsigned int wrapS,unsigned int wrapT,unsigned int minFilter,unsigned int magFilter){
    _dataBuf=dataBuf;
    _width=width;
    _height=height;
    _format=format;
    _internalFormat=internalFormat;
    _dataType=dataType;
    _wrapS=wrapS;
    _wrapT=wrapT;
    _minFilter=minFilter;
    _magFilter=magFilter;
    
}

bool textureBuffer::init(){
    return true;
}

//texturePos from GL_TEXTURE0,GL_TEXTURE1
void textureBuffer::glInit(int texturePos){
    glGenTextures(1,&_textureID);
    if(texturePos){
        glActiveTexture(texturePos);
        _texturePos=texturePos-GL_TEXTURE0;
    }
    glBindTexture(GL_TEXTURE_2D,_textureID);
   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);

    flylog("textureBuffer from buf %llu begin %d*%d %d",_dataBuf,_width,_height,_width*_height);
    glTexImage2D(GL_TEXTURE_2D,0,_internalFormat,_width,_height,0,_format,_dataType,_dataBuf);
    checkGLError();
    flylog("textureBuffer from buf %llu end",_dataBuf);
}
