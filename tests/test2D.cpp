//
//  test2D.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/7/4.
//  Copyright © 2021 joe. All rights reserved.
//

#include "test2D.h"

#include <math.h>

#include "defines.h"

#include "testWindow.h"
#include "texture.h"
#include "textureMgr.h"
#include "VAOMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "keyboardEventMgr.h"
#include "mouseEventMgr.h"
#include "cameraBase.h"
#include "cameraFPS.h"
#include "threadUtil.h"

using namespace flyEngine;

void drawPlane1() {
    float verticeArr[]={
        0.5,0.5,0,    1,1,
        0.5,-0.5,0,   1,0,
        -0.5,-0.5,0,  0,0,
        -0.5,0.5,0,   0,1
    };
    unsigned int verticeIndexArr[]={
        0,1,3,      //first triangle
        1,2,3       //second triangle
    };
    
    int shaderID=shaderMgr::createShaderFromFile("res/shader/1tex.vs","res/shader/1tex.fs");
    
    flyEngine::texture* texObj=(flyEngine::texture*)flyEngine::textureMgr::getInstance()->getTexture("res/smile.png");
    int texID1=texObj->getTextureID();
    if(!texID1)
        return;
    size texSize=texObj->getSize();
    size winSize=getWindowSize();
    
    float winWidth=winSize.width;
    float winHeight=winSize.height;
    float width=texSize.width;
    float height=texSize.height;
    //坐标设置为图片在屏幕正中心
    verticeArr[0]=width/winWidth;
    verticeArr[1]=height/winHeight;
    verticeArr[5*1+0]=width/winWidth;
    verticeArr[5*1+1]=-height/winHeight;
    verticeArr[5*2+0]=-width/winWidth;
    verticeArr[5*2+1]=-height/winHeight;
    verticeArr[5*3+0]=-width/winWidth;
    verticeArr[5*3+1]=height/winHeight;
      
    unsigned int vao,vbo,ebo;
    int stride=5*sizeof(float);
    
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&vbo);
    glGenBuffers(1,&ebo);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(verticeArr),verticeArr,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(verticeIndexArr),verticeIndexArr,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride,(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,stride,(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0); //0号纹理GL_TEXTURE0
  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texID1);
   
    glViewport(0,0,winWidth,winHeight);
    
    while(!glfwWindowShouldClose(g_window)){
        threadUtil::sleep(1);   //1000 means 1ms

        glClearColor(1.0,1.0,1.0,0.0);   //指定背影色为白色
        glClear(GL_COLOR_BUFFER_BIT);   //指定颜色缓存，该缓存使用glClearColor指定的背影色
        glBindVertexArray(vao);
//        flyEngine::shaderMgr::useShader(shaderID);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
}

void drawPlane2() {
    float verticeArr[]={
        0.5,0.5,0,    1,1,
        0.5,-0.5,0,   1,0,
        -0.5,-0.5,0,  0,0,
        -0.5,0.5,0,   0,1
    };
    unsigned int verticeIndexArr[]={
        0,1,3,      //first triangle
        1,2,3       //second triangle
    };
    
    int shaderID=shaderMgr::createShaderFromFile("res/shader/2tex.vs","res/shader/2tex.fs");
    
    flyEngine::texture* texObj1=(flyEngine::texture*)flyEngine::textureMgr::getInstance()->getTexture("res/wall.jpg");
    flyEngine::texture* texObj2=(flyEngine::texture*)flyEngine::textureMgr::getInstance()->getTexture("res/smile.png");
    int texID1=texObj1->getTextureID();
    int texID2=texObj2->getTextureID();
    if(!texID2 || !texID2)
        return;
    size texSize=texObj1->getSize();
    size winSize=getWindowSize();
    
    float winWidth=winSize.width;
    float winHeight=winSize.height;
    float width=texSize.width;
    float height=texSize.height;
   //坐标设置为图片在屏幕正中心
    verticeArr[0]=width/winWidth;
    verticeArr[1]=height/winHeight;
    verticeArr[5*1+0]=width/winWidth;
    verticeArr[5*1+1]=-height/winHeight;
    verticeArr[5*2+0]=-width/winWidth;
    verticeArr[5*2+1]=-height/winHeight;
    verticeArr[5*3+0]=-width/winWidth;
    verticeArr[5*3+1]=height/winHeight;
      
    
    unsigned int vao,vbo,ebo;
    int stride=5*sizeof(float);
    
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&vbo);
    glGenBuffers(1,&ebo);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(verticeArr),verticeArr,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(verticeIndexArr),verticeIndexArr,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride,(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,stride,(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
        
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0); //0号纹理GL_TEXTURE0
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1); //1号纹理GL_TEXTURE1
    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), 0.9);
  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texID1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texID2);
   
    glViewport(0,0,winWidth,winHeight);
    glUseProgram(shaderID);
    
    while(!glfwWindowShouldClose(g_window)){
        threadUtil::sleep(1);   //1000 means 1ms

        glClearColor(1.0,1.0,1.0,0);   //指定背影色为白色
        glClear(GL_COLOR_BUFFER_BIT);   //指定颜色缓存，该缓存使用glClearColor指定的背影色
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
}
