//
//  sprite.cpp
//  flyEngine
//
//  Created by joe on 16/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "sprite.h"
#include "logUtil.h"
#include "state.h"
#include "textureMgr.h"
#include "shaderMgr.h"
#include "camera.h"
#include "window.h"

USE_NS_FLYENGINE

sprite::sprite(const char* strFileName){
    _imgFileName=strFileName;
    init();
}

sprite::sprite(unsigned int texID){
    _texID=texID;
    init();
}

sprite::~sprite(){
    
}

bool sprite::init(){
    if(_imgFileName && _texID<=0){
        _texID=textureMgr::getInstance()->getTextureID(_imgFileName);
        flylog("sprite:get _texID %d",_texID);
    }
    if(_texID<=0){
        flylog("sprite:_texID %d<=0!return!",_texID);
        return false;
    }
    if(_imgFileName)
        _size=textureMgr::getInstance()->getTextureSize(_imgFileName);
    else
        _size=size{(float)g_winWidth,(float)g_winHigh};
    
    _shaderObj=shaderMgr::get2d1texShader();
    if(_shaderObj==NULL){
        flylog("sprite::init shaderObj is null,return!");
        return false;
    }
    _gl_program=_shaderObj->getProgramID();
    if(_gl_program<=0){
        flylog("sprite:init program id is 0,error!");
    }
    glInit();
    return true;
}

void sprite::glInit(){
    glm::vec2 s=getScale();
    float w=_size.width*s.x;
    float h=_size.height*s.y;
    float vertices[6][4]={
        {0,h,0,0},
        {0,0,0,1},
        {w,0,1,1},
        {0,h,0,0},
        {w,0,1,1},
        {w,h,1,0}
    };
    int descArr[]={2,2};
    initVAO((float*)vertices,sizeof(vertices),descArr,2);
}

void sprite::setContentSize(size s){
    _size.height=s.height;
    _size.width=s.width;
    _b_dirty_vertices=true;
}

void sprite::setScale(glm::vec3 v){
    node::setScale(v);
    _b_dirty_vertices=true;
}

void sprite::setScale(float v){
    node::setScale(v);
    _b_dirty_vertices=true;
}

void sprite::_reInitVertices(){
    glm::vec2 s=getScale();
    float h=_size.height*s.x;
    float w=_size.width*s.y;
    float vertices[6][4]={
        {0,h,0,0},
        {0,0,0,1},
        {w,0,1,1},
        {0,h,0,0},
        {w,0,1,1},
        {w,h,1,0}
    };
    glBindBuffer(GL_ARRAY_BUFFER,_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
}
void sprite::flipX(bool s){
    _bFlipX=s;
    _b_dirty_shader=true;
}
void sprite::flipY(bool s){
    _bFlipY=s;
    _b_dirty_shader=true;
}
void sprite::setReveseColor(bool s){
    _bReverseColor=s;
    _b_dirty_shader=true;
}
void sprite::setGray(bool s){
    _bGray=s;
    _b_dirty_shader=true;
}

void sprite::drawByType(int type,int verticeNum){
    _shaderObj->use();
    if(m_cb_before_draw_call!=nullptr)
        m_cb_before_draw_call(_shaderObj->getProgramID());
    _shaderObj->setInt("texture0", 0);
    if(_b_dirty_vertices){
        _reInitVertices();
        _b_dirty_vertices=false;
    }
    if(_b_dirty_shader){
        _shaderObj->setBool("bReverseColor", _bReverseColor);
        _shaderObj->setBool("bFlipX", _bFlipX);
        _shaderObj->setBool("bFlipY", _bFlipY);
        _shaderObj->setBool("bGray", _bGray);
        _b_dirty_shader=false;
    }

//    cam->update2D();
    updateModel();
    glBindVertexArray(_gl_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,_texID);
    glDrawArrays(type,0,verticeNum);
    state::log(verticeNum);
}


void sprite::draw(){
    drawByType(GL_TRIANGLES,6);
}
