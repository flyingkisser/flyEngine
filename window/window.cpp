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

#ifdef BUILD_MAC
#include "keyboardEventMgr.h"
#include "mouseEventMgr.h"
#elif BUILD_IOS
#include "windowUtil.h"
#include "ios_dirUtil.h"
#endif

USE_NS_FLYENGINE

GLFWwindow* g_window;
int g_screenWidth=0;
int g_screenHigh=0;

int g_winWidth=800;
int g_winHigh=600;
int g_shadowWidth=1024;
int g_shadowHigh=1024;

static void reshape2D(GLFWwindow* win,int w,int h)
{
    g_winWidth=w;
    g_winHigh=h;
    glViewport(0,0,w,h);
}


void window::init(){
#ifdef BUILD_MAC
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
    //设置msaa一个像素点进行4次采样
//    glfwWindowHint(GLFW_SAMPLES, 4);

    int monitorCount=0;
    //获取第一个屏幕的大小
    GLFWmonitor** pMonitor=glfwGetMonitors(&monitorCount);
    for(int i=0;i<monitorCount;i++){
        GLFWvidmode* mode=(GLFWvidmode*)glfwGetVideoMode(pMonitor[i]);
        g_screenWidth=mode->width;
        g_screenHigh=mode->height;
        break;
    }
    g_winWidth=g_screenWidth/2;
    g_winHigh=g_screenHigh/2;
   
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
    
    reshape2D(NULL,g_screenWidth,g_screenHigh);
    
#elif BUILD_IOS
    g_window=0;
    flyEngine::size st=windowUtil::getWinSize();
    g_winWidth=st.width;
    g_winHigh=st.height;
    st=windowUtil::getResolutionSize();
    g_screenWidth=st.width;
    g_screenHigh=st.height;
    g_shadowWidth=g_screenWidth;
    g_shadowHigh=g_screenHigh;
    
//    unsigned int depthRenderbuffer=0;
//    glGenRenderbuffers(1, &depthRenderbuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, g_winWidth, g_winHigh);
////    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
//    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
//    if(status != GL_FRAMEBUFFER_COMPLETE) {
//        flylog("failed to make complete framebuffer object %x", status);
//    }
#endif
}
