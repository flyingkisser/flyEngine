//
//  textureBuffer.h
//  flyEngine
//
//  Created by joe on 22/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef textureBuffer_h
#define textureBuffer_h

#include <stdio.h>
#include "defines.h"
#include "texture2.h"

using namespace std;

NS_FLYENGINE_BEGIN

class textureBuffer: public texture2{
    
private:
    float* _dataBuf;
    unsigned int _internalFormat;
    unsigned int _dataType;
    unsigned int _wrapS;
    unsigned int _wrapT;
    unsigned int _minFilter;
    unsigned int _magFilter;
    
public:
    textureBuffer(float* dataBuf,unsigned int width=64,unsigned int height=64,unsigned int internalFormat=GL_RGBA,unsigned int dataType=GL_FLOAT,unsigned int format=GL_RGBA,unsigned int wrapS=GL_CLAMP_TO_EDGE,unsigned int wrapT=GL_CLAMP_TO_EDGE,unsigned int minFilter=GL_NEAREST,unsigned int magFilter=GL_LINEAR);
    bool init();
    float* getBuf(){return _dataBuf;};
    void glInit(int texturePos=0);
};

NS_FLYENGINE_END
#endif /* textureBuffer_h */
