//
//  cubeTexIns.cpp
//  flyEngine
//
//  Created by joe on 21/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//


#include "cubeTexIns.h"
//#include "nodeIns.h"
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

USE_NS_FLYENGINE


cubeTexIns::cubeTexIns(const char* szPath,int count):nodeIns(count){
    _texPath=szPath;
}
cubeTexIns::cubeTexIns(unsigned int texID,int count):nodeIns(count){
    _gl_texture0=texID;
}

bool cubeTexIns::init(){
    int desc[]={3,2,3};
    
    if(_texPath!=NULL){
        _texObj=textureMgr::getInstance()->getTexture(_texPath);
        if(_texObj==NULL)
            return false;
        _gl_texture0=_texObj->getTextureID();
    }
    if(_gl_texture0<=0){
        flylog("cubeTexIns::texture id is 0,return!");
        return false;
    }
    if(_shaderObj==NULL)
        _shaderObj=shaderMgr::get3d1texInsPongShader();
    if(_shaderObj==NULL){
        flylog("cubeTexIns::init shaderObj is null,return!");
        return false;
    }
    _gl_program=_shaderObj->getProgramID();
    node::initVAO(g_verticeArrWithTexCoordAndNormal,sizeof(g_verticeArrWithTexCoordAndNormal),desc,3);
    return true;
}

void cubeTexIns::useInsByVBO(){
    setShader(shaderMgr::get3d1texInsByVBOPongShader());
    nodeIns::useInsByVBO(3);
}

void cubeTexIns::drawCall(){
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,_gl_texture0);
    glBindVertexArray(_gl_vao);
    glDrawArraysInstanced(GL_TRIANGLES,0,36,_insCount);
}

void cubeTexIns::draw(){
    updateModel();
    drawCall();
    state::log(36*_insCount);
}
