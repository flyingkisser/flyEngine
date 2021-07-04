//
//  test3D.cpp
//  flyEngine
//
//  Created by joe on 28/10/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "test3D.h"

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

void test3dView() {
    //drawPlane1();
}


void drawCube(){
    unsigned int vao,vbo,ebo;
    int texID1,texID2;
    int stride=5*sizeof(float);

    flyEngine::texture* texObj1=(flyEngine::texture*)textureMgr::getInstance()->getTexture("res/fire.png");
    flyEngine::texture* texObj2=(flyEngine::texture*)textureMgr::getInstance()->getTexture("res/smile.png");
    if(texObj1==NULL || texObj2==NULL)
       return;

    texID1=texObj1->getTextureID();
    texID2=texObj2->getTextureID();
   
    int shaderID=shaderMgr::createShader("res/shader/3dTextureApha.vs","res/shader/3dTextureApha.fs");
 
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&vbo);
    glGenBuffers(1,&ebo);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(g_verticeArr),g_verticeArr,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride,(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,stride,(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
        
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0); //0号纹理GL_TEXTURE0
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1); //1号纹理GL_TEXTURE1
    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), 0.7);
   
    glm::mat4 matModel=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-3));
    camera* cameraObj=new camera(shaderID);
    glUniformMatrix4fv(glGetUniformLocation(shaderID,"matModel"),1,GL_FALSE,glm::value_ptr(matModel));
    
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texID1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texID2);
    glEnable(GL_DEPTH_TEST);
    
    cameraObj->print();
    
    while(!glfwWindowShouldClose(g_window)){
        threadUtil::sleep(1);   //1000 means 1ms

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texID1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texID2);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);
        
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
   
}

