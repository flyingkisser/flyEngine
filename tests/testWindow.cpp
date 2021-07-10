//
//  testMain.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include <map>
#include <stdio.h>
#include <unistd.h>
//#include <syswait.h>

#include "testWindow.h"
#include "flyEngine.h"
#include "gpuUtil.h"
#include "shader.h"
#include "shaderMgr.h"

using namespace flyEngine;
using namespace std;

static int s_winWidth=0;
static int s_winHeight=0;
static int s_intWidth=800;
static int s_intHeight=800;
static int s_menuID=1;
static int s_near=1;
static int s_far=1000;

GLFWwindow* g_window;

//static std::map<int,function<void(void)>> s_mapMenuCallback;

std::function<void(unsigned char,int,int)> s_keyboard_callback;
std::function<void(int,int,int,int)> s_mouse_click_callback;
std::function<void(int,int)> s_mouse_clickmove_callback;
std::function<void(int,int)> s_mouse_move_callback;

size getWindowSize(){
    return flyEngine::size{(float)s_intWidth,(float)s_intHeight};
}

static void reshape2D(GLFWwindow* window,int w,int h)
{
    s_intWidth=w;
    s_intHeight=h;
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    //gluOrtho2D(0.0,(GLdouble)w,0.0,(GLdouble)h);
//    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,w,h);
}

static void initOtho2D(){
    glClearColor(1.0,1.0,1.0,0);   //指定背影色为白色
    glClear(GL_COLOR_BUFFER_BIT);   //指定颜色缓存，该缓存使用glClearColor指定的背影色
    
//    glEnable(GL_BLEND);             //启用颜色混合
//    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);   //设置当前色和背影色的透明通道使用方式 color*a+bgColor*(1-a)
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();               //加载一个单位矩阵
//    //gluOrtho2D(0, s_intWidth, 0, s_intHeight);     //设置为正交投影
    glViewport(0,0,s_intWidth,s_intHeight);
}

static void reshape3D(GLFWwindow* window,int w,int h){
    s_intWidth=w;
    s_intHeight=h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    gluPerspective(30,w/h,s_near,s_far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    gluLookAt(0, 0, 0,
//                   0, 0, -1000,
//                   0, 1, 0);
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
//    gluPerspective(30,s_intWidth/s_intHeight,s_near,s_far);    //设置为透视投影，角度是30度，宽高化, 近平面s_near，远平面s_far
    //相机位于在(0,0,0)，看向(0,0,-1)，观察向量是[0,1,0];
//    gluLookAt(0, 0, 0,
//               0, 0, -1000,
//               0, 1, 0);
    glEnable(GL_CULL_FACE);     //激活剔除功能
    glCullFace(GL_BACK);        //剔除后向面 GL_FRONT_AND_BACK 消除场景中的所有多边形面，只观察点和直线段
}



//void onKeyboard(unsigned char c,int x,int y){
//    switch (c) {
//        case 'T':
//            break;
//
//        default:
//            break;
//    }
//    s_keyboard_callback(c,x,y);
//}
//void onMouseClick(int button,int action,int x,int y){
//    switch (button) {
//        case GLUT_LEFT_BUTTON:
//            break;
//        case GLUT_RIGHT_BUTTON:
//                  break;
//        case GLUT_MIDDLE_BUTTON:
//                  break;
//        default:
//            break;
//    }
//    switch (action) {
//       case GLUT_DOWN:
//           break;
//       case GLUT_UP:
//            break;
//       default:
//           break;
//      }
//    s_mouse_click_callback(button,action,x,s_intHeight-y);
//}
//void onMouseMoveClick(GLFWwindow* window,double x,double y){
//    s_mouse_clickmove_callback((int)x,(int)(s_intHeight-y));
//}
//void testRegKeyboard(std::function<void(unsigned char,int,int)>cb){
//    s_keyboard_callback=cb;
//    glutKeyboardFunc (onKeyboard);
//}
//void testRegMouseClick(std::function<void(int,int,int,int)>cb){
//    s_mouse_click_callback=cb;
//    glutMouseFunc(onMouseClick);
//}
//void testRegMouseClickWithMove(std::function<void(int,int)>cb){
//    s_mouse_clickmove_callback=cb;
//    glutMotionFunc(onMouseMoveClick);
//}
//void testRegMouseMove(std::function<void(int,int)>cb){
//    s_mouse_move_callback=cb;
//    glutPassiveMotionFunc(onMouseMove);
//}
//static void menuCallback(GLint menuID){
//    if(!s_mapMenuCallback[menuID])
//        return;
//    s_mapMenuCallback[menuID]();
//}

void onKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(g_window, GL_TRUE);
        return;
    }
    const char* keyName = glfwGetKeyName(key, scancode);
    
    switch (keyName[0]) {
        case 'T':
            break;
        default:
            break;
    }
    unsigned char clickChar=(unsigned char)keyName[0];
    s_keyboard_callback(clickChar,0,0);
}

