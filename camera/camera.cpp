//
//  camera.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/7/4.
//  Copyright © 2021 joe. All rights reserved.
//

#include "camera.h"
#include "flyEngine.h"

flyEngine::camera::camera(){
    init();
}

void flyEngine::camera::_updateCamera(){
    if(!_program)
        return;
    if(!_dirtyPos)
        return;
    _dirtyPos=false;
    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
    glUniformMatrix4fv(glGetUniformLocation(_program, "matCamera"), 1,GL_FALSE,glm::value_ptr(_matCamera));
}

void flyEngine::camera::_updateProjection(){
    if(!_program)
        return;
    if(!_dirtyProj)
           return;
    _dirtyProj=false;
    _matProj=glm::perspective(glm::radians((double)_fov),(double)_screenRatio, 0.1, 100.0);
    glUniformMatrix4fv(glGetUniformLocation(_program, "matProj"), 1,GL_FALSE,glm::value_ptr(_matProj));
}


void flyEngine::camera::reset(){
     glUniformMatrix4fv(glGetUniformLocation(_program, "matCamera"), 1,GL_FALSE,glm::value_ptr(_matCameraOrigin));
}

void flyEngine::camera::setPosition(glm::vec3 pos){
    _cameraPos.x=pos.x;
    _cameraPos.y=pos.y;
    _cameraPos.z=pos.z;
    _dirtyPos=true;
}
void flyEngine::camera::setPositionX(float v){
    flylog("camera x:%f->%f",_cameraPos.x,v);
    _cameraPos.x=v;
    _dirtyPos=true;
}
void flyEngine::camera::setPositionY(float v){
    flylog("camera y:%f->%f",_cameraPos.y,v);
    _cameraPos.y=v;
    _dirtyPos=true;
}
void flyEngine::camera::setPositionZ(float v){
    flylog("camera z:%f->%f",_cameraPos.z,v);
    _cameraPos.z=v;
    _dirtyPos=true;
}

void flyEngine::camera::setPositionFront(glm::vec3 pos){
    _cameraFront.x=pos.x;
    _cameraFront.y=pos.y;
    _cameraFront.z=pos.z;
    _dirtyPos=true;
}


void flyEngine::camera::moveBy(glm::vec3 v){
    _cameraPos.x+=v.x;
    _cameraPos.y+=v.y;
    _cameraPos.z+=v.z;
    _dirtyPos=true;
}
void flyEngine::camera::rotate(glm::vec3 v){
    if(v.x)
        _matCamera=glm::rotate(_matCamera,v.x,glm::vec3(1,0,0));
    if(v.y)
        _matCamera=glm::rotate(_matCamera,v.y,glm::vec3(0,1,0));
    if(v.z)
        _matCamera=glm::rotate(_matCamera,v.z,glm::vec3(0,0,1));
    _dirtyPos=true;
}

void flyEngine::camera::enableControl(){
    _controlObj=new flyEngine::control();
    _controlObj->bindCamera(this);
}

control* flyEngine::camera::getControl(){
    return _controlObj;
}


void flyEngine::camera::print(){
    flylog("camera pos is %f %f %f",_cameraPos.x,_cameraPos.y,_cameraPos.z);
}


bool flyEngine::camera::init(){
    _yaw=-90;
    _pitch=0;
    _fov=30.0;
    _fovOrigin=_fov;
    _screenRatio=800/600;
    
    _cameraPos=glm::vec3(0,0,3);
    _cameraFront.x=cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
    _cameraFront.y=sin(glm::radians(_pitch));
    _cameraFront.z=sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
    _cameraFront=glm::normalize(_cameraFront);
    _cameraUp=glm::vec3(0,1,0);
    
    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
    _matCameraOrigin=_matCamera;
    
    _matProj=glm::perspective(glm::radians(double(_fov)), (double)_screenRatio, 0.1, 100.0);
    _matProjOrigin=_matProj;
    
    _dirtyPos=true;
    _dirtyProj=true;
    return true;
}


void flyEngine::camera::update(int programID){
    _program=programID;
    _updateCamera();
    _updateProjection();
}

