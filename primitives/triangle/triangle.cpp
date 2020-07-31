//
//  triangle.cpp
//  lines
//
//  Created by Joe on 2017/7/11.
//  Copyright © 2017年 joe. All rights reserved.
//


#include <GLUT/glut.h>
#include "defines.h"
#include "triangle.h"
using namespace flyEngine;

void drawTriangle(p2* v1,p2* v2,p2* v3){
    glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);
    glVertex2f(v1->x,v1->y);
    glVertex2f(v2->x,v2->y);
    glVertex2f(v3->x,v3->y);
    glEnd();
    glFlush();
}

void drawTriangle(p2* v1,p2* v2,p2* v3,structColor* color){
    glBegin(GL_TRIANGLES);
    glColor4ub(color->r,color->g,color->b,color->a);
    glVertex2f(v1->x,v1->y);
    glVertex2f(v2->x,v2->y);
    glVertex2f(v3->x,v3->y);
    glEnd();
    glFlush();
}

void drawTriangle(structPos2* v1,structPos2* v2,structPos2* v3,structColor* color){
    glBegin(GL_TRIANGLES);
    glColor4ub(color->r,color->g,color->b,color->a);
    glVertex2f(v1->x,v1->y);
    glVertex2f(v2->x,v2->y);
    glVertex2f(v3->x,v3->y);
    glEnd();
    glFlush();
}



void drawTriangle(){
    GLint p1[]={1,100};
    GLint p2[]={100,1};
    GLint p3[]={100,200};
    GLint p4[]={250,1};
    GLint p5[]={350,100};
    GLint p6[]={250,200};
    glColor3f (1, 0, 0);
	//每三个顶点构成一个三角形
    
    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);
    glVertex2iv(p1);
    glColor3ub(255,128,3);
    glVertex2iv(p2);
    glColor3ub(1,253,10);
    glVertex2iv(p3);
    glVertex2iv(p4);
    glVertex2iv(p5);
    glVertex2iv(p6);
    glEnd();
    glFlush();
    errorCheck();
    return;
    
    //每三个顶点构成一个三角形,中间或连接起来
    /*
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2iv(p1);
    glVertex2iv(p2);
    glVertex2iv(p3);
    glVertex2iv(p4);
    glVertex2iv(p6);
    glVertex2iv(p5);
    glEnd();
    glFlush();
    errorCheck();
    */
    
    //由p1,p2,p3购成第一个三角形
    //由p1,p3,p4购成第二个三角形
    //由p1,p4,p5购成第三个三角形
    //由p1,p5,p6购成第四个三角形
    glBegin(GL_TRIANGLE_FAN);
    glVertex2iv(p1);
    glVertex2iv(p2);
    glVertex2iv(p3);
    glVertex2iv(p4);
    glVertex2iv(p6);
    glVertex2iv(p5);
    glEnd();
    glFlush();
    errorCheck();
    
/*
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2iv(p4);
    glVertex2iv(p5);
    glVertex2iv(p6);
    glEnd();
    
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2iv(p1);
    glVertex2iv(p2);
    glVertex2iv(p3);
    glEnd();
   
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex2iv(p1);
    glVertex2iv(p2);
    glVertex2iv(p3);
    glVertex2iv(p4);
    glVertex2iv(p5);
    glVertex2iv(p6);
    glEnd();
    
    glFlush();
    errorCheck();
      */
}