/*
static std::function<void(void)> drawCubeRotate(int shaderID){
    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
    if(!texID1)
        return nullptr;
    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
    if(!texID2)
       return nullptr;
      
// void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerTexture,
//    int stride,int offsetVertex,int offsetTexture)
    int vao=VAOMgr::createVAO(s_verticeArr,sizeof(s_verticeArr),3,2,5*sizeof(float),0,3*sizeof(float));

    glm::mat4 matModel=glm::mat4(1.0);
    glm::mat4 matView=glm::mat4(1.0);
    glm::mat4 matProjection=glm::mat4(1.0);
    
    matModel=glm::rotate(matModel,(float)glfwGetTime(),glm::vec3(0.5,1,0));
    matView=glm::translate(matView,glm::vec3(0,0,-3.0));
    matProjection=glm::perspective(glm::radians(45.0), 800.0/600.0, 0.1, 100.0);
    
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), 0.2);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matModel"), 1,GL_FALSE,glm::value_ptr(matModel));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matView"), 1,GL_FALSE,glm::value_ptr(matView));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matProjection"), 1,GL_FALSE,glm::value_ptr(matProjection));
  
    glEnable(GL_DEPTH_TEST);
    
    return [vao,texID1,texID2,shaderID](){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texID1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texID2);
        glBindVertexArray(vao);
        
        glm::mat4 matModel=glm::rotate(glm::mat4(1.0),(float)glfwGetTime(),glm::vec3(0.5,1,0));
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "matModel"), 1,GL_FALSE,glm::value_ptr(matModel));
        
        glDrawArrays(GL_TRIANGLES,0,36);
    };
}

static std::function<void(void)> drawCubeControl(int shaderID){
    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
    if(!texID1)
        return nullptr;
    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
    if(!texID2)
       return nullptr;

    int vao=VAOMgr::createVAO(s_verticeArr,sizeof(s_verticeArr),3,2,5*sizeof(float),0,3*sizeof(float));
    
//    glm::mat4 mat_model=glm::rotate(glm::mat4(1.0),glm::radians(60.0f),glm::vec3(1,0,0));
//    mat_model=glm::translate(mat_model,glm::vec3(0,0,-2));
    
      glm::mat4 mat_model=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-90));
      mat_model=glm::rotate(mat_model,glm::radians(30.0f),glm::vec3(1,0,0));
      mat_model=glm::rotate(mat_model,glm::radians(30.0f),glm::vec3(0,1,0));
    
//    glm::mat4 mat_model=glm::mat4(1.0);
    glm::mat4 mat_camera=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-3.0));
    glm::mat4 mat_proj=glm::perspective(glm::radians(60.0), 800.0/600.0, 0.1, 100.0);
 
    
    kbmControl* kbmObj=new kbmControl(shaderID,
                                      std::string("matModel"),mat_model,
                                      std::string("matView"),mat_camera,
                                      std::string("matProjection"),mat_proj
                                      );
    
    
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), 0.2);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matModel"), 1,GL_FALSE,glm::value_ptr(mat_model));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matView"), 1,GL_FALSE,glm::value_ptr(mat_camera));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matProjection"), 1,GL_FALSE,glm::value_ptr(mat_proj));
  
    glEnable(GL_DEPTH_TEST);
    
    return [vao,texID1,texID2,shaderID](){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texID1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texID2);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);
    };
}

static std::function<void(void)> drawCubeMore(int shaderID){
    std::vector<glm::vec3> posVector={
                             glm::vec3(0,0,0),
                             glm::vec3( 2.0f,  5.0f, -15.0f),
                             glm::vec3(-1.5f, -2.2f, -2.5f),
                             glm::vec3(-3.8f, -2.0f, -12.3f),
                             glm::vec3( 2.4f, -0.4f, -3.5f),
                             glm::vec3(-1.7f,  3.0f, -7.5f),
                             glm::vec3( 1.3f, -2.0f, -2.5f),
                             glm::vec3( 1.5f,  2.0f, -2.5f),
                             glm::vec3( 1.5f,  0.2f, -1.5f),
                             glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
    if(!texID1)
        return nullptr;
    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
    if(!texID2)
       return nullptr;

    int vao=VAOMgr::createVAO(s_verticeArr,sizeof(s_verticeArr),3,2,5*sizeof(float),0,3*sizeof(float));
    
    glm::mat4 mat_model=glm::rotate(glm::mat4(1.0),glm::radians(60.0f),glm::vec3(1,1,1));
    mat_model=glm::translate(mat_model,glm::vec3(0,0,-2));
    glm::mat4 mat_camera=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-3.0));
    glm::mat4 mat_proj=glm::perspective(glm::radians(30.0), 800.0/600.0, 0.1, 100.0);
 
    
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), 0.2);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matModel"), 1,GL_FALSE,glm::value_ptr(mat_model));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matView"), 1,GL_FALSE,glm::value_ptr(mat_camera));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matProjection"), 1,GL_FALSE,glm::value_ptr(mat_proj));
  
    glEnable(GL_DEPTH_TEST);
    
    return [vao,texID1,texID2,shaderID,posVector](){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texID1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texID2);
        glBindVertexArray(vao);
        for(glm::vec3 pos : posVector){
            glm::mat4 matModel=glm::translate(glm::mat4(1.0),pos);
            matModel=glm::rotate(matModel,glm::radians((float)glfwGetTime()),glm::vec3(1,1,1));
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "matModel"), 1,GL_FALSE,glm::value_ptr(matModel));
            glDrawArrays(GL_TRIANGLES,0,36);
        }
    };
}


static std::function<void(void)> drawCubeControlByCamera(int shaderID){
    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
    if(!texID1)
        return nullptr;
    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
    if(!texID2)
       return nullptr;

    int vao=VAOMgr::createVAO(s_verticeArr,sizeof(s_verticeArr),3,2,5*sizeof(float),0,3*sizeof(float));
    
    glm::mat4 mat_model=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-3));
    glm::vec3 cameraPos=glm::vec3(0,0,3);
    glm::vec3 cameraUp=glm::vec3(0,1,0);
    camera* cameraObj=new camera(shaderID,
                                      std::string("matModel"),mat_model,
                                      std::string("matView"),cameraPos,cameraUp,
                                      std::string("matProjection"),800.0/600.0
                                      );
    
    
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), 0.2);

    glEnable(GL_DEPTH_TEST);
    
    cameraObj->print();
    cameraObj->updateCamera();
    
    return [vao,texID1,texID2,shaderID,cameraObj](){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texID1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texID2);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);
        cameraObj->updateCamera();
    };
}

static std::function<void(void)> drawCubeMoreByCamera(int shaderID){
    std::vector<glm::vec3> posVector={
                             glm::vec3(0,0,0),
                             glm::vec3( 2.0f,  5.0f, -15.0f),
                             glm::vec3(-1.5f, -2.2f, -2.5f),
                             glm::vec3(-3.8f, -2.0f, -12.3f),
                             glm::vec3( 2.4f, -0.4f, -3.5f),
                             glm::vec3(-1.7f,  3.0f, -7.5f),
                             glm::vec3( 1.3f, -2.0f, -2.5f),
                             glm::vec3( 1.5f,  2.0f, -2.5f),
                             glm::vec3( 1.5f,  0.2f, -1.5f),
                             glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
    if(!texID1)
        return nullptr;
    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
    if(!texID2)
       return nullptr;

    int vao=VAOMgr::createVAO(s_verticeArr,sizeof(s_verticeArr),3,2,5*sizeof(float),0,3*sizeof(float));
    
    glm::mat4 mat_model=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-3));
    glm::vec3 cameraPos=glm::vec3(0,0,3);
    glm::vec3 cameraUp=glm::vec3(0,1,0);
    camera* cameraObj=new camera(shaderID,
                                     std::string("matModel"),mat_model,
                                     std::string("matView"),cameraPos,cameraUp,
                                     std::string("matProjection"),800.0/600.0
                                     );
    
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), 0.2);

    glEnable(GL_DEPTH_TEST);
    
    return [vao,texID1,texID2,shaderID,posVector,cameraObj](){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texID1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texID2);
        glBindVertexArray(vao);
        for(glm::vec3 pos : posVector){
            glm::mat4 matModel=glm::translate(glm::mat4(1.0),pos);
            matModel=glm::rotate(matModel,glm::radians((float)glfwGetTime()),glm::vec3(1,1,1));
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "matModel"), 1,GL_FALSE,glm::value_ptr(matModel));
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        cameraObj->updateCamera();
    };
}

static std::function<void(void)> drawCubeMoreByCameraFPS(int shaderID){
    std::vector<glm::vec3> posVector={
                             glm::vec3(0,0,0),
                             glm::vec3( 2.0f,  5.0f, -15.0f),
                             glm::vec3(-1.5f, -2.2f, -2.5f),
                             glm::vec3(-3.8f, -2.0f, -12.3f),
                             glm::vec3( 2.4f, -0.4f, -3.5f),
                             glm::vec3(-1.7f,  3.0f, -7.5f),
                             glm::vec3( 1.3f, -2.0f, -2.5f),
                             glm::vec3( 1.5f,  2.0f, -2.5f),
                             glm::vec3( 1.5f,  0.2f, -1.5f),
                             glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
    if(!texID1)
        return nullptr;
    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
    if(!texID2)
       return nullptr;

    int vao=VAOMgr::createVAO(s_verticeArr,sizeof(s_verticeArr),3,2,5*sizeof(float),0,3*sizeof(float));
    
   glm::mat4 mat_model=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-3));
   glm::vec3 cameraPos=glm::vec3(0,0,3);
   glm::vec3 cameraUp=glm::vec3(0,1,0);

   camera* cameraObj=new camera(shaderID,
                                     std::string("matModel"),mat_model,
                                     std::string("matView"),cameraPos,cameraUp,
                                     std::string("matProjection"),800.0/600.0
                                     );
    
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), 0.2);
    glEnable(GL_DEPTH_TEST);
    
    return [vao,texID1,texID2,shaderID,posVector,cameraObj](){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texID1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texID2);
        glBindVertexArray(vao);
        for(glm::vec3 pos : posVector){
            glm::mat4 matModel=glm::translate(glm::mat4(1.0),pos);
            matModel=glm::rotate(matModel,glm::radians((float)glfwGetTime()),glm::vec3(1,1,1));
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "matModel"), 1,GL_FALSE,glm::value_ptr(matModel));
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        cameraObj->updateCamera();
    };
}
*/



