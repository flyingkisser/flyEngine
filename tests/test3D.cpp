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
#include "textureMgr.h"
#include "VAOMgr.h"
#include "shader.h"
#include "keyboardEventMgr.h"

using namespace flyEngine;

static std::function<void(void)> beginBasicView(int shaderID){
    float verticeArr[]={
        0.5,0.5,0,  1,0,0,  1,1,
        0.5,-0.5,0, 0,1,0,  1,0,
        -0.5,-0.5,0,0,0,1,  0,0,
        -0.5,0.5,0, 1,1,0,  0,1
    };
    unsigned int verticeIndexArr[]={
        0,1,3,      //first triangle
        1,2,3       //second triangle
    };
    
    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
    if(!texID1)
        return nullptr;
    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
    if(!texID2)
       return nullptr;
    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
    size winSize=getWindowSize();
    float winWith=winSize.width;
    float winHeight=winSize.height;
    float width=texSize.width;
    float height=texSize.height;
    verticeArr[0]=width/winWith;
    verticeArr[1]=height/winHeight;
    verticeArr[0+8*1]=width/winWith;
    verticeArr[1+8*1]=-height/winHeight;
    verticeArr[0+8*2]=-width/winWith;
    verticeArr[1+8*2]=-height/winHeight;
    verticeArr[0+8*3]=-width/winWith;
    verticeArr[1+8*3]=height/winHeight;
      
    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);

    glm::mat4 matModel=glm::mat4(1.0);
    
    glm::mat4 matView=glm::mat4(1.0);
    matView=glm::translate(matView,glm::vec3(0,0,-3.0));
    
    glm::mat4 matModel=glm::mat4(1.0);
    glm::mat4 projection=glm::perspective(glm::radians(45.0), 800.0/600.0, 0.1, 100.0);
   
    
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matrixTrans"), 1,GL_FALSE,glm::value_ptr(matTrans));
  
    return [vao,texID1,texID2,shaderID](){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texID1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texID2);
        glBindVertexArray(vao);
        
        glm::mat4 matTrans=glm::mat4(1.0);
        matTrans=glm::translate(matTrans,glm::vec3(0.5,-0.5,0));
        matTrans=glm::rotate(matTrans,(float)glfwGetTime(),glm::vec3(0,0,1));
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "matrixTrans"), 1,GL_FALSE,glm::value_ptr(matTrans));
        
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    };
}

void test3dView() {
    int shaderID=shaderMgr::createShader("res/shader/2dTextureAphaTrans.vs","res/shader/2dTextureAphaMix.fs");
    std::function<void(void)> drawCall=beginBasicView(shaderID);
    testInitWindow2D("openGL 2D png trans test",drawCall,shaderID);
    windowLoop();
}
