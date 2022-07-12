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

#include <queue>

USE_NS_FLYENGINE

//directionLight::directionLight(glm::vec3 color){
//    m_vec3Dir=glm::vec3(-0.2f, -1.0f, -0.3f);
//    m_vec3Color=color;
//    m_fAmbient=0.1;
//    m_fDiffuse=0.1;
//    m_fSpecular=0.1;
//    m_IntShiness=0;
//}

directionLight::directionLight(glm::vec3 color,glm::vec3 dir,float am,float diff,float spec,int shine){
    m_vec3Color=color;
    m_vec3Dir=dir;
    m_fAmbient=am;
    m_fDiffuse=diff;
    m_fSpecular=spec;
    m_IntShiness=shine;
      
    
    std::queue<int> q;
    q.push(5);
    q.push(6);
    int s=q.front();
    int s2=q.back();
    flylog("queue top is %d end is %d size %d",s,s2,q.size());
}

//bool directionLight::init(){
////    return light::init();
//}


void directionLight::glUpdate(int program_id){
    shaderMgr::setInt(program_id,uniform_name_light_direction_enabled, 1);
    shaderMgr::setVec3(program_id,uniform_name_light_direction_direction, (float*)glm::value_ptr(m_vec3Dir));
    shaderMgr::setVec3(program_id,uniform_name_light_direction_color, (float*)glm::value_ptr(m_vec3Color));
    shaderMgr::setFloat(program_id,uniform_name_light_direction_ambient,m_fAmbient);
    shaderMgr::setFloat(program_id,uniform_name_light_direction_diffuse, m_fDiffuse);
    shaderMgr::setFloat(program_id,uniform_name_light_direction_specular, m_fSpecular);
    shaderMgr::setInt(program_id,uniform_name_light_direction_shiness, m_IntShiness);
}

//void directionLight::draw(camera* cameraObj){
//}
