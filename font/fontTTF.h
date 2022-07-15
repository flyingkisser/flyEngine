//
//  fontTTF.h
//  flyEngine
//
//  Created by joe on 12/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//download freetype from https://download.savannah.gnu.org/releases/freetype/
//./configure
//make
//cp objs/.libs/libfreetype.a xx/
//cp -R include xx/freetype
//add xx/freetype in project include path
//macos 记得加载libbz.2.x.x.tbd

#ifndef fontTTF_h
#define fontTTF_h

#include <stdio.h>
#include <vector>
#include "defines.h"
#include "shader.h"
#include "ft2build.h"
#include FT_FREETYPE_H


NS_FLYENGINE_BEGIN

struct texFontStruct{
    int ascii;
    int texID;
    int width;
    int height;
    int bearingX;
    int bearingY;
    float advance;
};

class fontTTF{
public:
    fontTTF(const char* ttfFileName,int fontSize=48){
        _fontPath=(char*)ttfFileName;
        _fontSize=fontSize;
    };
    ~fontTTF(){
        FT_Done_Face(_ftFace);
        FT_Done_FreeType(_ftLib);
    };
    
    bool init();
    void glInit();
//    void setFontSize(int s);
    texFontStruct getTexStruct(int c);
    
private:
    FT_Library _ftLib;
    FT_Face _ftFace;
    int _fontSize;
    char* _fontPath;
//    shader* _shader2dObj;
    std::vector<texFontStruct> _vecStruct;
};

NS_FLYENGINE_END

#endif /* fontTTF_h */
