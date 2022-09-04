//
//  texture.h
//  flyEngine
//
//  Created by joe on 04/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

//#ifndef texture_h
//#define texture_h

#include <stdio.h>
#include <string>
#include "glRef.h"
#include "defines.h"

using namespace std;

NS_FLYENGINE_BEGIN

class texture:glRef{
private:
    int _width;
    int _height;
    int _type;
    int _format;
    int _internalFormat;
    int _colorType;
    int _texturePos;
    unsigned int _textureID;
    unsigned char* _dataBuf;
    std::string _strPath;
    bool _bFlipY=false;
    
public:
    texture(const char* szPath,bool _bFlipY=false);
    ~texture();
    bool init();
    flyEngine::size getSize();
    
    int getWidth(){return _width;};
    int getHeight(){return _height;};
    unsigned char* getBuf(){return _dataBuf;};
    unsigned int getTextureID(){return _textureID;};
    unsigned int getTexturePos(){return _texturePos;};
    void glInit(int texturePos=0);
};

NS_FLYENGINE_END

//#endif /* texture_h */
