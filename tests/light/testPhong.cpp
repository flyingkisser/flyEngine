//
//  testPhong.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/5/30.
//  Copyright © 2021 joe. All rights reserved.
//

#include "testPhong.h"


#include "test3D.h"

#include <math.h>

#include "defines.h"

#include "testWindow.h"
#include "textureMgr.h"
#include "VAOMgr.h"
#include "shader.h"
#include "keyboardEventMgr.h"
#include "mouseEventMgr.h"
#include "cameraBase.h"
#include "cameraFPS.h"

using namespace flyEngine;
static float s_verticeArr[]={
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

//static std::function<void(void)> drawCubeAmbient(int shaderID){
//    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
//    if(!texID1)
//        return nullptr;
//    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
//    if(!texID2)
//       return nullptr;
//
//    int vao=VAOMgr::createVAO(s_verticeArr,sizeof(s_verticeArr),3,2,5*sizeof(float),0,3*sizeof(float));
//
//    glm::mat4 mat_model=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-3));
////    glm::mat4 mat_proj=glm::perspective(glm::radians(60.0), 800.0/600.0, 0.1, 100.0);
//
//    glm::vec3 cameraPos=glm::vec3(0,0,3);
////    glm::vec3 cameraFront=glm::vec3(0,0,-1);
//    glm::vec3 cameraUp=glm::vec3(0,1,0);
//
//
//    camera* cameraObj=new camera(shaderID,
//                                      std::string("matModel"),mat_model,
//                                      std::string("matView"),cameraPos,cameraUp,
//                                      std::string("matProjection"),800.0/600.0
//                                      );
//
//
//    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
//    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
//    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), 0.2);
//
////    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matModel"), 1,GL_FALSE,glm::value_ptr(mat_model));
////    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matProjection"), 1,GL_FALSE,glm::value_ptr(mat_proj));
//
//    glEnable(GL_DEPTH_TEST);
//
//    cameraObj->print();
//    cameraObj->updateCamera();
//
//    return [vao,texID1,texID2,shaderID,cameraObj](){
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,texID1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D,texID2);
//        glBindVertexArray(vao);
//        glDrawArrays(GL_TRIANGLES,0,36);
//
//
//
//        cameraObj->updateCamera();
//    };
//}

void testPhongAmbient() {
//    int shaderID=shaderMgr::createShader("res/shader/light_normal.vs","res/shader/light_ambient.fs");
//    std::function<void(void)> drawCall=drawCubeAmbient(shaderID);
//    testInitWindow2D("flyEngine:lighting ambient test",drawCall,shaderID);
//    windowLoop();
}



