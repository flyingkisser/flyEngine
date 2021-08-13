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

cubeColor::cubeColor(glm::vec4 color){
    m_vec4Color=color;
}

bool cubeColor::init(){
    _shaderObj=shaderMgr::get3dColorShader();
    if(_shaderObj==NULL)
       return false;
    glInit();
    return true;
}

void cubeColor::glInit(){
    node::glInitShader();
    node::glInitVAO();
    node::setPosition(glm::vec3(0,0,-10));
    node::rotateBy(glm::vec3(30,0,30));
}

void cubeColor::draw(camera* cameraObj){
    node::updateModel(cameraObj);
    glUniform4fv(glGetUniformLocation(_gl_program, "color"),1, glm::value_ptr(m_vec4Color));
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(_gl_vao);
    glDrawArrays(GL_TRIANGLES,0,36);
}
