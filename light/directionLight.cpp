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
#include "logUtil.h"
#include "uboMgr.h"

#include <queue>

USE_NS_FLYENGINE

directionLight::directionLight(glm::vec3 color,glm::vec3 dir,float am,float diff,float spec,float shine){
    m_vec3Color=color;
    m_vec3Dir=dir;
    m_fAmbient=am;
    m_fDiffuse=diff;
    m_fSpecular=spec;
    m_fShiness=shine;
    // _ubo=uboMgr::createUBO(ubo_binding_light_dir, ubo_size_light_dir,"light_direction");
    _ubo=_ubo=g_ubo_id_arr[ubo_binding_light_dir0];
    _dirty=true;
    update();
}

void directionLight::update(){
    if(!_dirty)
        return;
    _dirty=false;
    glUpdate();
}

void directionLight::glUpdate(){
    int sizeArr[]={4,12,12,4,4,4,4};
    int offsetArr[]={0,16,32,44,48,52,56};
    int enabled=1;
    void* bufArr[]={
        &enabled,
        glm::value_ptr(m_vec3Dir),
        glm::value_ptr(m_vec3Color),
        &m_fAmbient,
        &m_fDiffuse,
        &m_fSpecular,
        &m_fShiness
    };
    uboMgr::writeData(_ubo, 7, sizeArr,offsetArr,bufArr);
    
//    shaderMgr::setInt(program_id,uniform_name_light_direction_enabled, 1);
//    shaderMgr::setVec3(program_id,uniform_name_light_direction_direction, (float*)glm::value_ptr(m_vec3Dir));
//    shaderMgr::setVec3(program_id,uniform_name_light_direction_color, (float*)glm::value_ptr(m_vec3Color));
//    shaderMgr::setFloat(program_id,uniform_name_light_direction_ambient,m_fAmbient);
//    shaderMgr::setFloat(program_id,uniform_name_light_direction_diffuse, m_fDiffuse);
//    shaderMgr::setFloat(program_id,uniform_name_light_direction_specular, m_fSpecular);
//    shaderMgr::setInt(program_id,uniform_name_light_direction_shiness, m_IntShiness);
}
