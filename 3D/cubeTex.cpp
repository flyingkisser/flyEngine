//
//  cubeTex.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/8/1.
//  Copyright © 2021 joe. All rights reserved.
//

#include "cubeTex.h"

#include "logUtil.h"
#include "texture.h"
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "action.h"
#include "camera.h"
#include "world.h"
#include "directionLight.h"
#include "material.h"
#include "state.h"

USE_NS_FLYENGINE

cubeTex::cubeTex(const char* texPath){
    _texPath=texPath;
}

cubeTex::cubeTex(unsigned int texID){
    _gl_texture0=texID;
}

void cubeTex::resetPos(){
    setPosition(glm::vec3(0,0,0));
    rotateBy(glm::vec3(-30,0,-30));
}

bool cubeTex::init(){
    int desc[]={3,2};
    return initByVerticeArr(g_verticeArrWithTexCoord,sizeof(g_verticeArrWithTexCoord),desc,2);
}

void cubeTex::glInit(){

}

bool cubeTex::initByVerticeArr(float* arr,int arrSize,int descArr[],int descArrNum){
    if(_texPath!=NULL){
        _texObj=textureMgr::getInstance()->getTexture(_texPath);
        if(_texObj==NULL)
            return false;
        _gl_texture0=_texObj->getTextureID();
    }
    if(_gl_texture0<=0){
        flylog("cubeTex::texture id is 0,return!");
        return false;
    }
    if(_shaderObj==NULL)
        _shaderObj=shaderMgr::get3d1texPongShader();    
    if(_shaderObj==NULL){
        flylog("cubeTex::init shaderObj is null,return!");
        return false;
    }
    _gl_program=_shaderObj->getProgramID();
    node::initVAO(arr,arrSize,descArr,descArrNum);
    return true;
}

cubeTex* cubeTex::clone(){
    cubeTex* n=new cubeTex(_texPath);
    n->initVAO(_vertice_arr,_vertice_arr_size,_desc_arr,_desc_arr_size);
    n->setShader(getShader());
    n->setPosition(getPosition());
    n->setScale(getScale());
    n->setRotation(getRotation());
    n->setMaterial(getMaterial());
    return n;
};

void cubeTex::draw(camera* cameraObj){
    _shaderObj->use();
    if(m_cb_before_draw_call!=nullptr)
        m_cb_before_draw_call(_shaderObj->getProgramID());
    
    node::updateModel();
    node::glUpdateLight();
    if(m_material!=NULL){
        _shaderObj->setBool(uniform_name_material_enabled, true);
         //环境光材质
        _shaderObj->setVec3(uniform_name_material_ambient, (float*)glm::value_ptr(m_material->getAmbient()));
        //漫反射材质
        _shaderObj->setVec3(uniform_name_material_diffuse, (float*)glm::value_ptr(m_material->getDiffuse()));
        //镜面反射材质
        _shaderObj->setVec3(uniform_name_material_specular, (float*)glm::value_ptr(m_material->getSpecular()),false);
        //镜面光滑系数
        _shaderObj->setFloat(uniform_name_material_shininess, m_material->getShininess());
        //高亮贴图
        texture* texSpecular=m_material->getTexSpecular();
       
        if(texSpecular!=NULL){
            //sharderObj->setInt(uniform_name_material_specular_tex, m_texSpecular->getTextureID(),false);
            _shaderObj->setInt(uniform_name_material_specular_tex, 1,false);
        }
        //m_material->glUpdateForCube(_gl_program);
    }else
         _shaderObj->setBool(uniform_name_material_enabled, false);
        
    _shaderObj->setInt("texture0", 0);
    
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,_gl_texture0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP,_gl_texture0);
    glBindVertexArray(_gl_vao);
    glDrawArrays(GL_TRIANGLES,0,36);
    state::log(36);
}
