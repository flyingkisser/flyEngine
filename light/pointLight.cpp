//
//  pointLight.cpp
//  flyEngine
//
//  Created by joe on 06/09/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "pointLight.h"
#include "shaderMgr.h"
#include "material.h"

USE_NS_FLYENGINE


pointLight::pointLight(glm::vec3 color,material* mt,float constant,float linear,float quadratic):light(color,mt){
    m_fConstant=constant;
    m_fLinear=linear;
    m_fQuadratic=quadratic;
    m_fAmbient=0;
    m_fDiffuse=0;
    m_fSpecular=0;
    m_IntShiness=0;
}

pointLight::pointLight(glm::vec3 color,glm::vec3 pos,float am,float diff,float spe,int shiness):light(color,nullptr){
    m_fConstant=0;
    m_fLinear=0;
    m_fQuadratic=0;
    
    m_fAmbient=am;
    m_fDiffuse=diff;
    m_fSpecular=spe;
    m_IntShiness=shiness;
    setPosition(pos);
}

void pointLight::glUpdate(int program_id, int light_index){
    int i=light_index;
    char szBuf[128]={0};
    
    //启用光源
    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_enabled,i);
    shaderMgr::setInt(program_id, szBuf, 1);
   
    //光源位置
    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_pos,i);
    shaderMgr::setVec3(program_id,szBuf,glm::value_ptr(getPosition()));

    //光源颜色
    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_color,i);
    glm::vec3 color=getColor();
    shaderMgr::setVec3(program_id,szBuf,glm::value_ptr(color));

    //Phong光照模型参数
    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_ambient,i);
    shaderMgr::setFloat(program_id,szBuf, m_fAmbient);

    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_diffuse,i);
    shaderMgr::setFloat(program_id,szBuf, m_fDiffuse);

    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_specular,i);
    shaderMgr::setFloat(program_id,szBuf, m_fSpecular);
    
    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_shiness,i);
    shaderMgr::setInt(program_id,szBuf, m_IntShiness);
    
    if(m_fConstant){
        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_constant,i);
        shaderMgr::setFloat(program_id, szBuf, m_fConstant);
    }
    if(m_fLinear){
        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_linear,i);
        shaderMgr::setFloat(program_id, szBuf, m_fLinear);
    }
    if(m_fQuadratic){
        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_quadratic,i);
        shaderMgr::setFloat(program_id, szBuf, m_fQuadratic);
    }
}


//支持多个点光源,light_index为点光源序号，从0开始
void pointLight::glUpdateUseMaterail(int program_id, int light_index){
    int i=light_index;
    char szBuf[128]={0};
    //light::glUpdateForCube(program_id,i);
    
    //启用光源
    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_enabled,i);
    shaderMgr::setInt(program_id, szBuf, 1);
   
    //光源位置
    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_pos,i);
    shaderMgr::setVec3(program_id,szBuf, glm::value_ptr(getPosition()));

    //光源颜色
    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_color,i);
    glm::vec3 color=getColor();
    shaderMgr::setVec3(program_id,szBuf, glm::value_ptr(color));

    //光源材质
    material* mt=getMaterial();
    //mt->glUpdateForPointLight(program_id,i);

    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_ambient,i);
    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getAmbient()));

    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_diffuse,i);
    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getDiffuse()));

    memset(szBuf,0,sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_specular,i);
    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getSpecular()));
    
    
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_constant,i);
    shaderMgr::setFloat(program_id, szBuf, m_fConstant);
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_linear,i);
    shaderMgr::setFloat(program_id, szBuf, m_fLinear);
    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_quadratic,i);
    shaderMgr::setFloat(program_id, szBuf, m_fQuadratic);
}
