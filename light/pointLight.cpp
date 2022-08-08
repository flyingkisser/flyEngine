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
#include "uboMgr.h"

USE_NS_FLYENGINE


pointLight::pointLight(glm::vec3 color,material* mt,float constant,float linear,float quadratic):light(color,mt){
    m_fConstant=constant;
    m_fLinear=linear;
    m_fQuadratic=quadratic;
    _ubo=uboMgr::createUBO(ubo_binding_light_point, ubo_size_light_point_arr);
}

void pointLight::update(int light_index){
    if(!isDirtyMT() && !isDirtyColor() && !isDirtyPos())
        return;
    //有设置过materail或color
    setDirtyMT(false);
    setDirtyColor(false);
//    setDirtyPos(false);   //位置的变化，由light->cubeColor的updateModel来更新dirty状态，这里不能更新
    glUpdate(light_index);
}

void pointLight::glUpdate(int light_index){
    int sizeArr[]={4,12,12,12,12,12,4,4,4};
    int offsetArr[]={0,16,32,48,64,80,92,96,100};
    int enabled=1;
    int num=9;
    material* mt=getMaterial();
    void* bufArr[]={
        &enabled,
        glm::value_ptr(getPosition()),
        (void*)glm::value_ptr(getColor()),
        (void*)glm::value_ptr(mt->getAmbient()),
        (void*)glm::value_ptr(mt->getDiffuse()),
        (void*)glm::value_ptr(mt->getSpecular()),
        &m_fConstant,
        &m_fLinear,
        &m_fQuadratic
    };
   
    for(int i=0;i<num;i++){
        offsetArr[i]=offsetArr[i]*(light_index+1)+ubo_size_light_point*light_index;
    }
    uboMgr::writeData(_ubo, num, sizeArr,offsetArr,bufArr);
}


//pointLight::pointLight(glm::vec3 color,glm::vec3 pos,float am,float diff,float spe,int shiness):light(color,nullptr){
//    m_fConstant=0;
//    m_fLinear=0;
//    m_fQuadratic=0;
//
//    m_fAmbient=am;
//    m_fDiffuse=diff;
//    m_fSpecular=spe;
//    m_IntShiness=shiness;
//    setPosition(pos);
//}

//支持多个点光源,light_index为点光源序号，从0开始
//void pointLight::glUpdateUseMaterail(int program_id, int light_index){
//    int i=light_index;
//    char szBuf[128]={0};
//    //light::glUpdateForCube(program_id,i);
//
//    //启用光源
//    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_enabled,i);
//    shaderMgr::setInt(program_id, szBuf, 1);
//
//    //光源位置
//    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_pos,i);
//    shaderMgr::setVec3(program_id,szBuf, glm::value_ptr(getPosition()));
//
//    //光源颜色
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_color,i);
//    glm::vec3 color=getColor();
//    shaderMgr::setVec3(program_id,szBuf, glm::value_ptr(color));
//
//    //光源材质
//    material* mt=getMaterial();
//    //mt->glUpdateForPointLight(program_id,i);
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_ambient,i);
//    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getAmbient()));
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_diffuse,i);
//    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getDiffuse()));
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_specular,i);
//    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getSpecular()));
//
//
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_constant,i);
//    shaderMgr::setFloat(program_id, szBuf, m_fConstant);
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_linear,i);
//    shaderMgr::setFloat(program_id, szBuf, m_fLinear);
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_quadratic,i);
//    shaderMgr::setFloat(program_id, szBuf, m_fQuadratic);
//}


//void pointLight::glUpdate(int program_id, int light_index){
//    int i=light_index;
//    char szBuf[128]={0};
//
//    //启用光源
//    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_enabled,i);
//    shaderMgr::setInt(program_id, szBuf, 1);
//
//    //光源位置
//    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_pos,i);
//    shaderMgr::setVec3(program_id,szBuf,glm::value_ptr(getPosition()));
//
//    //光源颜色
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_color,i);
//    glm::vec3 color=getColor();
//    shaderMgr::setVec3(program_id,szBuf,glm::value_ptr(color));
//
//    //Phong光照模型参数
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_ambient,i);
//    shaderMgr::setFloat(program_id,szBuf, m_fAmbient);
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_diffuse,i);
//    shaderMgr::setFloat(program_id,szBuf, m_fDiffuse);
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_specular,i);
//    shaderMgr::setFloat(program_id,szBuf, m_fSpecular);
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_shiness,i);
//    shaderMgr::setInt(program_id,szBuf, m_IntShiness);
//
//    if(m_fConstant){
//        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_constant,i);
//        shaderMgr::setFloat(program_id, szBuf, m_fConstant);
//    }
//    if(m_fLinear){
//        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_linear,i);
//        shaderMgr::setFloat(program_id, szBuf, m_fLinear);
//    }
//    if(m_fQuadratic){
//        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_quadratic,i);
//        shaderMgr::setFloat(program_id, szBuf, m_fQuadratic);
//    }
//}
