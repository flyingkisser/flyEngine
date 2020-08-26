//
//  testMain.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "testWindow.h"
#include "defines.h"
#include <map>
#include "gpuUtil.h"
#include <stdio.h>


using namespace flyEngine;
using namespace std;

static int s_intWidth=800;
static int s_intHeight=800;
static int s_menuID=1;
static int s_near=1;
static int s_far=1000;
static std::map<int,function<void(void)>> s_mapMenuCallback;

std::function<void(unsigned char,int,int)> s_keyboard_callback;
std::function<void(int,int,int,int)> s_mouse_click_callback;
std::function<void(int,int)> s_mouse_clickmove_callback;
std::function<void(int,int)> s_mouse_move_callback;

static void reshape2D(int w,int h)
{
    s_intWidth=w;
    s_intHeight=h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLdouble)w,0.0,(GLdouble)h);
    glMatrixMode(GL_MODELVIEW);
}

static void initOtho2D(){
    glClearColor(1.0,1.0,1.0,0);   //指定背影色为白色
    glClear(GL_COLOR_BUFFER_BIT);   //指定颜色缓存，该缓存使用glClearColor指定的背影色
    glEnable(GL_BLEND);             //启用颜色混合
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);   //设置当前色和背影色的透明通道使用方式 color*a+bgColor*(1-a)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();               //加载一个单位矩阵
    gluOrtho2D(0, s_intWidth, 0, s_intHeight);     //设置为正交投影
}

static void reshape3D(int w,int h){
    s_intWidth=w;
    s_intHeight=h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30,w/h,s_near,s_far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 0,
                   0, 0, -1000,
                   0, 1, 0);
}

static void initPerspective3D(){
    //glClearDepth(0.5);              //指定深度缓存的初始值，即深度大于此值的多边形会被剔除
    //glClear(GL_DEPTH_BUFFER_BIT);   
    glClearColor(1.0,1.0,1.0,0);   //指定背影色为白色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //初始化颜色缓存，该缓存使用glClearColor指定的背影色，GL_DEPTH_BUFFER_BIT 初始化深度缓存（默认值都为1）
    glEnable(GL_BLEND);             //启用颜色混合
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);   //设置当前色和背影色的透明通道使用方式 color*a+bgColor*(1-a)
    glEnable(GL_DEPTH_TEST);        //启用深度缓存可见性检测，即按z轴的大小进行绘制
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();               //加载一个单位矩阵
    gluPerspective(30,s_intWidth/s_intHeight,s_near,s_far);    //设置为透视投影，角度是30度，宽高化, 近平面s_near，远平面s_far
    //相机位于在(0,0,0)，看向(0,0,-1)，观察向量是[0,1,0];
    gluLookAt(0, 0, 0,
               0, 0, -1000,
               0, 1, 0);
    glEnable(GL_CULL_FACE);     //激活剔除功能
    glCullFace(GL_BACK);        //剔除后向面 GL_FRONT_AND_BACK 消除场景中的所有多边形面，只观察点和直线段
}


void testRegMenu(const char* szMenuName,std::function<void(void)>cb){
    s_mapMenuCallback[s_menuID]=cb;
    glutAddMenuEntry(szMenuName, s_menuID++);
//    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void onKeyboard(unsigned char c,int x,int y){
    switch (c) {
        case 'T':
            break;
        
        default:
            break;
    }
    s_keyboard_callback(c,x,y);
}

void onMouseClick(int button,int action,int x,int y){
    switch (button) {
        case GLUT_LEFT_BUTTON:
            break;
        case GLUT_RIGHT_BUTTON:
                  break;
        case GLUT_MIDDLE_BUTTON:
                  break;
        default:
            break;
    }
    switch (action) {
       case GLUT_DOWN:
           break;
       case GLUT_UP:
            break;
       default:
           break;
      }
    s_mouse_click_callback(button,action,x,s_intHeight-y);
}

void onMouseMoveClick(int x,int y){
    s_mouse_clickmove_callback(x,s_intHeight-y);
}

void onMouseMove(int x,int y){
    s_mouse_move_callback(x,s_intHeight-y);
}

void testRegKeyboard(std::function<void(unsigned char,int,int)>cb){
    s_keyboard_callback=cb;
    glutKeyboardFunc (onKeyboard);
}

void testRegMouseClick(std::function<void(int,int,int,int)>cb){
    s_mouse_click_callback=cb;
    glutMouseFunc(onMouseClick);
}

void testRegMouseClickWithMove(std::function<void(int,int)>cb){
    s_mouse_clickmove_callback=cb;
    glutMotionFunc(onMouseMoveClick);
}

void testRegMouseMove(std::function<void(int,int)>cb){
    s_mouse_move_callback=cb;
    glutPassiveMotionFunc(onMouseMove);
}

static void menuCallback(GLint menuID){
    if(!s_mapMenuCallback[menuID])
        return;
    s_mapMenuCallback[menuID]();
}

void printGpuInfo(){
    fprintf(stdout,"gpu brand:%s\ngpu vender:%s\nGL version:%s\nGLSL version:%s\nsupport etc:%d\nsupport pvr:%d\ngpu extension:\n%s\n",
           getGPUBrand(),
           getGPUVender(),
           getGLVersion(),
            getGLSLVersion(),
           isSupportETC1(),
           isSupportPVR(),
           getGPUSupportExtensions()
           );
}

void testInitWindow2D(const char* szTitle,void (*displayCB)(void),int displayMode) {
    glutInitDisplayMode(displayMode);
    glutInitWindowPosition(500,800);
    glutInitWindowSize(s_intWidth,s_intHeight);
    glutCreateWindow(szTitle);
    
    printGpuInfo();
    
    //init menu
    glutCreateMenu(menuCallback);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    //register callbacks
    glutDisplayFunc(displayCB);
    glutReshapeFunc(reshape2D);

    initOtho2D();
}

void testInitWindow2D(const char* szTitle,void (*displayCB)(void)) {
    testInitWindow2D(szTitle,displayCB,GLUT_SINGLE|GLUT_RGBA);
//    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
//    glutInitWindowPosition(500,800);
//    glutInitWindowSize(s_intWidth,s_intHeight);
//    glutCreateWindow(szTitle);
//
//    //init menu
//    glutCreateMenu(menuCallback);
//    glutAttachMenu(GLUT_RIGHT_BUTTON);
//
//    //register callbacks
//    glutDisplayFunc(displayCB);
//    glutReshapeFunc(reshape2D);
//
//    initOtho2D();
}



void testInitWindow3D(const char* szTitle,void (*displayCB)(void)) {
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA|GLUT_DEPTH);  //GLUT_DEPTH 启用深度缓存
    glutInitWindowPosition(500,800);
    glutInitWindowSize(s_intWidth,s_intHeight);
    glutCreateWindow(szTitle);
    
    printGpuInfo();
    
    //init menu
    glutCreateMenu(menuCallback);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    //register callbacks
    glutDisplayFunc(displayCB);
    glutReshapeFunc(reshape3D);

    initPerspective3D();
}

void windowLoop(){
    // enter GLUT event processing cycle,will not return!
    glutMainLoop();
}
