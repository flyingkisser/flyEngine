//
//  light.cpp
//  flyEngine
//
//  Created by joe on 10/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "light.h"
#include "material.h"
#include "shader.h"
#include "shaderMgr.h"

USE_NS_FLYENGINE

light::light(glm::vec3 color,material* mt):cubeColor(color){
    setMaterial(mt);
}

light::light(glm::vec3 color):cubeColor(color){
    setMaterial(new material(glm::vec3(1,1,1),glm::vec3(1,1,1),glm::vec3(1,1,1),1.0f));
}

bool light::init(){
    return cubeColor::init();
}

void light::draw(camera* cameraObj){
    cubeColor::draw(cameraObj);
}

void light::setColor(glm::vec3 color){
    cubeColor::setColor(color);
}

//void light::glUpdateForCube(int program_id,int light_index){
//    int i=light_index;
//    char szBuf[128]={0};
//    shader* shaderObj=shaderMgr::getShader(program_id);
//    
//    //启用光源
//    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_enabled,i);
//    shaderObj->setInt(szBuf, 1);
//
//    //光源位置
//    snprintf(szBuf, sizeof(szBuf),uniform_name_light_point_pos,i);
//    shaderObj->setVec3(szBuf, glm::value_ptr(getPosition()));
//
//    //光源颜色
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_color,i);
//    glm::vec3 color=getColor();
//    shaderObj->setVec3(szBuf, glm::value_ptr(color));
//    
//    //光源材质
//    material* mt=getMaterial();
//    //mt->glUpdateForPointLight(program_id,i);
//    
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_ambient,i);
//    shaderObj->setVec3(szBuf, (float*)glm::value_ptr(mt->getAmbient()));
//    
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_diffuse,i);
//    shaderObj->setVec3(szBuf, (float*)glm::value_ptr(mt->getDiffuse()));
//
//    memset(szBuf,0,sizeof(szBuf));
//    snprintf(szBuf, sizeof(szBuf), uniform_name_light_point_specular,i);
//    shaderObj->setVec3(szBuf, (float*)glm::value_ptr(mt->getSpecular()));
//}
