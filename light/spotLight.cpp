//
//  spotLight.cpp
//  flyEngine
//
//  Created by joe on 06/09/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "spotLight.h"
#include "camera.h"
#include "shaderMgr.h"
#include "material.h"

USE_NS_FLYENGINE

spotLight::spotLight(glm::vec3 color,material* mt,float cutOffEngleInner,float cutOffEngleOuter):light(color,mt){
    m_fcutOffInner=glm::cos(glm::radians(cutOffEngleInner));
    m_fcutOffOuter=glm::cos(glm::radians(cutOffEngleOuter));
    m_fEngleOuter=cutOffEngleOuter;
    m_fEngleInner=cutOffEngleInner;
}

void spotLight::glUpdateForCube(int program_id,int light_index,camera* c){
    int i=light_index;
    char szBuf[128]={0};
    //shader* shaderObj=shaderMgr::getShader(program_id);

    //启用光源
    snprintf(szBuf, sizeof(szBuf),uniform_name_light_spot_enabled,i);
    shaderMgr::setInt(program_id,szBuf, 1);

    //光源位置
    snprintf(szBuf, sizeof(szBuf),uniform_name_light_spot_pos,i);
    shaderMgr::setVec3(program_id,szBuf, glm::value_ptr(getPosition()));

    //光源颜色
    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_color,i);
    glm::vec3 color=getColor();
    shaderMgr::setVec3(program_id,szBuf, glm::value_ptr(color));

    //光源材质
    material* mt=getMaterial();
    //mt->glUpdateForSpotLight(program_id,i);
    
    //光源材质
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_ambient,i);
    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getAmbient()));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_diffuse,i);
    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getDiffuse()));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_specular,i);
    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getSpecular()));
    
    //角度的cos值
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_cutoff_inner,i);
    shaderMgr::setFloat(program_id, szBuf, m_fcutOffInner);
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_cutoff_outer,i);
    shaderMgr::setFloat(program_id, szBuf, m_fcutOffOuter);
    
    //方向向量
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_direction,i);
    shaderMgr::setVec3(program_id, szBuf,(float*)glm::value_ptr(c->getFront()));
    
    if(m_fConstant){
        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_constant,i);
        shaderMgr::setFloat(program_id, szBuf, m_fConstant);
        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_linear,i);
        shaderMgr::setFloat(program_id, szBuf, m_fLinear);
        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_quadratic,i);
        shaderMgr::setFloat(program_id, szBuf, m_fQuadratic);
    }
       
}
