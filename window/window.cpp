//
//  window.cpp
//  flyEngine
//
//  Created by joe on 23/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "window.h"
#include "logUtil.h"
#include "gpuUtil.h"
#include "keyboardEventMgr.h"
#include "mouseEventMgr.h"

USE_NS_FLYENGINE

GLFWwindow* g_window;
int g_screenWidth=0;
int g_screenHigh=0;

int g_winWidth=800;
int g_winHigh=600;

void window::printGpuInfo(){
    fprintf(stdout,"gpu brand:%s\ngpu vender:%s\nGL version:%s\nGLSL version:%s\nsupport etc:%d\nsupport pvr:%d\ngpu extension:\n%s\n",
           gpuUtil::getGPUBrand(),
           gpuUtil::getGPUVender(),
           gpuUtil::getGLVersion(),
            gpuUtil::getGLSLVersion(),
           gpuUtil::isSupportETC1(),
           gpuUtil::isSupportPVR(),
           gpuUtil::getGPUSupportExtensions()
           );
}

static void reshape2D(GLFWwindow* window,int w,int h)
{
    g_winWidth=w;
    g_winHigh=h;
    glViewport(0,0,w,h);
}


void window::init(){
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
        g_screenWidth=mode->width;
        g_screenHigh=mode->height;
        break;
    }

    GLFWwindow* window=glfwCreateWindow(g_winWidth, g_winHigh, "openGL test", NULL, NULL);
    if(!window){
       std::cout<<"glfwCreateWindow failed!"<<std::endl;
       glfwTerminate();
       return;
    }
    g_window=window;

    glfwSetWindowPos(window,(g_screenWidth-g_winWidth)/2,(g_screenHigh-g_winHigh)/2);
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