/*
void test3dViewRotate() {
    int shaderID=shaderMgr::createShader("res/shader/3dTextureApha.vs","res/shader/3dTextureApha.fs");
    std::function<void(void)> drawCall=drawCubeRotate(shaderID);
    testInitWindow2D("openGL 3D png test",drawCall,shaderID);
    windowLoop();
}

void test3dViewControl() {
    int shaderID=shaderMgr::createShader("res/shader/3dTextureApha.vs","res/shader/3dTextureApha.fs");
    std::function<void(void)> drawCall=drawCubeControl(shaderID);
    testInitWindow2D("openGL 3D png test",drawCall,shaderID);
    windowLoop();
}

void test3dViewControlCamera() {
    int shaderID=shaderMgr::createShader("res/shader/3dTextureApha.vs","res/shader/3dTextureApha.fs");
    std::function<void(void)> drawCall=drawCubeControlByCamera(shaderID);
    testInitWindow2D("openGL 3D png camera",drawCall,shaderID);
    windowLoop();
}

void test3dViewMoreCube() {
    int shaderID=shaderMgr::createShader("res/shader/3dTextureApha.vs","res/shader/3dTextureApha.fs");
    std::function<void(void)> drawCall=drawCubeMore(shaderID);
    testInitWindow2D("openGL 3D png test",drawCall,shaderID);
    windowLoop();
}

void test3dViewMoreCubeCamera() {
    int shaderID=shaderMgr::createShader("res/shader/3dTextureApha.vs","res/shader/3dTextureApha.fs");
    std::function<void(void)> drawCall=drawCubeMoreByCamera(shaderID);
    testInitWindow2D("openGL 3D png more cube camera",drawCall,shaderID);
    windowLoop();
}

void test3dViewMoreCubeCameraFPS() {
    int shaderID=shaderMgr::createShader("res/shader/3dTextureApha.vs","res/shader/3dTextureApha.fs");
    std::function<void(void)> drawCall=drawCubeMoreByCameraFPS(shaderID);
    testInitWindow2D("openGL 3D png more cube camera fps",drawCall,shaderID);
    windowLoop();
}
*/
