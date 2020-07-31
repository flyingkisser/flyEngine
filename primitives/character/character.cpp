//
//  character.cpp
//  lines
//
//  Created by joe on 24/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "character.h"
#include <GLUT/GLUT.h>
#include <string.h>

void drawStringByBitmap(const char* szBuf){
    glColor3f(1,0,0);
    glRasterPos2i(30,120);
    int len=(int)strlen(szBuf);
    for(int i=0;i<len;i++){
        //GLUT_BITMAP_9_BY_15
        //GLUT_BITMAP_TIMES_ROMAN_10
        //GLUT_BITMAP_HELVETICA_10
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, szBuf[i]);
    }
    glFlush();
}

void drawStringByStroke(const char* szBuf){
    glColor3ub(255,50,30);
    glRasterPos2i(100,100);
    int len=(int)strlen(szBuf);
    for(int i=0;i<len;i++){
        //GLUT_STROKE_ROMAN
        //GLUT_STROKE_MONO_ROMAN
        glutStrokeCharacter(GLUT_STROKE_ROMAN, szBuf[i]);
    }
    glFlush();
}
