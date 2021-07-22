//
//  lines.cpp
//  lines
//
//  Created by Joe on 2017/4/25.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "line.h"
#include "defines.h"
#include "error.h"

void drawLineByPosArrImm(structPos2* pointArr,int num,structColor* color){
    glColor4ub(color->r,color->g,color->b,color->a);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<num;i++)
        glVertex2f(pointArr[i].x,pointArr[i].y);
    glEnd();
    glFlush();
}


void drawLineStripImm(){
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

void drawLineImm(){
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

void drawLineLoopImm(){
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

std::function<void(void)> drawLines(){
//    GLbyte verticesInt[]={
//            0, 0, 0,
//            64,0, 0,
//            32, 64, 0
//          };
//    unsigned int vao=flyEngine::VAOMgr::createVAO(verticesInt, sizeof(verticesInt), 3,0,true);
    
    float vertices[]={
      0, 0, 0,
      0.5,0, 0,
        
      0.5,0, 0,
      0.25, 0.5, 0,
        
      0.25, 0.5, 0,
      0, 0, 0
    };
    unsigned int vao=flyEngine::VAOMgr::createVAO(vertices, sizeof(vertices), 3, 3*sizeof(float),false);
    return [vao](){
        VAOMgr::drawPrimitive(vao, GL_LINES, 6);
    };
}
