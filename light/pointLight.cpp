//
//  pointLight.cpp
//  flyEngine
//
//  Created by joe on 06/09/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "pointLight.h"
#include "shaderMgr.h"
#include "material2.h"
#include "uboMgr.h"
#include "logUtil.h"

USE_NS_FLYENGINE


pointLight::pointLight(glm::vec3 color,material2* mt,float constant,float linear,float quadratic):light(color,mt){
    m_fConstant=constant;
    m_fLinear=linear;
    m_fQuadratic=quadratic;
    _ubo=g_ubo_id_arr[ubo_binding_light_point];
    if(!init())
        flylog("pointLight init failed!!");
    if(constant>0 && linear>0 && quadratic>0){
        float lightMax=std::fmaxf(std::fmaxf(color.r,color.g),color.b);
        m_fRadius=(-linear+std::sqrtf(linear*linear-4*quadratic*(constant-(256.0/5.0)*lightMax)))/(2*quadratic);
        m_fRadius*=1000;
    }
  
    // _ubo=uboMgr::createUBO(ubo_binding_light_point, ubo_size_light_point_arr,"light_point");
//    _ubo=g_ubo_id_light_point0;
}

void pointLight::update(int light_index){
    if(!isDirtyUBO()){
        return;
    }
    setDirtyUBO(false);
    glUpdate(light_index);
}

void pointLight::glUpdate(int light_index){
    int sizeArr[]={4,12,12,12,12,12,4,4,4,4};
    int offsetArr[]={0,16,32,48,64,80,92,96,100,104};
    int enabled=1;
    int num=sizeof(offsetArr)/sizeof(offsetArr[0]);
    material2* mt=getMaterial();
    glm::vec3 pos=getPosition();
    void* bufArr[]={
        &enabled,
        glm::value_ptr(pos),
        (void*)glm::value_ptr(getColor()),
        (void*)glm::value_ptr(mt->getAmbient()),
        (void*)glm::value_ptr(mt->getDiffuse()),
        (void*)glm::value_ptr(mt->getSpecular()),
        &m_fConstant,
        &m_fLinear,
        &m_fQuadratic,
        &m_fRadius
    };
    for(int i=0;i<num;i++)
        offsetArr[i]=offsetArr[i]+ubo_size_light_point*light_index;
    uboMgr::writeData(_ubo,num,sizeArr,offsetArr,bufArr);
    flylog("pointLight[%d] ubo_data updated pos %f %f %f!radius %f",light_index,pos.x,pos.y,pos.z,m_fRadius);
}


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
