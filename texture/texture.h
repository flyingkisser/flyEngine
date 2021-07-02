//
//  texture.h
//  flyEngine
//
//  Created by joe on 24/06/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef texture_h
#define texture_h

#include <stdio.h>
#include <string>
#include "flyEngine.h"

using namespace std;

namespace flyEngine{

class texture{
public:
    texture(const char* szPath);
    ~texture();
    bool init();
    flyEngine::size getSize();
    
    int getWidth(){return _width;};
    int getHeight(){return _height;};
    unsigned char* getBuf(){return _dataBuf;};
    unsigned int getTextureID(){return _textureID;};
    
private:
    int _width;
    int _height;
    int _type;
    int _format;
    int _internalFormat;
    int _colorType;
    unsigned int _textureID;
    unsigned char* _dataBuf;
    std::string _strPath;
    
    void _glInit();
    
};

}
#endif /* texture_h */
