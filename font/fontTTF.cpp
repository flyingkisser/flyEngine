//
//  fontTTF.cpp
//  flyEngine
//
//  Created by joe on 12/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "fontTTF.h"
#include "logUtil.h"
#include "shader.h"
#include "shaderMgr.h"
#include "window.h"
#include "logUtil.h"

using namespace flyEngine;
using namespace std;

bool fontTTF::init(){
    if (FT_Init_FreeType(&_ftLib)){
        flylog("could not init FreeType Library");
        return false;
    }
    if (FT_New_Face(_ftLib, _fontPath, 0, &_ftFace)){
        flylog("failed to load font %s",_fontPath);
        FT_Done_FreeType(_ftLib);
        return false;
    }
    FT_Set_Pixel_Sizes(_ftFace, 0, _fontSize);
    glInit();
    flylog("create font %s size %d",_fontPath,_fontSize);
    return true;
}

void fontTTF::glInit(){
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    for(int i=0;i<128;i++){
        GLuint texID=0;
        texFontStruct st={0};
        if(FT_Load_Char(_ftFace, i, FT_LOAD_RENDER)){
            flylog("load char %c ascii %d failed!",i,i);
            _vecStruct.push_back(st);
            continue;
        }
        glGenTextures(1,&texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RED,
                     _ftFace->glyph->bitmap.width,
                     _ftFace->glyph->bitmap.rows,
                     0,GL_RED,GL_UNSIGNED_BYTE,_ftFace->glyph->bitmap.buffer
                     );
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        st.ascii=i;
        st.texID=texID;
        st.width=_ftFace->glyph->bitmap.width;
        st.height=_ftFace->glyph->bitmap.rows;
        st.bearingX=_ftFace->glyph->bitmap_left;
        st.bearingY=_ftFace->glyph->bitmap_top;
        st.advance=_ftFace->glyph->advance.x;
        _vecStruct.push_back(st);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

//void fontTTF::setFontSize(int s){
//    _fontSize=s;
//    FT_Set_Pixel_Sizes(_ftFace, 0, _fontSize);
//}

texFontStruct fontTTF::getTexStruct(int c){
    return _vecStruct[c];
}
