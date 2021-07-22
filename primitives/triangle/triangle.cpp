//
//  triangle.cpp
//  lines
//
//  Created by Joe on 2017/7/11.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "defines.h"
#include "triangle.h"
#include "shader.h"
#include "VAOMgr.h"
using namespace flyEngine;

void drawTriangleImm(p2* v1,p2* v2,p2* v3){
    glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);
    glVertex2f(v1->x,v1->y);
    glVertex2f(v2->x,v2->y);
    glVertex2f(v3->x,v3->y);
    glEnd();
    glFlush();
}

void drawTriangleImm(p2* v1,p2* v2,p2* v3,structColor* color){
    glBegin(GL_TRIANGLES);
    glColor4ub(color->r,color->g,color->b,color->a);
    glVertex2f(v1->x,v1->y);
    glVertex2f(v2->x,v2->y);
    glVertex2f(v3->x,v3->y);
    glEnd();
    glFlush();
}

void drawTriangleImm(structPos2* v1,structPos2* v2,structPos2* v3,structColor* color){
    glBegin(GL_TRIANGLES);
    glColor4ub(color->r,color->g,color->b,color->a);
    glVertex2f(v1->x,v1->y);
    glVertex2f(v2->x,v2->y);
    glVertex2f(v3->x,v3->y);
    glEnd();
    glFlush();
}


void drawTriangleImm(){
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
    checkGLError();
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
    checkGLError();
    
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

//std::function<void(void)> drawTriangle(){
//    float vertices[]={
//             -0.5f, -0.5f, 0.0f,
//              0.5f, -0.5f, 0.0f,
//              0.0f,  0.5f, 0.0f
//          };
//    unsigned int vao=flyEngine::VAOMgr::createVAO(vertices, sizeof(vertices), 3,false);
//    return [vao](){
//        VAOMgr::drawPrimitive(vao, GL_TRIANGLES, 3);
//    };
//}


std::function<void(void)> drawTriangle(structPos2* v1,structPos2* v2,structPos2* v3,structColor* color){
    GLbyte vertices[6]={0};
    vertices[0]=v1->x;
    vertices[1]=v1->y;
    vertices[2]=v2->x;
    vertices[3]=v2->y;
    vertices[4]=v3->x;
    vertices[5]=v3->y;
    unsigned int vao=flyEngine::VAOMgr::createVAO(vertices, sizeof(vertices), 2,0,true);
    return [vao](){
        VAOMgr::drawPrimitive(vao, GL_TRIANGLES, 3);
    };
}

std::function<void(void)> drawTriangle(float* v1,float* v2,float* v3,structColor* color){
    GLbyte vertices[6]={0};
    vertices[0]=v1[0];
    vertices[1]=v1[1];
    vertices[2]=v2[0];
    vertices[3]=v2[1];
    vertices[4]=v3[0];
    vertices[5]=v3[1];
    unsigned int vao=flyEngine::VAOMgr::createVAO(vertices, sizeof(vertices), 2,0,true);
    return [vao](){
        VAOMgr::drawPrimitive(vao, GL_TRIANGLES, 3);
    };
}

std::function<void(void)> drawTriangle(){
    //使用整数坐标时，如果指定的是GLbyte类型，认为坐标范围是[-127,127]，这里与glViewport里指定的视口大小没有任何关系
    //规范化的操作是变成f=value/127
    //如果指定int类型，规范化的操作是变成f=value/2^31,坐标范围是[-2^31,2^31]，是一个相当大的范围，这时指定50,100这样的数值，点与点的距离是相当近的
    //如果指定unsigned int类型，规范化的操作是变成f=value/2^32
    GLbyte vertices[]={
            0, 0, 0,
            127,0, 0,
            64, 127, 0
          };
    unsigned int vao=flyEngine::VAOMgr::createVAO(vertices, sizeof(vertices), 3,0,true);
    return [vao](){
        VAOMgr::drawPrimitive(vao, GL_TRIANGLES, 3);
    };
}
