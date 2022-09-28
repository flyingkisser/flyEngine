//
//  nodeIns.cpp
//  flyEngine
//
//  Created by joe on 25/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "nodeIns.h"
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

nodeIns::nodeIns(int count){
    _insCount=count;
    _posVec.assign(count, glm::vec3(0,0,0));
    _rotationVec.assign(count, glm::vec3(0,0,0));
    _scaleVec.assign(count, glm::vec3(0,0,0));
    _matModelVec.assign(count, glm::mat4(1.0f));
    _dirtyPosVec.assign(count, false);
    _dirtyUBOVec.assign(count, false);
}

void nodeIns::setCount(int count){
    _insCount=count;
}

bool nodeIns::_safeIndex(int i){
    if(i>=_posVec.size()){
        flylog("nodeIns:safeIndex give %d >=%d",i,_posVec.size());
        return false;
    }
    return true;
}
void nodeIns::moveBy(int i,glm::vec3 v){
    if(!_safeIndex(i))
        return;
    _posVec[i]+=v;
    _dirtyPosVec[i]=true;
    _dirtyUBOVec[i]=true;
    _dirtyPos=true;
}

void nodeIns::setPosition(int i,glm::vec3 v){
    if(!_safeIndex(i))
        return;
    _posVec[i]=v;
    _dirtyPosVec[i]=true;
    _dirtyUBOVec[i]=true;
    _dirtyPos=true;
}

void nodeIns::setRotation(int i, glm::vec3 v){
    if(!_safeIndex(i))
        return;
    _rotationVec[i]=v;
    _dirtyPosVec[i]=true;
    _dirtyUBOVec[i]=true;
    _dirtyPos=true;
}
void nodeIns::setScale(int i, glm::vec3 v){
    if(!_safeIndex(i))
        return;
    _scaleVec[i]=v;
    _dirtyPosVec[i]=true;
    _dirtyUBOVec[i]=true;
    _dirtyPos=true;
}

void nodeIns::rotateBy(glm::vec3 v){
    _dirtyPos=true;
    for(int i=0;i<_insCount;i++){
        _rotationVec[i]+=v;
        _dirtyPosVec[i]=true;
        _dirtyUBOVec[i]=true;
    }
}

void nodeIns::setRotation(glm::vec3 v){
    _dirtyPos=true;
    for(int i=0;i<_insCount;i++){
        _rotationVec[i]=v;
        _dirtyPosVec[i]=true;
        _dirtyUBOVec[i]=true;
    }
}

void nodeIns::setPosition(glm::vec3 v){
    _dirtyPos=true;
    for(int i=0;i<_insCount;i++){
        _posVec[i]+=v;
        _dirtyPosVec[i]=true;
        _dirtyUBOVec[i]=true;
    }
}

void nodeIns::setPositionX(float v){
    _dirtyPos=true;
    for(int i=0;i<_insCount;i++){
        _posVec[i].x+=v;
        _dirtyPosVec[i]=true;
        _dirtyUBOVec[i]=true;
    }
}
void nodeIns::setPositionY(float v){
    _dirtyPos=true;
    for(int i=0;i<_insCount;i++){
        _posVec[i].y+=v;
        _dirtyPosVec[i]=true;
        _dirtyUBOVec[i]=true;
    }
}
void nodeIns::setPositionZ(float v){
    _dirtyPos=true;
    for(int i=0;i<_insCount;i++){
        _posVec[i].z+=v;
        _dirtyPosVec[i]=true;
        _dirtyUBOVec[i]=true;
    }
}


void nodeIns::useInsByVBO(int attribIndex){
    if(_insVBO>0)
        return;
    if(_gl_vao<=0){
        flylog("nodeIns::useInsByVBO _gl_vao is not created yet!");
        return;
    }
    _insVBOIndex=attribIndex;
    glGenBuffers(1,&_insVBO);
    glBindBuffer(GL_ARRAY_BUFFER,_insVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::mat4)*_insCount,(float*)_matModelVec.data(),GL_STATIC_DRAW);
    glBindVertexArray(_gl_vao);
    for(int i=0;i<4;i++){
        glEnableVertexAttribArray(attribIndex+i);
        glVertexAttribPointer(attribIndex+i,4,GL_FLOAT,GL_FALSE,64,(void*)(i*16));
        glVertexAttribDivisor(attribIndex+i,1);
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
}
void nodeIns::updateInsVBO(){
    glBindBuffer(GL_ARRAY_BUFFER,_insVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::mat4)*_insCount,(float*)_matModelVec.data(),GL_STATIC_DRAW);
}

void nodeIns::updateModel(){
    if(!_dirtyPos)
        return;
    for(int i=0;i<_insCount;i++){
        if(!_dirtyPosVec[i])
            continue;
        glm::mat4 _matModel=glm::mat4(1.0f);
        glm::vec3 _rotate=_rotationVec[i];
        glm::vec3 _scale=_scaleVec[i];
        //移动
        _matModel=glm::translate(glm::mat4(1.0f),_posVec[i]);
       
        //旋转
        if(_rotate.x)//沿x轴
          _matModel=glm::rotate(_matModel,glm::radians(_rotate.x),glm::vec3(1,0,0));
        if(_rotate.y)//沿y轴
          _matModel=glm::rotate(_matModel,glm::radians(_rotate.y),glm::vec3(0,1,0));
        if(_rotate.z)//沿z轴
          _matModel=glm::rotate(_matModel,glm::radians(_rotate.z),glm::vec3(0,0,1));
        //缩放
        if(_scale.x || _scale.y || _scale.z)
            _matModel=glm::scale(_matModel,_scale);
        _dirtyPosVec[i]=false;
        _matModelVec[i]=_matModel;
    }
    _shaderObj->use();
    if(_insVBO>0){
        updateInsVBO();
    }
    else
        _shaderObj->setMat4Multi(uniform_name_mat_model_arr,(float*)_matModelVec.data(),_insCount,true);
}
