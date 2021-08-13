//
//  light.cpp
//  flyEngine
//
//  Created by joe on 10/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "light.h"

USE_NS_FLYENGINE

light::light(glm::vec4 color):cubeColor(color){
//    cubeColor(color);
//    m_vec3Color=color;
}

bool light::init(){
    return cubeColor::init();
//    _shaderObj=shaderMgr::get3dColorShader();
//    if(_shaderObj==NULL)
//       return false;
//    glInit();
//    return true;
}

void light::glInit(){
    return cubeColor::glInit();
    
//    node::glInitShader();
//    node::glInitVAO();
//    node::setPosition(glm::vec3(0,0,-10));
//    node::rotateBy(glm::vec3(30,0,30));
}

void light::draw(camera* cameraObj){
    cubeColor::draw(cameraObj);
    
//    node::updateModel(cameraObj);
//    glUniform3fv(glGetUniformLocation(_gl_program, "color"),1, glm::value_ptr(m_vec3Color));
//    glEnable(GL_DEPTH_TEST);
//    glBindVertexArray(_gl_vao);
//    glDrawArrays(GL_TRIANGLES,0,36);
}
