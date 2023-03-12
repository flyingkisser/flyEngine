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
    _texObj=textureMgr::getInstance()->getTexture(_texPath,true);
    if(_texObj==NULL)
        return false;
    if(_shaderObj==NULL)
        _shaderObj=shaderMgr::getHeightCpuShader();
    if(_shaderObj==NULL){
        flylog("heightMap::init shaderObj is null,return!");
        return false;
    }
    _gl_program=_shaderObj->getProgramID();

    float yScale=64.0/256.0;
    float yShift=16.0;
    int height=_texObj->getHeight();
    int width=_texObj->getWidth();
    int channels=_texObj->getChannels();
    unsigned char* buf=_texObj->getBuf();
    
   
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
    
    for(int i=0;i<height-1;i++){
        for(int j=0;j<width;j++){
            for(int k=0;k<2;k++){
                _indices.push_back(j+width*(i+k));
            }
        }
    }
    
    num_strips=height-1;
    num_verts_per_strip=width*2;

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    
    glGenVertexArrays(1,&_gl_vao);
    glBindVertexArray(_gl_vao);
    glGenBuffers(1,&_gl_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER,_vertices.size()*sizeof(float),&_vertices[0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&_gl_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_gl_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,_indices.size()*sizeof(int),&_indices[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    return true;
}


void heightMapPatch::setPipelineValue(){
    _shaderObj->use();
    
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
    for(int i=0;i<num_strips;i++){
        glDrawArrays(GL_PATCHES,0,4*_rez*_rez);
    }
}

void heightMapPatch::draw(){
    setPipelineValue();
    drawCall();
    state::log(num_strips*num_verts_per_strip);
}
