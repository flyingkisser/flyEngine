//
//  camera.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/7/4.
//  Copyright © 2021 joe. All rights reserved.
//

#include "camera.h"
#include "logUtil.h"
#include "control.h"
#include "shader.h"
#include "shaderMgr.h"

using namespace flyEngine;

camera::camera(){
    init();
}

void camera::_updateCamera(){
    if(!_program)
        return;
    if(_dirtyPos){
        _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
        _dirtyPos=false;
    }
    shaderMgr::setMat4(_program,uniform_name_mat_camera,glm::value_ptr(_matCamera));
    shaderMgr::setVec3(_program,uniform_name_camera_pos,glm::value_ptr(_cameraPos));
}

void camera::_updateProjection(){
    if(!_program)
        return;
    shaderMgr::setMat4(_program,uniform_name_mat_proj,glm::value_ptr(_matProj));
}


void camera::reset(){
    shaderMgr::setMat4(_program,uniform_name_mat_camera,glm::value_ptr(_matCameraOrigin));
}

void camera::setPosition(glm::vec3 pos){
    _cameraPos.x=pos.x;
    _cameraPos.y=pos.y;
    _cameraPos.z=pos.z;
    _dirtyPos=true;
}
void camera::setPositionX(float v){
    flylog("camera x:%f->%f",_cameraPos.x,v);
    _cameraPos.x=v;
    _dirtyPos=true;
}
void camera::setPositionY(float v){
    flylog("camera y:%f->%f",_cameraPos.y,v);
    _cameraPos.y=v;
    _dirtyPos=true;
}
void camera::setPositionZ(float v){
    flylog("camera z:%f->%f",_cameraPos.z,v);
    _cameraPos.z=v;
    _dirtyPos=true;
}

void camera::setPositionFront(glm::vec3 pos){
    _cameraFront.x=pos.x;
    _cameraFront.y=pos.y;
    _cameraFront.z=pos.z;
    _dirtyPos=true;
}


void camera::moveBy(glm::vec3 v){
    _cameraPos.x+=v.x;
    _cameraPos.y+=v.y;
    _cameraPos.z+=v.z;
    _dirtyPos=true;
}

void camera::rotate(glm::vec3 v){
    if(v.x)
        _matCamera=glm::rotate(_matCamera,v.x,glm::vec3(1,0,0));
    if(v.y)
        _matCamera=glm::rotate(_matCamera,v.y,glm::vec3(0,1,0));
    if(v.z)
        _matCamera=glm::rotate(_matCamera,v.z,glm::vec3(0,0,1));
    _dirtyPos=true;
}

void camera::enableControl(){
    _controlObj=new flyEngine::control();
    _controlObj->bindCamera(this);
}

control* camera::getControl(){
    return _controlObj;
}

void camera::print(){
    flylog("camera pos is %f %f %f",_cameraPos.x,_cameraPos.y,_cameraPos.z);
}


bool camera::init(){
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

void camera::update(int programID){
    _program=programID;
    _updateCamera();
    _updateProjection();
}
