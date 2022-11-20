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
#include "material2.h"
#include "uboMgr.h"
#include "ssboMgr.h"

USE_NS_FLYENGINE

spotLight::spotLight(glm::vec3 color,material2* mt,float cutOffEngleInner,float cutOffEngleOuter):light(color,mt){
    m_fcutOffInner=(float)glm::cos(glm::radians(cutOffEngleInner));
    m_fcutOffOuter=glm::cos(glm::radians(cutOffEngleOuter));
    m_fEngleOuter=cutOffEngleOuter;
    m_fEngleInner=cutOffEngleInner;
    _ubo=g_ubo_id_arr[ubo_binding_light_spot];
//     _ubo=uboMgr::createUBO(ubo_binding_light_spot, ubo_size_light_spot_arr,"light_spot");
//    _ubo=ssboMgr::createSSBO(ubo_binding_light_spot, ubo_size_light_spot_arr,"light_spot");
//    _ubo=g_ubo_id_light_spot0;
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
    if(!isDirtyUBO()){
        return;
    }
    setDirtyUBO(false);
    glUpdate(light_index);
}


 void spotLight::glUpdate(int light_index){
    int sizeArr[]={4,4,4, 12,12,12, 12,12,12, 4,4,4};
    int offsetArr[]={0,4,8,16,32,48,64,80,96,108,112,116};
    int enabled=1;
    int num=12;
    material2* mt=getMaterial();
    glm::vec3 sp=mt->getSpecular();

    void* bufArr[]={
        &enabled,
        &m_fcutOffInner,
        &m_fcutOffOuter,

        (void*)glm::value_ptr(_vec3Direction),
        glm::value_ptr(getPosition()),
        (void*)glm::value_ptr(getColor()),

        (void*)glm::value_ptr(mt->getAmbient()),
        (void*)glm::value_ptr(mt->getDiffuse()),
        (void*)glm::value_ptr(sp),

        &m_fConstant,
        &m_fLinear,
        &m_fQuadratic
    };

    for(int i=0;i<num;i++){
        // offsetArr[i]=offsetArr[i]*(light_index+1)+ubo_size_light_spot*light_index;
        offsetArr[i]=offsetArr[i]+ubo_size_light_spot*light_index;
    }

    uboMgr::writeData(_ubo, num, sizeArr,offsetArr,bufArr);
 }


// void spotLight::glUpdate(int program_id,int light_index){
//     int i=light_index;
//     char szBuf[128]={0};
//     //shader* shaderObj=shaderMgr::getShader(program_id);

//     //启用光源
//     snprintf(szBuf, sizeof(szBuf),uniform_name_light_spot_enabled,i);
//     shaderMgr::setInt(program_id,szBuf, 1);

//     //光源位置
//     snprintf(szBuf, sizeof(szBuf),uniform_name_light_spot_pos,i);
//     shaderMgr::setVec3(program_id,szBuf, glm::value_ptr(getPosition()));

//     //光源颜色
//     memset(szBuf,0,sizeof(szBuf));
//     snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_color,i);
//     glm::vec3 color=getColor();
//     shaderMgr::setVec3(program_id,szBuf, glm::value_ptr(color));

//     //光源材质
//     material* mt=getMaterial();
//     //mt->glUpdateForSpotLight(program_id,i);

//     //光源材质
//     snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_ambient,i);
//     shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getAmbient()));
//     snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_diffuse,i);
//     shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getDiffuse()));
//     snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_specular,i);
//     shaderMgr::setVec3(program_id,szBuf, (float*)glm::value_ptr(mt->getSpecular()));

//     //角度的cos值
//     snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_cutoff_inner,i);
//     shaderMgr::setFloat(program_id, szBuf, m_fcutOffInner);
//     snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_cutoff_outer,i);
//     shaderMgr::setFloat(program_id, szBuf, m_fcutOffOuter);

//     //方向向量
//     snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_direction,i);
//     shaderMgr::setVec3(program_id, szBuf,(float*)glm::value_ptr(getDirection()));

//     if(m_fConstant){
//         snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_constant,i);
//         shaderMgr::setFloat(program_id, szBuf, m_fConstant);
//         snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_linear,i);
//         shaderMgr::setFloat(program_id, szBuf, m_fLinear);
//         snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_quadratic,i);
//         shaderMgr::setFloat(program_id, szBuf, m_fQuadratic);
//     }

// }
