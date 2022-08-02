//
//  directionLight.cpp
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "directionLight.h"
#include "shader.h"
#include "shaderMgr.h"
#include "material.h"
#include "logUtil.h"
#include "uboMgr.h"

#include <queue>

USE_NS_FLYENGINE

directionLight::directionLight(glm::vec3 color,glm::vec3 dir,float am,float diff,float spec,int shine){
    m_vec3Color=color;
    m_vec3Dir=dir;
    m_fAmbient=am;
    m_fDiffuse=diff;
    m_fSpecular=spec;
    m_IntShiness=shine;
    _ubo=uboMgr::createUBO(ubo_binding_light_dir, ubo_size_light_dir);
}

void directionLight::glUpdate(int program_id){
    int v=1;
    m_fAmbient=1.0f;
    m_vec3Color=glm::vec3(1.0f,1.0f,1.0f);
    glBindBuffer(GL_UNIFORM_BUFFER,_ubo);
    
    glBufferSubData(GL_UNIFORM_BUFFER,0,4,&v);
    glBufferSubData(GL_UNIFORM_BUFFER,4,16,glm::value_ptr(m_vec3Dir));
    glBufferSubData(GL_UNIFORM_BUFFER,20,16,glm::value_ptr(m_vec3Color));
    glBufferSubData(GL_UNIFORM_BUFFER,36,4,&m_fAmbient);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    return;
    
//    int v=0;
//    m_fAmbient=1.0f;
//    glBindBuffer(GL_UNIFORM_BUFFER,_ubo);
//    glBufferSubData(GL_UNIFORM_BUFFER,0,4,&v);
//    glBufferSubData(GL_UNIFORM_BUFFER,4,12,glm::value_ptr(m_vec3Dir));
//    glBufferSubData(GL_UNIFORM_BUFFER,16,12,glm::value_ptr(m_vec3Color));
//    glBufferSubData(GL_UNIFORM_BUFFER,36,4,&m_fAmbient);
//    return;
    
//    uboMgr::writeData(_ubo, 7, <#int *sizeArr#>, <#void **bufArr#>)
    int sizeArr[]={4,12,12,4,4,4,4};
    int enabled=1;
    void* bufArr[]={
        &enabled,
        glm::value_ptr(m_vec3Dir),
        glm::value_ptr(m_vec3Color),
        &m_fAmbient,
        &m_fDiffuse,
        &m_fSpecular,
        &m_IntShiness
    };
    uboMgr::writeData(_ubo, 7, sizeArr, bufArr);
    
//    shaderMgr::setInt(program_id,uniform_name_light_direction_enabled, 1);
//    shaderMgr::setVec3(program_id,uniform_name_light_direction_direction, (float*)glm::value_ptr(m_vec3Dir));
//    shaderMgr::setVec3(program_id,uniform_name_light_direction_color, (float*)glm::value_ptr(m_vec3Color));
//    shaderMgr::setFloat(program_id,uniform_name_light_direction_ambient,m_fAmbient);
//    shaderMgr::setFloat(program_id,uniform_name_light_direction_diffuse, m_fDiffuse);
//    shaderMgr::setFloat(program_id,uniform_name_light_direction_specular, m_fSpecular);
//    shaderMgr::setInt(program_id,uniform_name_light_direction_shiness, m_IntShiness);
}
