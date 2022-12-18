//
//  semiSphere.cpp
//  flyEngine
//
//  Created by joe on 16/12/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "semiSphere.h"
#include "logUtil.h"
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "action.h"
#include "camera.h"
#include "state.h"

USE_NS_FLYENGINE

semiSphere::semiSphere(glm::vec3 color){
    m_vec3Color=color;
}

bool semiSphere::init(){
    _shaderObj=shaderMgr::get3dColorShader();
    if(_shaderObj==NULL)
       return false;
    _gl_program=_shaderObj->getProgramID();
    const float pi=3.14;
    int maxX=64;
    int maxY=64;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<int> indices;
    std::vector<float> datas;
 
    for(int x=0;x<=maxX;x++){
        for(int y=0;y<=maxY;y++){
            float xs=(float)x/(float)maxX;
            float ys=(float)y/(float)maxY;
            float posx=std::cos(xs*2*pi)*std::sin(ys*pi);
            float posy=std::cos(ys*pi);
            float posz=std::sin(xs*2*pi)*std::sin(ys*pi);
            positions.push_back(glm::vec3(posx,posy,posz));
            uvs.push_back(glm::vec2(xs,ys));
            normals.push_back(glm::vec3(posx,posy,posz));
        }
    }
    bool oddRow=false;
    for(int y=0;y<maxY;y++){
        if(!oddRow){
            for(int x=0;x<=maxX;x++){
                indices.push_back(y*(maxX+1)+x);
                indices.push_back((y+1)*(maxX+1)+x);
            }
        }else{
            for(int x=maxX;x>=0;x--){
                indices.push_back((y+1)*(maxX+1)+x);
                indices.push_back(y*(maxX+1)+x);
            }
        }
        oddRow=!oddRow;
    }
    
    for(int i=0;i<positions.size();i++){
        datas.push_back(positions[i].x);
        datas.push_back(positions[i].y);
        datas.push_back(positions[i].z);
        datas.push_back(normals[i].x);
        datas.push_back(normals[i].y);
        datas.push_back(normals[i].z);
        datas.push_back(uvs[i].x);
        datas.push_back(uvs[i].y);
    }
    _index_size=(unsigned int)indices.size();
    
    int stride=8*sizeof(float);
    glGenVertexArrays(1,&_gl_vao);
    glGenBuffers(1,&_gl_vbo);
    glGenBuffers(1,&_gl_ebo);
    glBindVertexArray(_gl_vao);
    glBindBuffer(GL_ARRAY_BUFFER,_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER,datas.size()*sizeof(float),&datas[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_gl_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride,(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,stride,(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,stride,(void*)(6*sizeof(float)));
 
    return true;
}


void semiSphere::draw(){
    _shaderObj->use();
    if(m_cb_before_draw_call!=nullptr)
        m_cb_before_draw_call(_shaderObj->getProgramID());
    node::updateModel();
    node::glUpdateLight();
    _shaderObj->setVec3("color", glm::value_ptr(m_vec3Color));

    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(_gl_vao);
    glDrawElements(GL_TRIANGLE_STRIP,_index_size,GL_UNSIGNED_INT,0);
    state::log(_index_size);
}

