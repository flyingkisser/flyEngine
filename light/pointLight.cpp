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
    light::glUpdateForCube(program_id,i);
    
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_constant,i);
    shaderMgr::setFloat(program_id, szBuf, m_fConstant);
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_linear,i);
    shaderMgr::setFloat(program_id, szBuf, m_fLinear);
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_quadratic,i);
    shaderMgr::setFloat(program_id, szBuf, m_fQuadratic);
}
