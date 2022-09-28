//
//  billboard.cpp
//  flyEngine
//
//  Created by joe on 21/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "billboard.h"

#include "logUtil.h"
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "action.h"
#include "camera.h"
#include "state.h"

USE_NS_FLYENGINE

billboard::billboard(glm::vec3 color){
    m_vec3Color=color;
}

bool billboard::init(){
    setPosition(glm::vec3(0,0,-10));
    rotateBy(glm::vec3(30,0,30));
    
    _shaderObj=shaderMgr::get3dColorShader();
    if(_shaderObj==NULL)
       return false;
    _gl_program=_shaderObj->getProgramID();
    glInit();
    return true;
}

void billboard::glInit(){
    int descArr[]={3};
    initVAO(g_verticeArr, sizeof(g_verticeArr), descArr, 1);
}

void billboard::draw(){
    _shaderObj->use();
    node::updateModel();
    _shaderObj->setVec3("color", glm::value_ptr(m_vec3Color));

    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(_gl_vao);
    glDrawArrays(GL_TRIANGLES,0,36);
    state::log(36);
}
