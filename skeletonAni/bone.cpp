//
//  bone.cpp
//  flyEngine
//
//  Created by joe on 27/01/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#include "bone.h"
#include "mathUtil.h"
#include "logUtil.h"

USE_NS_FLYENGINE

//从aiNodeAnim对象里,读取关键帧信息(位移，旋转，缩放),分别存入相应的vector中
bone::bone(std::string name,int id,aiNodeAnim* channel):_name(name),_id(id),_localTransform(1.0){
    _numPositions=channel->mNumPositionKeys;
    _numRotations=channel->mNumRotationKeys;
    _numScales=channel->mNumScalingKeys;
    for(int i=0;i<_numPositions;i++){
        KeyPosition data;
        data.position=mathUtil::getGLMVec(channel->mPositionKeys[i].mValue);
        data.timeStamp=channel->mPositionKeys[i].mTime;
        _positions.push_back(data);
    }
    for(int i=0;i<_numRotations;i++){
        KeyRotation data;
        data.orientation=mathUtil::getGLMQuat(channel->mRotationKeys[i].mValue);
        data.timeStamp=channel->mPositionKeys[i].mTime;
        _rotations.push_back(data);
    }
    for(int i=0;i<_numScales;i++){
        KeyScale data;
        data.scale=mathUtil::getGLMVec(channel->mScalingKeys[i].mValue);
        data.timeStamp=channel->mPositionKeys[i].mTime;
        _scales.push_back(data);
    }
}

void bone::update(float animationTime){
    _localTransform=_interpolatePosition(animationTime)*_interpolateRotation(animationTime)*_interpolateScaling(animationTime);
}

int bone::getPositionIndex(float animationTime){
    for(int i=0;i<_numPositions-1;i++){
        if(animationTime<_positions[i+1].timeStamp)
            return i;
    }
    return -1;
}

int bone::getRotationIndex(float animationTime){
    for(int i=0;i<_numRotations-1;i++){
        if(animationTime<_rotations[i+1].timeStamp)
            return i;
    }
    return -1;
}

int bone::getScaleIndex(float animationTime){
    for(int i=0;i<_numScales-1;i++){
        if(animationTime<_scales[i+1].timeStamp)
            return i;
    }
    return -1;
}

float bone::_getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime){
    float midWayLength=animationTime-lastTimeStamp;
    float frameDiff=nextTimeStamp-lastTimeStamp;
    return midWayLength/frameDiff;
}

glm::mat4 bone::_interpolatePosition(float animationTime){
    if(_numPositions==1)
        return glm::translate(glm::mat4(1.0), _positions[0].position);
    int index0=getPositionIndex(animationTime);
    if(index0==-1){
        flylog("bone:_interpolatePosition aniTime %f is beyond this animation,do nothing!",animationTime);
        return glm::mat4(1.0);
    }
    float scale=_getScaleFactor(_positions[index0].timeStamp,_positions[index0+1].timeStamp,animationTime);
    glm::vec3 v=glm::mix(_positions[index0].position,_positions[index0+1].position,scale);
    return glm::translate(glm::mat4(1.0),v);
}

glm::mat4 bone::_interpolateRotation(float animationTime){
    if(_numRotations==1)
        return glm::toMat4(glm::normalize(_rotations[0].orientation));
    int index0=getRotationIndex(animationTime);
    if(index0==-1){
        flylog("bone:_interpolateRotation aniTime %f is beyond this animation,do nothing!",animationTime);
        return glm::mat4(1.0);
    }
    float scale=_getScaleFactor(_rotations[index0].timeStamp,_rotations[index0+1].timeStamp,animationTime);
    glm::quat v=glm::slerp(_rotations[index0].orientation,_rotations[index0+1].orientation,scale);
    return glm::toMat4(glm::normalize(v));
}

glm::mat4 bone::_interpolateScaling(float animationTime){
    if(_numScales==1)
        return glm::scale(glm::mat4(1.0), _scales[0].scale);
    int index0=getScaleIndex(animationTime);
    if(index0==-1){
        flylog("bone:_interpolateScaling aniTime %f is beyond this animation,do nothing!",animationTime);
        return glm::mat4(1.0);
    }
    float scale=_getScaleFactor(_scales[index0].timeStamp,_scales[index0+1].timeStamp,animationTime);
    glm::vec3 v=glm::mix(_scales[index0].scale,_scales[index0+1].scale,scale);
    return glm::scale(glm::mat4(1.0),v);
}
