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
#include "frustum.h"

using namespace flyEngine;

camera::camera(){
    init();
}

void camera::_updateCamera(){
    int sizeArr[]={64,12};
    int offsetArr[]={64,128};
    if(!_dirtyPos)
        return;
    _dirtyPos=false;
    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
    if(_bFocus)
        _matCamera=glm::lookAt(_cameraPos, _focusPos, _cameraUp);
    void* bufArr[]={
        (void*)glm::value_ptr(_matCamera),
        (void*)glm::value_ptr(_cameraPos)
    };
    uboMgr::writeData(_ubo_mat_3d,2,sizeArr,offsetArr,bufArr);
    
    updateFrustum();
}

void camera::_updateProjection(){
    int sizeArr[]={64};
    int offsetArr[]={0};
    void* bufArr[]={
        (void*)glm::value_ptr(_matProjPerspective)
    };
    uboMgr::writeData(_ubo_mat_3d,1,sizeArr,offsetArr,bufArr);
}


void camera::reset(){
//    shaderMgr::setMat4(_program,uniform_name_mat_camera,glm::value_ptr(_matCameraOrigin));
    _matCamera=_matCameraOrigin;
    setPosition(_cameraPosOrigin);
    _updateUBO();
    
}

void camera::setPosition(glm::vec3 pos){
    _cameraPos.x=pos.x;
    _cameraPos.y=pos.y;
    _cameraPos.z=pos.z;
    _dirtyPos=true;
}
void camera::setPositionX(float v){
//    flylog("camera x:%f->%f",_cameraPos.x,v);
    _cameraPos.x=v;
    _dirtyPos=true;
}
void camera::setPositionY(float v){
//    flylog("camera y:%f->%f",_cameraPos.y,v);
    _cameraPos.y=v;
    _dirtyPos=true;
}
void camera::setPositionZ(float v){
//    flylog("camera z:%f->%f",_cameraPos.z,v);
    _cameraPos.z=v;
    _dirtyPos=true;
}

void camera::_updateFront(){
    glm::vec3 posFront;
    posFront.x=cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
    posFront.y=sin(glm::radians(_pitch));
    posFront.z=sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
    posFront=glm::normalize(posFront);
    _cameraFront.x=posFront.x;
    _cameraFront.y=posFront.y;
    _cameraFront.z=posFront.z;
    _cameraRight=glm::normalize(glm::cross(_cameraFront,glm::vec3(0,1,0)));
    _cameraUp=glm::normalize(glm::cross(_cameraRight,_cameraFront));
    _dirtyPos=true;
}

void camera::setYaw(float v){
    _yaw=v;
    _updateFront();
}
void camera::setPitch(float v){
    _pitch=v;
    _updateFront();
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
    _yaw=-90;//points to -z
    _pitch=0;
    _fov=30.0;  //field of view,in short:fov, you can see more with high fov, ex 45.0;
    _fovOrigin=_fov;
    _screenRatio=800/600;
    
    _cameraPos=glm::vec3(0,0,3);
    _cameraPosOrigin=_cameraPos;
    glm::vec3 direction;
    direction.x=cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
    direction.y=sin(glm::radians(_pitch));
    direction.z=sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
    _cameraFront=glm::normalize(direction);
    _cameraRight=glm::normalize(glm::cross(_cameraFront,glm::vec3(0,1,0)));
    _cameraUp=glm::normalize(glm::cross(_cameraRight,_cameraFront));
 
    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
    _matCameraOrigin=_matCamera;
    
    _matProjPerspective=glm::perspective(glm::radians(double(_fov)), (double)_screenRatio, (double)_nearPlane, (double)_farPlane);
    _matProjPerspectiveOrigin=_matProjPerspective;
    
    _matProjOrtho=glm::ortho(0.0f,(float)g_winWidth,0.0f,(float)g_winHigh);
    
    _dirtyPos=false;
    initUBO();
    initFrustum();
    return true;
}

void camera::setFarPlane(float v){
    _farPlane=v;
    _matProjPerspective=glm::perspective(glm::radians(double(_fov)), (double)_screenRatio, (double)_nearPlane, (double)_farPlane);
    _matProjPerspectiveOrigin=_matProjPerspective;
}
void camera::setNearPlane(float v){
    _nearPlane=v;
    _matProjPerspective=glm::perspective(glm::radians(double(_fov)), (double)_screenRatio, (double)_nearPlane, (double)_farPlane);
    _matProjPerspectiveOrigin=_matProjPerspective;
}

