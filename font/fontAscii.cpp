//
//  fontAscii.cpp
//  flyEngine
//
//  Created by joe on 12/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "fontAscii.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "logUtil.h"

using namespace flyEngine;
using namespace std;

//static fontAscii* s_fontAscii=nullptr;
//fontAscii* getInstance(){
//    if(s_fontAscii==nullptr)
//        s_fontAscii=new fontAscii();
//    return s_fontAscii;
//};

bool fontAscii::init(const char* ttfFileName){
    FT_Library ftLib;
    FT_Face ftFace;
    
    if (FT_Init_FreeType(&ftLib)){
        flylog("Could not init FreeType Library");
        return false;
    }
        
    if (FT_New_Face(ftLib, ttfFileName, 0, &ftFace)){
        flylog("Failed to load font %s",ttfFileName);
        FT_Done_FreeType(ftLib);
        return false;
    }
        
    FT_Set_Pixel_Sizes(ftFace, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    for(int i=0;i<128;i++){
        GLuint texID=0;
        AscStruct st={0};
        if(FT_Load_Char(ftFace, i, FT_LOAD_RENDER)){
            flylog("load char %c ascii %d failed!",i,i);
            m_vecAscStruct.push_back(st);
            continue;
        }
        glGenTextures(1,&texID);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RED,
                     ftFace->glyph->bitmap.width,
                     ftFace->glyph->bitmap.rows,
                     0,GL_RED,GL_UNSIGNED_BYTE,ftFace->glyph->bitmap.buffer
                     );
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        st.texID=texID;
        st.width=ftFace->glyph->bitmap.width;
        st.height=ftFace->glyph->bitmap.rows;
        st.bearingX=ftFace->glyph->bitmap_left;
        st.bearingY=ftFace->glyph->bitmap_top;
        st.advance=ftFace->glyph->advance.x;
        m_vecAscStruct.push_back(st);
    }
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLib);
    return true;
}




