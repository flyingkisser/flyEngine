//
//  test2D.cpp
//  flyEngine
//
//  Created by joe on 30/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "test2D.h"
#include "defines.h"
#include "line.h"
#include "triangle.h"
#include "2dTrans.h"
#include <math.h>

#include "testWindow.h"

using namespace flyEngine;

static void beginBasicTrans(void) {
    glClearColor(1.0,1.0,1.0,0);    //指定背影色为白色
    glClear(GL_COLOR_BUFFER_BIT);   //指定颜色缓存，该缓存使用glClearColor指定的背影色
    p2* v1=new p2(10,10);
    p2* v2=new p2(30,10);
    p2* v3=new p2(20,30);
    structColor color1={0,255,0,100};
    float triangleWidth=20;
    float triangleHeight=pow(400-100,0.5);
    float centerX=10+triangleWidth/2;
    float centerY=10+triangleHeight/2;
    drawTriangle(v1,v2,v3,&color1);
    float moveX=100;
    float moveY=200;

    
    //移动
    structColor color2={210,210,210,255};
    glLoadIdentity();
    glTranslatef(moveX, moveY, 0);
    drawTriangle(v1,v2,v3,&color2);
    
    //缩放
    //先移动到-centerX,-centerY
    //再缩放3,3
    //再移动回centerX,centerY
    //注意变换的矩阵是右乘的，所以实际是按照调用顺序从后往前左乘
    structColor color3={255,0,0,80};
    glLoadIdentity();
    glTranslatef(centerX, centerY, 0);
    glScalef(3, 3, 0);
    glTranslatef(-centerX, -centerY, 0);
    drawTriangle(v1,v2,v3,&color3);
    
    //旋转
    structColor color4={0,10,250,150};
    glLoadIdentity();
    glTranslatef(moveX, moveY, 0);
    glTranslatef(centerX, centerY, 0);
    glRotatef(180, 0, 0,1);    //M_PI,这里使用的度数，不是pi值，180表示旋转180度
    glTranslatef(-centerX, -centerY, 0);
    drawTriangle(v1,v2,v3,&color4);
}

static void beginBasicView(){
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-201,201,-201,201);
    glMatrixMode(GL_MODELVIEW);
    
    structColor colorLine={255,0,0,255};
    structPos2 posArr1[4]={{-200,-200},{200,-200},{200,200},{-200,200}};
    structPos2 posArr2[2]={{-200,0},{200,0}};
    structPos2 posArr3[2]={{0,-200},{0,200}};
    structPos2 posArr10[4]={{-100,-100},{100,-100},{100,100},{-100,100}};
    
    structColor color1={210,210,210,255};
    structPos2 p1={-50,-25};
    structPos2 p2={50,-25};
    structPos2 p3={0,25};
    glViewport(0, 0, 300, 300);
    
    drawLineByPosArr(posArr1,4,&colorLine);
    drawLineByPosArr(posArr2,2,&colorLine);
    drawLineByPosArr(posArr3,2,&colorLine);
    drawLineByPosArr(posArr10,4,&colorLine);
    drawTriangle(&p1,&p2,&p3,&color1);
    
    glViewport(400, 0, 300, 300);
    
    drawLineByPosArr(posArr1,4,&colorLine);
    drawLineByPosArr(posArr2,2,&colorLine);
    drawLineByPosArr(posArr3,2,&colorLine);
    drawLineByPosArr(posArr10,4,&colorLine);
    drawTriangle(&p1,&p2,&p3,&color1);
    
}

void test2dTrans() {
    testInitWindow2D("openGL 2D trans test", beginBasicTrans);
    testRegMenu("click1", [](){
        printf("btn1 clicked!");
    });
    windowLoop();
}

void test2dView() {
    testInitWindow2D("openGL 2D view test", beginBasicView);
    windowLoop();
}
