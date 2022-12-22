//
//  textureHdr.cpp
//  flyEngine
//
//  Created by joe on 21/12/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "textureHdr.h"
#include "logUtil.h"
#include "stb_image.h"

#ifdef BUILD_IOS
#include "ios_dirUtil.h"
#endif

USE_NS_FLYENGINE

textureHdr::textureHdr(const char* szPath,bool _bFlipY):texture2(szPath,_bFlipY){
    
}

bool textureHdr::init(){
    char* szPath=(char*)_strPath.c_str();
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    float *data = stbi_loadf(szPath, &width, &height, &nrComponents, 0);
    _width=width;
    _height=height;
    _format = GL_RGB16F;
    _dataBuf=data;
    return true;
}


//texturePos from GL_TEXTURE0,GL_TEXTURE1
void textureHdr::glInit(int texturePos){
    glRef::glInit();
    glGenTextures(1,&_textureID);
    if(texturePos){
        glActiveTexture(texturePos);
        _texturePos=texturePos-GL_TEXTURE0;
    }
    glBindTexture(GL_TEXTURE_2D,_textureID);
   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    flylog("glTexImage2D from buf %llu begin %d*%d %d",_dataBuf,_width,_height,_width*_height);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,_width,_height,0,GL_RGB,GL_FLOAT,_dataBuf);
    checkGLError();
    flylog("glTexImage2D from buf %llu end",_dataBuf);
}
