//
//  material.cpp
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "material.h"
#include "texture.h"
#include "logUtil.h"
#include "shader.h"
#include "shaderMgr.h"

USE_NS_FLYENGINE;

material::material(glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,float shininess){
    m_vec3Ambient=ambient;
    m_vec3Diffuse=diffuse;
    m_vec3Specular=specular;
    m_fShininess=shininess;
    m_texSpecular=NULL;
}

bool material::setDiffuseTex(const char* szTex){
    texture* m_texDiffuse=new texture(szTex);
    if(!m_texDiffuse->init())
        return false;
    m_texDiffuse->glInit();
    return true;
}

bool material::setSpecularTex(const char* szTex){
    m_texSpecular=new texture(szTex);
    if(!m_texSpecular->init())
        return false;
    m_texSpecular->glInit(GL_TEXTURE1);
    return true;
}

//void material::glUpdateForCube(int gl_program){
//    //环境光材质
//    shader* sharderObj=shaderMgr::getShader(gl_program);
//    sharderObj->setVec3(uniform_name_material_ambient, glm::value_ptr(m_vec3Ambient));
//    //漫反射材质
//    sharderObj->setVec3(uniform_name_material_diffuse, glm::value_ptr(m_vec3Diffuse));
//    //镜面反射材质
//    sharderObj->setVec3(uniform_name_material_specular, glm::value_ptr(m_vec3Specular),false);
//    //镜面光滑系数
//    sharderObj->setFloat(uniform_name_material_shininess, m_fShininess);
//    //高亮贴图
//    if(m_texSpecular!=NULL){
////       sharderObj->setInt(uniform_name_material_specular_tex, m_texSpecular->getTextureID(),false);
//        sharderObj->setInt(uniform_name_material_specular_tex, 1,false);
//    }
//}

//i:light index from 0
//void material::glUpdateForPointLight(int gl_program,int i){
//    char szBuf[128]={0};
//    shader* shaderObj=shaderMgr::getShader(gl_program);
//    //光源材质
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_ambient,i);
//    shaderObj->setVec3(szBuf, glm::value_ptr(m_vec3Ambient));
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_diffuse,i);
//    shaderObj->setVec3(szBuf, glm::value_ptr(m_vec3Diffuse));
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_specular,i);
//    shaderObj->setVec3(szBuf, glm::value_ptr(m_vec3Specular));
//}

//void material::glUpdateForSpotLight(int gl_program,int i){
//    char szBuf[128]={0};
//    shader* shaderObj=shaderMgr::getShader(gl_program);
//    //光源材质
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_ambient,i);
//    shaderObj->setVec3(szBuf, glm::value_ptr(m_vec3Ambient));
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_diffuse,i);
//    shaderObj->setVec3(szBuf, glm::value_ptr(m_vec3Diffuse));
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_spot_specular,i);
//    shaderObj->setVec3(szBuf, glm::value_ptr(m_vec3Specular));
//}
