//
//  animator.cpp
//  flyEngine
//
//  Created by Joe on 2023/1/30.
//  Copyright © 2023 joe. All rights reserved.
//

#include "animator.h"
#include "bone.h"
#include "timeUtil.h"
#include "shaderMgr.h"

USE_NS_FLYENGINE

animator::animator(animation* aniObj){
    _currentTime=0.0;
    _lastTime=0;
    _delteTime=0;
    _currentAnimation=aniObj;
    _finalBoneMatrices.reserve(100);
    for(int i=0;i<100;i++)
        _finalBoneMatrices.push_back(glm::mat4(1.0));
    _shaderObj=shaderMgr::getModelAniShader();
    setShader(_shaderObj);
    aniObj->getModel()->setShader(_shaderObj);
}

void animator::updateAnimation(float dt){
    _delteTime=dt;
    if(_currentAnimation==nullptr)
        return;
    _currentTime+=_currentAnimation->getTicksPersecond()*dt;
    _currentTime=fmod(_currentTime,_currentAnimation->getDuration());
    calculateBoneTransform(&_currentAnimation->getRootNode(),glm::mat4(1.0));
}

void animator::playAnimation(animation* aniObj){
    _currentAnimation=aniObj;
    _currentTime=0.0;
}

void animator::calculateBoneTransform(AssimpNodeData* nodeData,glm::mat4 parentTransform){
    std::string nodeName=nodeData->name;
    glm::mat4 nodeTransformation=nodeData->transformation;
    bone* boneObj=_currentAnimation->findBone(nodeName);
    if(boneObj){
        boneObj->update(_currentTime);
        nodeTransformation=boneObj->getLocalTransform();
    }
    glm::mat4 globalTransformation=parentTransform*nodeTransformation;
    auto boneInfoMap=_currentAnimation->getBoneInfoMap();
    if(boneInfoMap.find(nodeName)!=boneInfoMap.end()){
        int index=boneInfoMap[nodeName].id;
        glm::mat4 offset=boneInfoMap[nodeName].offset;
        _finalBoneMatrices[index]=globalTransformation*offset;
    }
    for(int i=0;i<nodeData->childrenCount;i++)
        calculateBoneTransform(&nodeData->childrens[i], globalTransformation);
}

bool animator::init(){
    return true;
}

void animator::draw(){
    _currentTime=timeUtil::getTime();
    if(_lastTime==0)
        _lastTime=_currentTime;
    _delteTime=_currentTime-_lastTime;
    _lastTime=_currentTime;
    _shaderObj->use();
    updateAnimation(_delteTime);
    auto transforms=getFinalBoneMatries();
    for(int i=0;i<transforms.size();i++){
        _shaderObj->setMat4("finalBoneMatrices["+std::to_string(i)+"]", transforms[i],true);
    }
    _currentAnimation->getModel()->draw();
}
