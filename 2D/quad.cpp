//
//  quad.cpp
//  flyEngine
//
//  Created by joe on 10/10/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "quad.h"
#include "logUtil.h"
#include "state.h"
#include "textureMgr.h"
#include "shaderMgr.h"
#include "camera.h"
#include "window.h"

USE_NS_FLYENGINE

quad::quad(const char* strFileName):sprite(strFileName){
    _imgFileName=strFileName;
    init();
}

quad::quad(unsigned int texID):sprite(texID){
    _texID=texID;
    init();
}

quad::quad(unsigned int texID,int width,int height):sprite(texID){
    _texID=texID;
    _size.width=width;
    _size.height=height;
    init();
}

bool quad::init(){
    if(_imgFileName && _texID<=0){
        _texID=textureMgr::getInstance()->getTextureID(_imgFileName);
        flylog("quad:get _texID %d",_texID);
    }
    if(_texID<=0){
        flylog("quad:_texID %d<=0!return!",_texID);
        return false;
    }
    //    if(_imgFileName)
    //        _size=textureMgr::getInstance()->getTextureSize(_imgFileName);
    if(_size.width==0 || _size.height==0)
        _size=size{(float)g_winWidth,(float)g_winHigh};
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

void quad::glInit(){
    glm::vec3 s=getScale();
    size contSize=getContentSize();
    float x=1.0f*s.x*contSize.width;
    float y=1.0f*s.y*contSize.height;
    float g_verticeArrWithTexCoord_quad1[] = {
        // positions        // texture Coords
        -x,  y, 0.0f, 0.0f, 1.0f,
        -x, -y, 0.0f, 0.0f, 0.0f,
        x,  y, 0.0f, 1.0f, 1.0f,
        x, -y, 0.0f, 1.0f, 0.0f,
    };
    int descArr[]={3,2};
    initVAO(g_verticeArrWithTexCoord_quad1, sizeof(g_verticeArrWithTexCoord_quad1),descArr,2);
}

void quad::_reInitVertices(){
    glm::vec3 s=getScale();
    size contSize=getContentSize();
    float x=1.0f*s.x*contSize.width;
    float y=1.0f*s.y*contSize.height;
    float g_verticeArrWithTexCoord_quad1[] = {
        // positions        // texture Coords
        -x,  y, 0.0f, 0.0f, 1.0f,
        -x, -y, 0.0f, 0.0f, 0.0f,
        x,  y, 0.0f, 1.0f, 1.0f,
        x, -y, 0.0f, 1.0f, 0.0f,
    };
    int descArr[]={3,2};
    glBindBuffer(GL_ARRAY_BUFFER,_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(g_verticeArrWithTexCoord_quad1),g_verticeArrWithTexCoord_quad1,GL_STATIC_DRAW);
}

void quad::setContentSize(flyEngine::size s){
    sprite::setContentSize(s);
    _reInitVertices();
}

void quad::draw(){
    drawByType(GL_TRIANGLE_STRIP,4);
}

