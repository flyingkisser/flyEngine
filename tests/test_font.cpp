//
//  test_font.cpp
//  flyEngine
//
//  Created by joe on 11/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include "test_font.h"

//download freetype from https://download.savannah.gnu.org/releases/freetype/
//./configure
//make
//cp objs/.libs/libfreetype.a xx/
//cp -R include xx/freetype
//add xx/freetype in project include path
//macos 记得加载libbz.2.x.x.tbd
#include "ft2build.h"
#include FT_FREETYPE_H

using namespace std;

void test_font_ttf(){
    FT_Library ft;
    FT_Face face;
    
    if (FT_Init_FreeType(&ft))
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
    if (FT_New_Face(ft, "res/font/Tahoma.ttf", 0, &face))
        cout << "ERROR::FREETYPE: Failed to load font" << endl;
    FT_Set_Pixel_Sizes(face, 0, 48);
}
