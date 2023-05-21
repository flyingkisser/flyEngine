//
//  quadSize.cpp
//  flyEngine
//
//  Created by Joe on 2023/3/14.
//  Copyright © 2023 joe. All rights reserved.
//

#include "quadSize.h"
#include "logUtil.h"
#include "state.h"
#include "textureMgr.h"
#include "shaderMgr.h"
#include "camera.h"
#include "window.h"

USE_NS_FLYENGINE

quadSize::quadSize(const char* strFileName):sprite(strFileName){
    _imgFileName=strFileName;
    init();
}

quadSize::quadSize(unsigned int texID,float width,float height):sprite(texID){
    _texID=texID;
    _size.width=width;
    _size.height=height;
    init();
}

bool quadSize::init(){
    if(_imgFileName && _texID<=0){
        _texID=textureMgr::getInstance()->getTextureID(_imgFileName);
        flylog("quad:get _texID %d",_texID);
    }
    if(_texID<=0){
        flylog("quad:_texID %d<=0!return!",_texID);
        return false;
    }
    if(_imgFileName)
        _size=textureMgr::getInstance()->getTextureSize(_imgFileName);
    
    _shaderObj=shaderMgr::get3dQuadShader();
    if(_shaderObj==NULL){
        flylog("quad::init shaderObj is null,return!");
        return false;
    }
    _gl_program=_shaderObj->getProgramID();
    if(_gl_program<=0){
        flylog("quad:init program id is 0,error!");
    }
    glInit();
    return true;
}

//void quadSize::glInit(){
//    int descArr[]={3,2};
//    initVAO(g_verticeArrWithTexCoord_quad, sizeof(g_verticeArrWithTexCoord_quad),descArr,2);
//}

void quadSize::glInit(){
    glm::vec3 s=getScale();
    float x=_size.width;
    float y=_size.height;
    float verticeArrWithTexCoord[] = {
        // positions        // texture Coords
        -x,  y, 0.0f, 0.0f, 1.0f,
        -x, -y, 0.0f, 0.0f, 0.0f,
        x,  y, 0.0f, 1.0f, 1.0f,
        x, -y, 0.0f, 1.0f, 0.0f,
    };
    int descArr[]={3,2};
    initVAO(verticeArrWithTexCoord, sizeof(verticeArrWithTexCoord),descArr,2);
}

void quadSize::_reInitVertices(){
    glBindBuffer(GL_ARRAY_BUFFER,_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(g_verticeArrWithTexCoord_quad),g_verticeArrWithTexCoord_quad,GL_STATIC_DRAW);
}

void quadSize::draw(){
    drawByType(GL_TRIANGLE_STRIP,4);
}
