//
//  quadColor.cpp
//  flyEngine
//
//  Created by Joe on 2023/1/8.
//  Copyright © 2023 joe. All rights reserved.
//

#include "quadColor.h"
#include "logUtil.h"
#include "state.h"
#include "textureMgr.h"
#include "shaderMgr.h"
#include "camera.h"
#include "window.h"

USE_NS_FLYENGINE



quadColor::quadColor(glm::vec4 color,float width,float height):sprite(1){
    _color=color;
    _size.width=width;
    _size.height=height;
    init();
}

bool quadColor::init(){
    _shaderObj=shaderMgr::get3dQuadColorShader();
    if(_shaderObj==NULL){
        flylog("quadColor::init shaderObj is null,return!");
        return false;
    }
    _gl_program=_shaderObj->getProgramID();
    if(_gl_program<=0){
        flylog("quadColor:init program id is 0,error!");
    }
    glInit();
    return true;
}

void quadColor::glInit(){
    int descArr[]={3,2};
    initVAO(g_verticeArrWithTexCoord_quadColor, sizeof(g_verticeArrWithTexCoord_quadColor),descArr,2);
    _shaderObj->use();
    _shaderObj->setVec4("color", _color,true);
}


void quadColor::_reInitVertices(){
    glBindBuffer(GL_ARRAY_BUFFER,_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(g_verticeArrWithTexCoord_quadColor),g_verticeArrWithTexCoord_quadColor,GL_STATIC_DRAW);
}

void quadColor::draw(){
    _shaderObj->use();
    _shaderObj->setVec4("color", _color);
    drawByType(GL_TRIANGLES, 6);
}
