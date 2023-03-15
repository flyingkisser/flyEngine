//
//  texture2.h
//  flyEngine
//
//  Created by joe on 04/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef texture2_h
#define texture2_h

#include <stdio.h>
#include <string>
#include "glRef.h"
#include "defines.h"

using namespace std;

NS_FLYENGINE_BEGIN

class texture2:glRef{
protected:
    int _width;
    int _height;
    int _type;
    int _format;
    int _internalFormat;
    int _channels;
    int _colorType;
    int _texturePos;
    unsigned int _textureID;
    unsigned char* _dataBuf;
    std::string _strPath;
    bool _bFlipY=false;
    
public:
    texture2(const char* szPath,bool _bFlipY=false);
    ~texture2();
    bool init();
    flyEngine::size getSize();
    
    int getChannels(){return _channels;};
    int getWidth(){return _width;};
    int getHeight(){return _height;};
    unsigned char* getBuf(){return _dataBuf;};
    unsigned int getTextureID(){return _textureID;};
    unsigned int getTexturePos(){return _texturePos;};
    void glInit(int texturePos=0);
    void glInitWithParam(int texturePos=0,unsigned int wrapS=GL_REPEAT,unsigned int wrapT=GL_REPEAT,
                unsigned int minfilter=GL_LINEAR,unsigned int magFilter=GL_LINEAR,
                unsigned int dateType=GL_UNSIGNED_INT,bool bGenMipmap=false);
};

NS_FLYENGINE_END

#endif /* texture2_h */
