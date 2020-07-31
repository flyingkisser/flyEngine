//
//  lines.cpp
//  lines
//
//  Created by Joe on 2017/4/25.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "line.h"
#include <GLUT/glut.h>
#include "error.h"

void drawLineByPosArr(structPos2* pointArr,int num,structColor* color){
    glColor4ub(color->r,color->g,color->b,color->a);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<num;i++)
        glVertex2f(pointArr[i].x,pointArr[i].y);
    glEnd();
    glFlush();
}


void drawLineStrip(){
    glColor3f (1, 0, 0);
    //画一条折线，每个点和下一个点连接起来
    glBegin(GL_LINE_STRIP);
    glVertex2i(10,10);
    glVertex2i(100,100);
    glVertex2i(100, 200);
    glVertex2i(200,200);
    glEnd();
    glFlush();
}

void drawLine(){
    glShadeModel(GL_SMOOTH);    //默认值，使用两端点颜色的插值显示变色的线段
//    glShadeModel(GL_FLAT);    //使用最后一次指定的端点的颜色进行绘制
    //画出两条线段，第一个glVertex2i指定线段的一头，第二个glVertex2i指定另一头
    glBegin(GL_LINES);
    glColor3f (1, 0, 0);
    glVertex2i(0, 0);
    glVertex2i(100,100);
    glColor3f (1, 0, 0);
    glVertex2i(0, 100);
    glColor3f (0, 1, 0);
    glVertex2i(100,0);
    glEnd();
    glFlush();
}

void drawLineLoop(){
      glColor3f (1, 0, 0);
      //画一条封闭的折线，每个点和下一个点连接起来
      glBegin(GL_LINE_LOOP);
      glVertex2i(10,10);
      glVertex2i(100,100);
      glVertex2i(100,200);
      glVertex2i(300,200);
      glEnd();
      glFlush();
}
