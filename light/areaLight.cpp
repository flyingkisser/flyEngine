//
//  areaLight.cpp
//  flyEngine
//
//  Created by joe on 22/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#include "areaLight.h"
#include "camera.h"
#include "shaderMgr.h"
#include "material2.h"
#include "textureBuffer.h"
#include "uboMgr.h"
#include "ssboMgr.h"
#include "ltc_matrix.h"
#include "state.h"

USE_NS_FLYENGINE

//float* dataBuf,
//unsigned int width,
//unsigned int height,
//unsigned int internalFormat,
//unsigned int format,
//unsigned int dataType,
//unsigned int wrapS,
//unsigned int wrapT,
//unsigned int minFilter,
//unsigned int magFilter

areaLight::areaLight(glm::vec3 color,glm::vec3 normal,material2* mt):light(color,mt){
    _normal=normal;
    _ubo=g_ubo_id_arr[ubo_binding_light_area];
}

bool areaLight::init(){
    textureBuffer* texBuf1=new textureBuffer((float*)LTC1,64,64,GL_RGBA,GL_RGBA,GL_FLOAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST,GL_LINEAR);
    textureBuffer* texBuf2=new textureBuffer((float*)LTC2,64,64,GL_RGBA,GL_RGBA,GL_FLOAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST,GL_LINEAR);
    _texForMat1=texBuf1->getTextureID();
    _texForMat2=texBuf2->getTextureID();
    _shaderObj=shaderMgr::get3dColorShader();
    if(_shaderObj==NULL)
       return false;
    _gl_program=_shaderObj->getProgramID();
    int descArr[]={3,2,3};
    initVAO(g_verticeArrWithTexCoordAndNormal_plane_areaLight, sizeof(g_verticeArrWithTexCoordAndNormal_plane_areaLight), descArr, 3);
    return true;
}

glm::vec3 areaLight::getVerticePosByIndex(int index){
    float x=g_verticeArrWithTexCoordAndNormal_plane_areaLight[index*8];
    float y=g_verticeArrWithTexCoordAndNormal_plane_areaLight[index*8+1];
    float z=g_verticeArrWithTexCoordAndNormal_plane_areaLight[index*8+2];
    return glm::vec3(x,y,z);
}

void areaLight::rotateBy(glm::vec3 v){
    node::rotateBy(v);
    glm::mat4 m=glm::mat4(1);
    if(v.x)//沿x轴
        m=glm::rotate(m,glm::radians(v.x),glm::vec3(1,0,0));
    if(v.y)//沿y轴
        m=glm::rotate(m,glm::radians(v.y),glm::vec3(0,1,0));
    if(v.z)//沿z轴
        m=glm::rotate(m,glm::radians(v.z),glm::vec3(0,0,1));
    _normal=glm::vec3(glm::normalize(m*glm::vec4(_normal,1.0f)) );
    setDirtyPos(true);
}

void areaLight::update(int light_index){
    if(!isDirtyUBO()){
        return;
    }
    setDirtyUBO(false);
    glUpdate(light_index);
}

 void areaLight::glUpdate(int light_index){
    int sizeArr[]=  {4,4,4,4,12,12, 12,12,12,12};
    int offsetArr[]={0,4,8,12,16,32, 48,64,80,96};
    int enabled=1;
    int num=10;
    void* bufArr[]={
        &enabled,
        &_twoSided,
        &_intensity,
        &_roughness,
        glm::value_ptr(getPosition()),
        (void*)glm::value_ptr(getColor()),
        (void*)glm::value_ptr(getVerticePosByIndex(0)),
        (void*)glm::value_ptr(getVerticePosByIndex(1)),
        (void*)glm::value_ptr(getVerticePosByIndex(4)),
        (void*)glm::value_ptr(getVerticePosByIndex(5))
    };

    for(int i=0;i<num;i++){
        offsetArr[i]=offsetArr[i]+ubo_size_light_area*light_index;
    }
    uboMgr::writeData(_ubo,num,sizeArr,offsetArr,bufArr);
 }

//void areaLight::glUpdate(int light_index){
//   int sizeArr[]=  {4,4,4,4,12,12   ,12,12,12,12};
//   int offsetArr[]={0,4,8,12,16,32  ,48,64,80,96};
//   int enabled=1;
//   int num=10;
//   void* bufArr[]={
//       &enabled,
//       &_twoSided,
//       &_intensity,
//       &_roughness,
//       glm::value_ptr(getPosition()),
//       (void*)glm::value_ptr(getColor()),
//       (void*)glm::value_ptr(getVerticePosByIndex(0)),
//      (void*)glm::value_ptr(getVerticePosByIndex(1)),
//      (void*)glm::value_ptr(getVerticePosByIndex(4)),
//      (void*)glm::value_ptr(getVerticePosByIndex(5))
//   };
//   for(int i=0;i<num;i++){
//       offsetArr[i]=offsetArr[i]+ubo_size_light_area*light_index;
//   }
//   uboMgr::writeData(_ubo,num,sizeArr,offsetArr,bufArr);
//}



void areaLight::draw(){
    _shaderObj->use();
    if(m_cb_before_draw_call!=nullptr)
        m_cb_before_draw_call(_shaderObj->getProgramID());
    node::updateModel();
    node::glUpdateLight();
    _shaderObj->setVec3("color", glm::value_ptr(m_vec3Color));
    glBindVertexArray(_gl_vao);
    glDrawArrays(GL_TRIANGLES,0,6);
    state::log(6);
}
