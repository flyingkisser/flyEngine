//
//  pointLight.cpp
//  flyEngine
//
//  Created by joe on 06/09/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "pointLight.h"
#include "shaderMgr.h"

USE_NS_FLYENGINE


pointLight::pointLight(glm::vec3 color,material* mt,float constant,float linear,float quadratic):light(color,mt){
    m_fConstant=constant;
    m_fLinear=linear;
    m_fQuadratic=quadratic;
}

//支持多个点光源,light_index为点光源序号，从0开始
void pointLight::glUpdateForCube(int program_id, int light_index){
    int i=light_index;
    char szBuf[128]={0};
    //light::glUpdateForCube(program_id,i);
    
    //启用光源
    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_enabled,i);
    shaderObj->setInt(szBuf, 1);

    //光源位置
    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_pos,i);
    shaderObj->setVec3(szBuf, glm::value_ptr(getPosition()));

    //光源颜色
    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_color,i);
    glm::vec3 color=getColor();
    shaderObj->setVec3(szBuf, glm::value_ptr(color));

    //光源材质
    material* mt=getMaterial();
    //mt->glUpdateForPointLight(program_id,i);

    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_ambient,i);
    shaderObj->setVec3(szBuf, (float*)glm::value_ptr(mt->getAmbient()));

    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_diffuse,i);
    shaderObj->setVec3(szBuf, (float*)glm::value_ptr(mt->getDiffuse()));

    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_specular,i);
    shaderObj->setVec3(szBuf, (float*)glm::value_ptr(mt->getSpecular()));
    
    
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_constant,i);
    shaderMgr::setFloat(program_id, szBuf, m_fConstant);
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_linear,i);
    shaderMgr::setFloat(program_id, szBuf, m_fLinear);
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_quadratic,i);
    shaderMgr::setFloat(program_id, szBuf, m_fQuadratic);
}
