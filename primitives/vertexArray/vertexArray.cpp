//
//  vertexArray.cpp
//  lines
//
//  Created by Joe on 2017/7/12.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "vertexArray.h"
#include "defines.h"
#include "error.h"
typedef GLint vertex3 [3];
GLint width=200;
vertex3 points[8]={{0,0,0},{0,width,0},{width,0,0},{width,width,0},{0,0,width},{0,width,width},{width,0,width},{width,width,width}};

vertex3 points2[8]={{0,0,0},{0,width,0},{width,0,0},{width,width,0},{0,0,width},{0,width,width},{width,0,width},{width,width,width}};

void _drawQuad(GLint n1,GLint n2,GLint n3,GLint n4){
    //draw一个四边形，由4个顶点指定位置
    glBegin(GL_QUADS);
    glVertex3iv(points[n1]);
    glVertex3iv(points[n2]);
    glVertex3iv(points[n3]);
    glVertex3iv(points[n4]);
    glEnd();
}

void drawCube(){
    glColor3f (1, 1, 0);
    //draw六个四边形
    _drawQuad(6, 2, 3, 7);
    _drawQuad(5, 1, 0, 4);
    _drawQuad(7, 3, 1, 5);
    _drawQuad(4, 0, 2, 6);
    _drawQuad(2, 0, 1, 3);
    _drawQuad(7, 5, 4, 6);
    glFlush();
}

void drawElementsByVertexArr(){
    GLubyte vertexIndexArr[]={6,2,3,7,5,1,0,4,7,3,1,5,4,0,2,6,2,0,1,3,7,5,4,6};
    glColor3f (1, 1, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_INT,0,points);
    //使用顶点数组vertexIndexArr，绘制四边形，给出了24个顶点，结果是应该绘制出6个四边形
    glDrawElements(GL_QUADS,24,GL_UNSIGNED_BYTE,vertexIndexArr);
    glFlush();
}

void draw(){
    glFlush();
    checkGLError();
}
