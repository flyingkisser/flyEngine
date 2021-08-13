//
//  ambientLight.cpp
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "ambientLight.h"

USE_NS_FLYENGINE

ambientLight::ambientLight(glm::vec4 color):light(color){
//    cubeColor(color);
//    m_vec3Color=color;
}

bool ambientLight::init(){
    return light::init();
}

void ambientLight::glInit(){
    light::glInit();
    
//    node::glInitShader();
//    node::glInitVAO();
//    node::setPosition(glm::vec3(0,0,-10));
//    node::rotateBy(glm::vec3(30,0,30));
}

void ambientLight::draw(camera* cameraObj){
//    cubeColor::draw(cameraObj);
    
//    node::updateModel(cameraObj);
//    glUniform3fv(glGetUniformLocation(_gl_program, "color"),1, glm::value_ptr(m_vec3Color));
//    glEnable(GL_DEPTH_TEST);
//    glBindVertexArray(_gl_vao);
//    glDrawArrays(GL_TRIANGLES,0,36);
}
