//
//  testAniBySwapBuffer.cpp
//  flyEngine
//
//  Created by joe on 22/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//
#include <math.h>
#include "defines.h"
#include "testAniBySwapBuffer.h"
#include "testWindow.h"
#include "3DTrans.h"

USE_NS_FLYENGINE;

static int s_listID=0;
static int s_theta=0;
static structPos3 s_structPos={0,0,0};
static structPos3 s_structPosAxisStart={200,200,0};
static structPos3 s_structPosAxisEnd={200,200,1};
static float s_rotateTheta=0;
static bool s_bInit=false;

static void init(){
    glClearColor(1,1,1,0);
   
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    glVertex2f(s_structPosAxisStart.x,s_structPosAxisStart.y);
    glEnd();
    glFlush();
    
    s_listID=glGenLists(1);
    glNewList(s_listID, GL_COMPILE);
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    for(int k=0;k<6;k++){
        s_theta=2*M_PI*k/6;
        s_structPos.x=200+100*cos(s_theta);
        s_structPos.y=200+100*sin(s_theta);
        _3dTrans::_rotate(&s_structPos,&s_structPosAxisStart,&s_structPosAxisEnd, s_rotateTheta);
        glVertex2f(s_structPos.x,s_structPos.y);
    }
    glEnd();
    glEndList();
}

static void rotateCircle(){
    s_rotateTheta+=0.5;
    if(s_rotateTheta>360)
        s_rotateTheta-=360;
//    glutPostRedisplay();
}

static void drawAniBySwapBuffer(){
    if(!s_bInit)
        init();
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glCallList(s_listID);
//    glutSwapBuffers();
    glFlush();
}

static void onMouseClick(int button,int action,int x,int y){
    if(button==GLFW_MOUSE_BUTTON_LEFT){
        if(action==GLFW_PRESS){
//            glutIdleFunc(NULL);
        }else{
          
        }
    }else if(button==GLFW_MOUSE_BUTTON_MIDDLE){
        if(action==GLFW_PRESS){
//            glutIdleFunc(rotateCircle);
        }else{
           
        }
    }
}

void testAniBySwapBuffer(){
//    testInitWindow2D("openGL ani swapBuffers test", drawAniBySwapBuffer);
//    std::function<void(int,int,int,int)> mouseClick=onMouseClick;
//    testRegMouseClick(mouseClick);
//    windowLoop();
}