void testRegKeyboard(std::function<void(unsigned char, int,int)>cb){
    s_keyboard_callback=cb;
    glfwSetKeyCallback(g_window, onKeyboard);
}

void onMouseClick(GLFWwindow* window,int button,int action,int mods){
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            break;
        default:
            break;
    }
    switch (action) {
       case GLFW_PRESS:
           break;
       case GLFW_RELEASE:
            break;
       default:
           break;
      }
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    s_mouse_click_callback(button,action,(int)x,(int)(s_intHeight-y));
}

void onMouseClickMove(GLFWwindow* window,double x,double y){
    if(s_mouse_move_callback){
        s_mouse_move_callback((int)x,(int)(s_intHeight-y));
    }
    if(s_mouse_clickmove_callback){
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
               glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)== GLFW_PRESS ||
               glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)== GLFW_PRESS)
               s_mouse_clickmove_callback((int)x,(int)(s_intHeight-y));
    }
}

void testRegMouseClick(std::function<void(int,int,int,int)>cb){
    s_mouse_click_callback=cb;
    glfwSetMouseButtonCallback(g_window, onMouseClick);
}

void testRegMouseMove(std::function<void(int,int)>cb){
    s_mouse_move_callback=cb;
    glfwSetCursorPosCallback(g_window,onMouseClickMove);
}

void testRegMouseClickWithMove(std::function<void(int,int)>cb){
    s_mouse_clickmove_callback=cb;
    glfwSetCursorPosCallback(g_window,onMouseClickMove);
}


void testRegMenu(const char* szMenuName,std::function<void(void)>cb){
//    s_mapMenuCallback[s_menuID]=cb;
//    glutAddMenuEntry(szMenuName, s_menuID++);
////    glutAttachMenu(GLUT_RIGHT_BUTTON);
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


void initWindow(){
    if(!glfwInit()){
        flylog("glfwInit failed!");
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //GLFW_OPENGL_COMPAT_PROFILE
    //GLFW_OPENGL_CORE_PROFILE
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    int monitorCount=0;
    //获取第一个屏幕的大小
    GLFWmonitor** pMonitor=glfwGetMonitors(&monitorCount);
    for(int i=0;i<monitorCount;i++){
        GLFWvidmode* mode=(GLFWvidmode*)glfwGetVideoMode(pMonitor[i]);
        s_winWidth=mode->width;
        s_winHeight=mode->height;
        g_winWidth=s_winWidth;
        g_winHeight=g_winHeight;
        break;
    }
    
    GLFWwindow* window=glfwCreateWindow(s_intWidth, s_intHeight, "openGL test", NULL, NULL);
    if(!window){
       std::cout<<"glfwCreateWindow failed!"<<std::endl;
       glfwTerminate();
       return;
    }
    g_window=window;
    
    glfwSetWindowPos(window,(g_winWidth-s_intWidth)/2,(g_winHeight-s_intHeight)/2);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, reshape2D);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
       std::cout<<"gladLoadGLLoader failed!"<<std::endl;
       glfwTerminate();
       return;
    }
    keyboardEventMgr::init(window);
    mouseEventMgr::init(window);
    printGpuInfo();
}


