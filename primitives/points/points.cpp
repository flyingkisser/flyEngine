//
//  points.cpp
//  lines
//
//  Created by Joe on 2017/4/28.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "points.h"
#include "defines.h"
#include "shader.h"
#include "VAOMgr.h"

USE_NS_FLYENGINE

void drawPointsImm(){
    //glClearColor(0, 0, 0, 0);
    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 0, 0);
    glPointSize(5);
    glBegin (GL_POINTS);
    glVertex2f (10, 10);
    glVertex2f (100, 100);
    glVertex2i (200, 150);
     glVertex2i (220, 150);
     glVertex2i (240, 150);
    glEnd();
    glFlush();
}

void drawPointsImm2(){
    unsigned int vao,vbo;
    float vertices[]={
        10,10,
        100,100,
        200,150,
        220,150,
        240,150
    };
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    shader* shaderObj=new shader("./res/shader/basic.vs","./res/shader/basic.fs");
    shaderObj->use();
    //参数1：绘制的图元类型
    //参数2：顶点索引的开始位置
    //参数3：顶点个数
    glDrawArrays(GL_POINTS,0,5);
}

std::function<void(void)> drawPoints(){
    //使用整数坐标时，如果指定的是GLbyte类型，认为坐标范围是[-127,127]，这里与glViewport里指定的视口大小没有任何关系
    //规范化的操作是变成f=value/127
    //如果指定int类型，规范化的操作是变成f=value/2^31,坐标范围是[-2^31,2^31]，是一个相当大的范围，这时指定50,100这样的数值，点与点的距离是相当近的
    //如果指定unsigned int类型，规范化的操作是变成f=value/2^32
   
//    GLbyte verticesInt[]={
//            0, 0, 0,
//            64,0, 0,
//            32, 64, 0
//          };
//    unsigned int vao=flyEngine::VAOMgr::createVAO(verticesInt, sizeof(verticesInt), 3,0,true);
    
    float vertices[]={
      0, 0, 0,
      0.5,0, 0,
      0.25, 0.5, 0
    };
    unsigned int vao=VAOMgr::createVAO(vertices, sizeof(vertices), 3,3*sizeof(float),false);
    return [vao](){
        VAOMgr::drawPrimitive(vao, GL_POINTS, 3);
    };
}
