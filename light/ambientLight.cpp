//
//  ambientLight.cpp
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "ambientLight.h"
#include "shader.h"
#include "shaderMgr.h"
#include "material.h"

USE_NS_FLYENGINE

ambientLight::ambientLight(glm::vec3 color):light(color){
//    cubeColor(color);
//    m_vec3Color=color;
    m_vec3Direction=glm::vec3(-0.2f, -1.0f, -0.3f);
    float ambient=0.1;
    float diffuse=0.1;
    float specular=0.1;
    float shineness=0;
    material* mt=new material(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
    setMaterial(mt);
}

bool ambientLight::init(){
    return light::init();
}


void ambientLight::glUpdate(int program_id){
    shaderMgr::setInt(program_id,uniform_name_light_direction_enabled, 1);
    shaderMgr::setVec3(program_id,uniform_name_light_direction_direction, (float*)glm::value_ptr(m_vec3Direction));
    shaderMgr::setVec3(program_id,uniform_name_light_direction_color, (float*)glm::value_ptr(getColor()));
    shaderMgr::setVec3(program_id,uniform_name_light_direction_ambient, (float*)glm::value_ptr(m_material->getAmbient()));
    shaderMgr::setVec3(program_id,uniform_name_light_direction_diffuse, (float*)glm::value_ptr(m_material->getDiffuse()));
    shaderMgr::setVec3(program_id,uniform_name_light_direction_specular, (float*)glm::value_ptr(m_material->getSpecular()));
}

void ambientLight::draw(camera* cameraObj){
}