void windowLoop(){
   while(!glfwWindowShouldClose(g_window)){
        usleep(3000);
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
    std::cout<<"window loop end"<<std::endl;
    glfwTerminate();
}


//void testInitWindow2D(const char* szTitle,std::function<void(void)> drawCall) {
//    initOtho2D();
//    while (!glfwWindowShouldClose(g_window)){
//        drawCall();
//        glfwSwapBuffers(g_window);
//        glfwPollEvents();
//    }
//}

//void testInitWindow2D(const char* szTitle,std::function<void(void)> drawCall,unsigned int shaderID) {
//    initOtho2D();
//    if(!shaderID)
//        shaderID=flyEngine::shaderMgr::getDefaultShader();
//    flyEngine::shaderMgr::useShader(shaderID);
//
//    while (!glfwWindowShouldClose(g_window)){
////        shaderMgr::useShader(shaderID);
//        usleep(17*1000);   //1000 means 1ms
//        drawCall();
//        glfwSwapBuffers(g_window);
//        glfwPollEvents();
//    }
//}

//void testInitWindow2D(const char* szTitle,void (*drawCall)(void)) {
////    glutInitDisplayMode(displayMode);
////    glutInitWindowPosition(500,800);
////    glutInitWindowSize(s_intWidth,s_intHeight);
////    glutCreateWindow(szTitle);
////
////    printGpuInfo();
////
////    //init menu
////    glutCreateMenu(menuCallback);
////    glutAttachMenu(GLUT_RIGHT_BUTTON);
////
////    //register callbacks
////    glutDisplayFunc(displayCB);
////    glutReshapeFunc(reshape2D);
////
////    initOtho2D();
//
//    GLFWwindow* window=glfwCreateWindow(s_intWidth, s_intHeight, szTitle, NULL, NULL);
//    if(!window){
//       std::cout<<"glfwCreateWindow failed!"<<std::endl;
//       glfwTerminate();
//       return;
//    }
//    s_window=window;
//    glfwSetWindowPos(window,500,800);
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, reshape2D);
//    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
//       std::cout<<"gladLoadGLLoader failed!"<<std::endl;
//       glfwTerminate();
//       return;
//    }
//    printGpuInfo();
//    initOtho2D();
////    drawCall();
////    glfwSwapBuffers(s_window);
//}

//void testInitWindow2DNotUse(const char* szTitle,void (*drawCall)(void)) {
//    GLFWwindow* window=glfwCreateWindow(s_intWidth, s_intHeight, szTitle, NULL, NULL);
//    if(!window){
//       std::cout<<"glfwCreateWindow failed!"<<std::endl;
//       glfwTerminate();
//       return;
//    }
//    s_window=window;
//    glfwSetWindowPos(window,500,800);
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, reshape2D);
//    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
//       std::cout<<"gladLoadGLLoader failed!"<<std::endl;
//       glfwTerminate();
//       return;
//    }
//    printGpuInfo();
//    initOtho2D();
//
//    while (!glfwWindowShouldClose(window)){
//       glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//       glClear(GL_COLOR_BUFFER_BIT);
//
//       drawCall();
//
//       glfwSwapBuffers(window);
//       glfwPollEvents();
//    }
//}

//void testInitWindow3D(const char* szTitle,void (*drawCall)(void)) {
//    printGpuInfo();
//
//    GLFWwindow* window=glfwCreateWindow(s_intWidth, s_intHeight, szTitle, NULL, NULL);
//    if(!window){
//        std::cout<<"glfwCreateWindow failed!"<<std::endl;
//        glfwTerminate();
//        return;
//    }
//    s_window=window;
//    glfwSetWindowPos(window,500,800);
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, reshape3D);
//    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
//        std::cout<<"gladLoadGLLoader failed!"<<std::endl;
//        glfwTerminate();
//        return;
//    }
//
//    initPerspective3D();
//
//    drawCall();
//
//    glfwSwapBuffers(s_window);
//
////    while(!glfwWindowShouldClose(window)){
////        glfwPollEvents();
////        glfwSwapBuffers(window);
////    }
////
////    glfwTerminate();
//}
