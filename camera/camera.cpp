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
#include "window.h"
#include "uboMgr.h"

using namespace flyEngine;

camera::camera(){
    init();
}

void camera::_updateCamera(){
    int sizeArr[]={64,16};
    int offsetArr[]={64,128};
    if(!_dirtyPos)
        return;
    _dirtyPos=false;
    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
    void* bufArr[]={
        (void*)glm::value_ptr(_matCamera),
        (void*)glm::value_ptr(_cameraPos)
    };
    uboMgr::writeData(_ubo_mat,2,sizeArr,offsetArr,bufArr);
}

void camera::_updateProjection(){
    int sizeArr[]={64};
    int offsetArr[]={0};
    void* bufArr[]={
        (void*)glm::value_ptr(_matProj)
    };
    uboMgr::writeData(_ubo_mat,1,sizeArr,offsetArr,bufArr);
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
    
    _matProj2D=glm::ortho(0.0f,(float)g_winWidth,0.0f,(float)g_winHigh);
    
    _dirtyPos=false;
//    _dirtyProj=true;
    
    initUBO();
    return true;
}

glm::mat4 camera::getLookAtMatrix(){
    return _matCamera;
}
glm::mat4 camera::getProjMatrix(){
    return _matProj;
}

void camera::update(){
    //proj矩阵在共享内存里，在相机初始化已经设置，不需要重复更新
    //_updateProjection();
    _updateCamera();
}

void camera::update2D(){
    int sizeArr[]={64,64,16};
    int offsetArr[]={0,64,128};
    void* bufArr[]={
        (void*)glm::value_ptr(_matProj2D),
        (void*)glm::value_ptr(_matCamera),
        (void*)glm::value_ptr(_cameraPos)
    };
    uboMgr::writeData(_ubo_mat,3,sizeArr,offsetArr,bufArr);
}

void camera::_updateUBO(){
    int sizeArr[]={64,64,16};
    int offsetArr[]={0,64,128};
    void* bufArr[]={
        (void*)glm::value_ptr(_matProj),
        (void*)glm::value_ptr(_matCamera),
        (void*)glm::value_ptr(_cameraPos)
    };
    uboMgr::writeData(_ubo_mat,3,sizeArr,offsetArr,bufArr);
    bufArr[0]=(void*)glm::value_ptr(_matProj2D),
    uboMgr::writeData(_ubo_mat_2d,3,sizeArr,offsetArr,bufArr);
}

void camera::initUBO(){
    _ubo_mat=uboMgr::createUBO(ubo_binding_mat,ubo_size_mat);
    _ubo_mat_2d=uboMgr::createUBO(ubo_binding_mat_2d,ubo_size_mat_2d);
    _updateUBO();
}
