//
//  heightMapPatch.cpp
//  flyEngine
//
//  Created by joe on 12/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#include "heightMapPatch.h"
#include "logUtil.h"
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "action.h"
#include "camera.h"
#include "world.h"
#include "directionLight.h"
#include "material2.h"
#include "state.h"
#include "texture2.h"
#include "stb_image.h"

USE_NS_FLYENGINE

heightMapPatch::heightMapPatch(const char* texPath){
    _texPath=texPath;
}

void heightMapPatch::resetPos(){
    setPosition(glm::vec3(0,0,0));
    rotateBy(glm::vec3(-30,0,-30));
}

bool heightMapPatch::init(){
    _texObj=new texture2(_texPath,true);
    if(!_texObj->init()){
        flylog("heightMapPatch::init texture2 failed,return!");
        return false;
    }
    _texObj->glInitWithParam(0,GL_REPEAT,GL_REPEAT,GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR,GL_UNSIGNED_BYTE,true);
  
    _shaderObj=shaderMgr::getHeightGpuShader();
    if(_shaderObj==NULL){
        flylog("heightMapPatch::init shaderObj is null,return!");
        return false;
    }
    _gl_program=_shaderObj->getProgramID();
    
    _shaderObj->use();
   

    int height=_texObj->getHeight();
    int width=_texObj->getWidth();
   
    _rez=20;
    for(int i=0;i<=_rez-1;i++){
        for(int j=0;j<=_rez-1;j++){
            _vertices.push_back(-width/2.0f+width*i/(float)_rez); // v.x
            _vertices.push_back(0.0f); // v.y
            _vertices.push_back(-height/2.0f+height*j/(float)_rez); // v.z
            _vertices.push_back(i/(float)_rez); // u
            _vertices.push_back(j/(float)_rez); // v

            _vertices.push_back(-width/2.0f+width*(i+1)/(float)_rez); // v.x
            _vertices.push_back(0.0f); // v.y
            _vertices.push_back(-height/2.0f+height*j/(float)_rez); // v.z
            _vertices.push_back((i+1)/(float)_rez); // u
            _vertices.push_back(j/(float)_rez); // v

            _vertices.push_back(-width/2.0f+width*i/(float)_rez); // v.x
            _vertices.push_back(0.0f); // v.y
            _vertices.push_back(-height/2.0f+height*(j+1)/(float)_rez); // v.z
            _vertices.push_back(i/(float)_rez); // u
            _vertices.push_back((j+1)/(float)_rez); // v

            _vertices.push_back(-width/2.0f+width*(i+1)/(float)_rez); // v.x
            _vertices.push_back(0.0f); // v.y
            _vertices.push_back(-height/2.0f+height*(j+1)/(float)_rez); // v.z
            _vertices.push_back((i+1)/(float)_rez); // u
            _vertices.push_back((j+1)/(float)_rez); // v
        }
    }

    glGenVertexArrays(1,&_gl_vao);
    glBindVertexArray(_gl_vao);
    glGenBuffers(1,&_gl_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER,_vertices.size()*sizeof(float),&_vertices[0],GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    return true;
}

void heightMapPatch::setPipelineValue(){
    _shaderObj->use();
    camera* cam=world::getInstance()->getCamera();
    _shaderObj->setInt("heightMap", 0,true);
    _shaderObj->setMat4("proj", cam->getPerspectiveMatrix());
    _shaderObj->setMat4("view", cam->getLookAtMatrix());
    if(m_cb_before_draw_call!=nullptr)
        m_cb_before_draw_call(_shaderObj->getProgramID());
    
    node::updateModel();
    node::glUpdateLight();
}

void heightMapPatch::drawCall(){
    if(_bLineMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(_gl_vao);
    glDrawArrays(GL_PATCHES,0,4*_rez*_rez);
}

void heightMapPatch::draw(){
    setPipelineValue();
    drawCall();
    state::log(4*_rez*_rez);
}