void camera::initFrustum(){
    frustum* stFrustum=new frustum;
    const float halfVSide = _farPlane * tanf(_fov * .5f);
    const float halfHSide = halfVSide * _screenRatio;
    const glm::vec3 frontMultFar = _farPlane * _cameraFront;
    stFrustum->near = { _cameraPos + _nearPlane * _cameraFront, _cameraFront,"near" };
    stFrustum->far = { _cameraPos + frontMultFar, -_cameraFront,"far"};
    stFrustum->right = { _cameraPos,glm::cross(_cameraUp,frontMultFar + _cameraRight * halfHSide),"right" };
    stFrustum->left = { _cameraPos, glm::cross(frontMultFar - _cameraRight * halfHSide,_cameraUp),"left" };
    stFrustum->top = { _cameraPos,glm::cross(_cameraRight, frontMultFar - _cameraUp * halfVSide),"top" };
    stFrustum->bottom = { _cameraPos,glm::cross(frontMultFar + _cameraUp * halfVSide, _cameraRight),"bottom" };
    _stFrustum=stFrustum;
}

void camera::updateFrustum(){
    const float halfVSide = _farPlane * tanf(_fov * .5f);
    const float halfHSide = halfVSide * _screenRatio;
    const glm::vec3 frontMultFar = _farPlane * _cameraFront;
    _stFrustum->near = { _cameraPos + _nearPlane * _cameraFront, _cameraFront,"near" };
    _stFrustum->far = { _cameraPos + frontMultFar, -_cameraFront,"far"};
    _stFrustum->right = { _cameraPos,glm::cross(_cameraUp,frontMultFar + _cameraRight * halfHSide),"right" };
    _stFrustum->left = { _cameraPos, glm::cross(frontMultFar - _cameraRight * halfHSide,_cameraUp),"left" };
    _stFrustum->top = { _cameraPos,glm::cross(_cameraRight, frontMultFar - _cameraUp * halfVSide),"top" };
    _stFrustum->bottom = { _cameraPos,glm::cross(frontMultFar + _cameraUp * halfVSide, _cameraRight),"bottom" };
}


glm::mat4 camera::getLookAtMatrix(){
    return _matCamera;
}
glm::mat4 camera::getPerspectiveMatrix(){
    return _matProjPerspective;
}

void camera::update(){
    //proj矩阵在共享内存里，在相机初始化已经设置，不需要重复更新
    //_updateProjection();
    _updateCamera();
}

void camera::update2D(){
    int sizeArr[]={64,64,12};
    int offsetArr[]={0,64,128};
    void* bufArr[]={
        (void*)glm::value_ptr(_matProjOrtho),
        (void*)glm::value_ptr(_matCamera),
        (void*)glm::value_ptr(_cameraPos)
    };
    uboMgr::writeData(_ubo_mat_2d,3,sizeArr,offsetArr,bufArr);
}

void camera::_updateUBO(){
    int sizeArr[]={64,64,12};
    int offsetArr[]={0,64,128};
    void* bufArr[]={
        (void*)glm::value_ptr(_matProjPerspective),
        (void*)glm::value_ptr(_matCamera),
        (void*)glm::value_ptr(_cameraPos)
    };
    uboMgr::writeData(_ubo_mat_3d,3,sizeArr,offsetArr,bufArr);
    bufArr[0]=(void*)glm::value_ptr(_matProjOrtho);
    uboMgr::writeData(_ubo_mat_2d,3,sizeArr,offsetArr,bufArr);
}

void camera::updateUBOForShadow(glm::vec3 lightPos){
    float near_plane=1.0f;
    float far_plane=7.5f;
    glm::mat4 matProjOrthoForDir=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
    glm::mat4 matCamera=glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));   //look at center of scene
    int sizeArr[]={64,64,12};
    int offsetArr[]={0,64,128};
    void* bufArr[]={
        (void*)glm::value_ptr(matProjOrthoForDir),
        (void*)glm::value_ptr(matCamera),
        (void*)glm::value_ptr(_cameraPos)
    };
    uboMgr::writeData(_ubo_mat_3d_shadow,3,sizeArr,offsetArr,bufArr);
}
//glm::mat4 camera::getLightSpaceMat(glm::vec3 lightPos){
//    float near_plane=1.0f;
//    float far_plane=7.5f;
//    glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
////    glm::mat4 matCamera=glm::lookAt(lightPos,_cameraPos+_cameraFront, _cameraUp);   //look at center of scene
//    glm::mat4 matCamera=glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));   //look at center of scene
//    return matProj*matCamera;
//}

void camera::initUBO(){
    // _ubo_mat=uboMgr::createUBO(ubo_binding_mat,ubo_size_mat,"mat");
    // _ubo_mat_2d=uboMgr::createUBO(ubo_binding_mat_2d,ubo_size_mat_2d,"mat2d");
    _ubo_mat_2d=g_ubo_id_mat_2d;
    _ubo_mat_3d=g_ubo_id_mat_3d;
    _ubo_mat_3d_shadow=g_ubo_id_mat_3d_shadow;
    _updateUBO();
}
