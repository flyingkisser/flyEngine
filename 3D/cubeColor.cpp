//
//  cubeColor.cpp
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "cubeColor.h"
#include "logUtil.h"
#include "texture.h"
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "action.h"
#include "camera.h"

USE_NS_FLYENGINE

cubeColor::cubeColor(glm::vec3 color){
    m_vec3Color=color;
}

bool cubeColor::init(){
    _shaderObj=shaderMgr::get3dColorShader();
    if(_shaderObj==NULL)
       return false;
    _gl_program=_shaderObj->getProgramID();
    glInit();
    return true;
}

void cubeColor::glInit(){
    node::glInitVAO();
    node::setPosition(glm::vec3(0,0,-10));
    node::rotateBy(glm::vec3(30,0,30));
}

void cubeColor::draw(camera* cameraObj){
    _shaderObj->use();
    cameraObj->update(_gl_program);
    node::updateModel(cameraObj);
    _shaderObj->setVec3("color", glm::value_ptr(m_vec3Color));

    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(_gl_vao);
    glDrawArrays(GL_TRIANGLES,0,36);
}
