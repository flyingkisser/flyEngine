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
#include "uboMgr.h"

USE_NS_FLYENGINE

spotLight::spotLight(glm::vec3 color,material* mt,float cutOffEngleInner,float cutOffEngleOuter):light(color,mt){
    m_fcutOffInner=glm::cos(glm::radians(cutOffEngleInner));
    m_fcutOffOuter=glm::cos(glm::radians(cutOffEngleOuter));
    m_fEngleOuter=cutOffEngleOuter;
    m_fEngleInner=cutOffEngleInner;
    _ubo=uboMgr::createUBO(ubo_binding_light_spot, ubo_size_light_spot_arr);
}

void spotLight::rotateBy(glm::vec3 v){
    node::rotateBy(v);
    //旋转
    glm::vec3 pos=getPosition();
    glm::vec3 dir=getDirection();
    glm::mat4 m=glm::mat4(1);
//    m=glm::translate(m, pos);
    if(v.x)//沿x轴
        m=glm::rotate(m,glm::radians(v.x),glm::vec3(1,0,0));
    if(v.y)//沿y轴
        m=glm::rotate(m,glm::radians(v.y),glm::vec3(0,1,0));
    if(v.z)//沿z轴
        m=glm::rotate(m,glm::radians(v.z),glm::vec3(0,0,1));
//    m=glm::translate(m, -pos);
    
    _vec3Direction=glm::vec3(glm::normalize(m*glm::vec4(dir,1.0f)) );
    
    setDirtyPos(true);
}

void spotLight::update(int light_index){
    if(!isDirtyMT() && !isDirtyColor() && !isDirtyPos())
        return;
    //有设置过materail或color
    setDirtyMT(false);
    setDirtyColor(false);
    //    setDirtyPos(false);   //位置的变化，由light->cubeColor的updateModel来更新dirty状态，这里不能更新
    glUpdate(light_index);
}

void spotLight::glUpdate(int light_index){
    int sizeArr[]={4,12,12,12,12,12,4,4,4,4,4,12};
    int offsetArr[]={0,16,32,48,64,80,92,96,100,104,108,112};
    int enabled=1;
    int num=12;
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
        &m_fQuadratic,
        (void*)&m_fcutOffInner, //无法写入
        (void*)&m_fcutOffOuter,//无法写入
        (void*)glm::value_ptr(_vec3Direction)//无法写入
    };
    
    for(int i=0;i<num;i++){
        offsetArr[i]=offsetArr[i]*(light_index+1)+ubo_size_light_spot*light_index;
    }
    uboMgr::writeData(_ubo, num, sizeArr,offsetArr,bufArr);
}

//void spotLight::glUpdateForCube(int program_id,int light_index,camera* c){
//    int i=light_index;
//    char szBuf[128]={0};
//    //shader* shaderObj=shaderMgr::getShader(program_id);
//
//    //启用光源
//    snprintf(szBuf, sizeof(szBuf),uniform_name_light_spot_enabled,i);
//    shaderMgr::setInt(program_id,szBuf, 1);
//
//    //光源位置
//    snprintf(szBuf, sizeof(szBuf),uniform_name_light_spot_pos,i);
//    shaderMgr::setVec3(program_id,szBuf, glm::value_ptr(getPosition()));
//
//    //光源颜色
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_color,i);
//    glm::vec3 color=getColor();
//    shaderMgr::setVec3(program_id,szBuf, glm::value_ptr(color));
//
//    //光源材质
//    material* mt=getMaterial();
//    //mt->glUpdateForSpotLight(program_id,i);
//
//    //光源材质
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_ambient,i);
//    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getAmbient()));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_diffuse,i);
//    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getDiffuse()));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_specular,i);
//    shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getSpecular()));
//
//    //角度的cos值
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_cutoff_inner,i);
//    shaderMgr::setFloat(program_id, szBuf, m_fcutOffInner);
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_cutoff_outer,i);
//    shaderMgr::setFloat(program_id, szBuf, m_fcutOffOuter);
//
//    //方向向量
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_direction,i);
//    shaderMgr::setVec3(program_id, szBuf,(float*)glm::value_ptr(c->getFront()));
//
//    if(m_fConstant){
//        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_constant,i);
//        shaderMgr::setFloat(program_id, szBuf, m_fConstant);
//        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_linear,i);
//        shaderMgr::setFloat(program_id, szBuf, m_fLinear);
//        snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_quadratic,i);
//        shaderMgr::setFloat(program_id, szBuf, m_fQuadratic);
//    }
//
//}
