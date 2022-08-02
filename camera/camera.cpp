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
    if(!_dirtyPos)
        return;
    _dirtyPos=false;
    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
//    glBindBuffer(GL_UNIFORM_BUFFER,_ubo1);
//    glBufferSubData(GL_UNIFORM_BUFFER,64,64,(void*)glm::value_ptr(_matCamera));
//    glBufferSubData(GL_UNIFORM_BUFFER,128,16,(void*)glm::value_ptr(_cameraPos));
//    glBindBuffer(GL_UNIFORM_BUFFER,0);
    
    int sizeArr[]={64,16};
    void* bufArr[]={
        (void*)glm::value_ptr(_matCamera),
        (void*)glm::value_ptr(_cameraPos)
    };
    uboMgr::writeData(_ubo1,2,sizeArr,bufArr,64);
}

void camera::_updateProjection(){
    
//    glBindBuffer(GL_UNIFORM_BUFFER,_ubo1);
//    glBufferSubData(GL_UNIFORM_BUFFER,0,64,(void*)glm::value_ptr(_matProj));
//    glBindBuffer(GL_UNIFORM_BUFFER,0);
    
    int sizeArr[]={64};
    void* bufArr[]={
        (void*)glm::value_ptr(_matProj)
    };
    uboMgr::writeData(_ubo1,1,sizeArr,bufArr);
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
    
    _dirtyPos=true;
    _dirtyProj=true;
    
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
//    glBindBuffer(GL_UNIFORM_BUFFER,_ubo1);
//    glBufferSubData(GL_UNIFORM_BUFFER,0,64,(void*)glm::value_ptr(_matProj2D));
//    glBindBuffer(GL_UNIFORM_BUFFER,0);

    int sizeArr[]={64};
    void* bufArr[]={
        (void*)glm::value_ptr(_matProj2D)
    };
    uboMgr::writeData(_ubo1,1,sizeArr,bufArr);
}

void camera::_updateUBO(){
    int sizeArr[]={64,64,16};
    void* bufArr[]={
        (void*)glm::value_ptr(_matProj),
        (void*)glm::value_ptr(_matCamera),
        (void*)glm::value_ptr(_cameraPos)
    };
    uboMgr::writeData(_ubo1,3,sizeArr,bufArr);

//    glBindBuffer(GL_UNIFORM_BUFFER,_ubo1);
//    glBufferSubData(GL_UNIFORM_BUFFER,0,64,(void*)glm::value_ptr(_matProj));
//    glBufferSubData(GL_UNIFORM_BUFFER,64,64,(void*)glm::value_ptr(_matCamera));
//    glBufferSubData(GL_UNIFORM_BUFFER,128,16,(void*)glm::value_ptr(_cameraPos));
//    glBindBuffer(GL_UNIFORM_BUFFER,0);
}

void camera::initUBO(){
    _ubo1=uboMgr::createUBO(ubo_binding_mat,ubo_size_mat);
//    glGenBuffers(1,&_ubo1);
//    glBindBuffer(GL_UNIFORM_BUFFER,_ubo1);
//    glBufferData(GL_UNIFORM_BUFFER,144,NULL,GL_DYNAMIC_DRAW);
//    glBindBufferRange(GL_UNIFORM_BUFFER,0,_ubo1,0,144);
    _updateUBO();
}

//void camera::_updateCamera_old(){
//    if(!_program)
//        return;
//    if(_dirtyPos){
//        _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
//        _dirtyPos=false;
//    }
//    shaderMgr::setMat4(_program,uniform_name_mat_camera,glm::value_ptr(_matCamera));
//
//    int pos=glGetUniformLocation(_program, uniform_name_camera_pos);
//    if(pos!=-1)
//        shaderMgr::setVec3(_program,uniform_name_camera_pos,glm::value_ptr(_cameraPos));
//}
//void camera::_updateProjection_old(){
//    if(!_program)
//        return;
//    glBindBuffer(GL_UNIFORM_BUFFER,_ubo1);
//    glBufferSubData(GL_UNIFORM_BUFFER,0,64,(void*)glm::value_ptr(_matProj));
//    glBindBuffer(GL_UNIFORM_BUFFER,0);
//    shaderMgr::setMat4(_program,uniform_name_mat_proj,glm::value_ptr(_matProj));
//}
//void camera::updateUBOOnDirty(){
//    if(!_dirtyPos)
//        return;
//    _dirtyPos=false;
//    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
//    _updateUBO();
//}
