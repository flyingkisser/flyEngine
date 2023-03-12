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
    int sizeArr[]={64,64,12};
    int offsetArr[]={0,64,128};
    if(!_dirty)
        return;
    _dirty=false;
    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
    _matProjPerspective=glm::perspective(glm::radians(double(_fov)), (double)_screenRatio, (double)_nearPlane, (double)_farPlane);
    if(_bFocus)
        _matCamera=glm::lookAt(_cameraPos, _focusPos, _cameraUp);
    void* bufArr[]={
        (void*)glm::value_ptr(_matProjPerspective),
        (void*)glm::value_ptr(_matCamera),
        (void*)glm::value_ptr(_cameraPos)
    };
    uboMgr::writeData(_ubo_mat_3d,3,sizeArr,offsetArr,bufArr);
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
    _matCamera=_matCameraOrigin;
    setPosition(_cameraPosOrigin);
    _updateUBO();
}

void camera::setPosition(glm::vec3 pos){
    _cameraPos.x=pos.x;
    _cameraPos.y=pos.y;
    _cameraPos.z=pos.z;
    _dirty=true;
}
void camera::setPositionX(float v){
    _cameraPos.x=v;
    _dirty=true;
}
void camera::setPositionY(float v){
    _cameraPos.y=v;
    _dirty=true;
}
void camera::setPositionZ(float v){
    _cameraPos.z=v;
    _dirty=true;
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
    _dirty=true;
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
    _dirty=true;
}

void camera::rotate(glm::vec3 v){
    if(v.x)
        _matCamera=glm::rotate(_matCamera,v.x,glm::vec3(1,0,0));
    if(v.y)
        _matCamera=glm::rotate(_matCamera,v.y,glm::vec3(0,1,0));
    if(v.z)
        _matCamera=glm::rotate(_matCamera,v.z,glm::vec3(0,0,1));
    _dirty=true;
}

void camera::enableControl(){
    _controlObj=new flyEngine::control();
    _controlObj->bindCamera(this);
}

control* camera::getControl(){
    return _controlObj;
}

void camera::print(){
//    flylog("camera pos is %f %f %f",_cameraPos.x,_cameraPos.y,_cameraPos.z);
    flylog("camera: zoom %f ratio %f near %f far %f",_fov,_screenRatio,_nearPlane,_farPlane);
}


bool camera::init(){
    _yaw=-90.0;//points to -z
    _pitch=0.0;
    _fov=45.0;  //field of view,in short:fov, you can see more with high fov, ex 45.0;
    _fovOrigin=_fov;
    _screenRatio=800.0f/600.0f;
    
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
    
    _dirty=false;
    initUBO();
    initFrustum();
    return true;
}

void camera::setFarPlane(float v){
    _farPlane=v;
    _dirty=true;
}
void camera::setNearPlane(float v){
    _nearPlane=v;
    _dirty=true;
}
void camera::setScreenRatio(float v){
    _screenRatio=v;
    _dirty=true;
}
void camera::setFov(float v){
    _fov=v;
    _dirty=true;
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


void camera::initFrustum(){
//    frustum* stFrustum=new frustum;
//    const float halfVSide = _farPlane * tanf(_fov * .5f);
//    const float halfHSide = halfVSide * _screenRatio;
//    const glm::vec3 frontMultFar = _farPlane * _cameraFront;
//    stFrustum->near = { _cameraPos + _nearPlane * _cameraFront, _cameraFront,"near" };
//    stFrustum->far = { _cameraPos + frontMultFar, -_cameraFront,"far"};
//    stFrustum->right = { _cameraPos,glm::cross(_cameraUp,frontMultFar + _cameraRight * halfHSide),"right" };
//    stFrustum->left = { _cameraPos, glm::cross(frontMultFar - _cameraRight * halfHSide,_cameraUp),"left" };
//    stFrustum->top = { _cameraPos,glm::cross(_cameraRight, frontMultFar - _cameraUp * halfVSide),"top" };
//    stFrustum->bottom = { _cameraPos,glm::cross(frontMultFar + _cameraUp * halfVSide, _cameraRight),"bottom" };
//    _stFrustum=stFrustum;
    glm::mat4 clips=_matProjPerspective*_matCamera;
    float clip[16];
    for(int i=0;i<16;i++)
        clip[i]=clips[i/4][i%4];
    float l;
   
    //right plane
    _frustum[0][0]=clip[3]-clip[0];
    _frustum[0][1]=clip[7]-clip[4];
    _frustum[0][2]=clip[11]-clip[8];
    _frustum[0][3]=clip[15]-clip[12];
    l=sqrt((_frustum[0][0]*_frustum[0][0]+_frustum[0][1]*_frustum[0][1]+_frustum[0][2]*_frustum[0][2]+_frustum[0][3]*_frustum[0][3]));
    _frustum[0][0]/=l;
    _frustum[0][1]/=l;
    _frustum[0][2]/=l;
    _frustum[0][3]/=l;
    
    //left plane
    _frustum[1][0]=clip[3]+clip[0];
    _frustum[1][1]=clip[7]+clip[4];
    _frustum[1][2]=clip[11]+clip[8];
    _frustum[1][3]=clip[15]+clip[12];
    l=sqrt((_frustum[1][0]*_frustum[1][0]+_frustum[1][1]*_frustum[1][1]+_frustum[1][2]*_frustum[1][2]+_frustum[1][3]*_frustum[1][3]));
    _frustum[1][0]/=l;
    _frustum[1][1]/=l;
    _frustum[1][2]/=l;
    _frustum[1][3]/=l;
    
    //bottom plane
    _frustum[2][0]=clip[3]+clip[1];
    _frustum[2][1]=clip[7]+clip[5];
    _frustum[2][2]=clip[11]+clip[9];
    _frustum[2][3]=clip[15]+clip[13];
    l=sqrt((_frustum[2][0]*_frustum[2][0]+_frustum[2][1]*_frustum[2][1]+_frustum[2][2]*_frustum[2][2]+_frustum[2][3]*_frustum[2][3]));
    _frustum[2][0]/=l;
    _frustum[2][1]/=l;
    _frustum[2][2]/=l;
    _frustum[2][3]/=l;
    
    //top plane
    _frustum[3][0]=clip[3]-clip[1];
    _frustum[3][1]=clip[7]-clip[5];
    _frustum[3][2]=clip[11]-clip[9];
    _frustum[3][3]=clip[15]-clip[13];
    l=sqrt((_frustum[3][0]*_frustum[3][0]+_frustum[3][1]*_frustum[3][1]+_frustum[3][2]*_frustum[3][2]+_frustum[3][3]*_frustum[3][3]));
    _frustum[3][0]/=l;
    _frustum[3][1]/=l;
    _frustum[3][2]/=l;
    _frustum[3][3]/=l;
    
    //far plane
    _frustum[4][0]=clip[3]-clip[2];
    _frustum[4][1]=clip[7]-clip[6];
    _frustum[4][2]=clip[11]-clip[10];
    _frustum[4][3]=clip[15]-clip[14];
    l=sqrt((_frustum[4][0]*_frustum[4][0]+_frustum[4][1]*_frustum[4][1]+_frustum[4][2]*_frustum[4][2]+_frustum[4][3]*_frustum[4][3]));
    _frustum[4][0]/=l;
    _frustum[4][1]/=l;
    _frustum[4][2]/=l;
    _frustum[4][3]/=l;
    
    //near plane
    _frustum[5][0]=clip[3]+clip[2];
    _frustum[5][1]=clip[7]+clip[6];
    _frustum[5][2]=clip[11]+clip[10];
    _frustum[5][3]=clip[15]+clip[14];
    l=sqrt((_frustum[5][0]*_frustum[5][0]+_frustum[5][1]*_frustum[5][1]+_frustum[5][2]*_frustum[5][2]+_frustum[5][3]*_frustum[5][3]));
    _frustum[5][0]/=l;
    _frustum[5][1]/=l;
    _frustum[5][2]/=l;
    _frustum[5][3]/=l;
    
}

void camera::updateFrustum(){
//    const float halfVSide = _farPlane * tanf(_fov * .5f);
//    const float halfHSide = halfVSide * _screenRatio;
//    const glm::vec3 frontMultFar = _farPlane * _cameraFront;
//    _stFrustum->near = { _cameraPos + _nearPlane * _cameraFront, _cameraFront,"near" };
//    _stFrustum->far = { _cameraPos + frontMultFar, -_cameraFront,"far"};
//    _stFrustum->right = { _cameraPos,glm::cross(_cameraUp,frontMultFar + _cameraRight * halfHSide),"right" };
//    _stFrustum->left = { _cameraPos, glm::cross(frontMultFar - _cameraRight * halfHSide,_cameraUp),"left" };
//    _stFrustum->top = { _cameraPos,glm::cross(_cameraRight, frontMultFar - _cameraUp * halfVSide),"top" };
//    _stFrustum->bottom = { _cameraPos,glm::cross(frontMultFar + _cameraUp * halfVSide, _cameraRight),"bottom" };
    initFrustum();
}

bool camera::isInFrustumBySphere(glm::vec3 p,float r){
    for(int i=0;i<6;i++){
        if(_frustum[i][0]*p.x+_frustum[i][1]*p.y+_frustum[i][2]*p.z+_frustum[i][3]<=-r)
            return false;
    }
    return true;
}

bool camera::isInFrustumByAABB(glm::vec3 p,glm::vec3 rx,glm::vec3 ry,glm::vec3 rz){
    for(int i=0;i<6;i++){
        float vx=_frustum[i][0]*rx.x+_frustum[i][1]*rx.y+_frustum[i][2]*rx.z;
        float vy=_frustum[i][0]*ry.x+_frustum[i][1]*ry.y+_frustum[i][2]*ry.z;
        float vz=_frustum[i][0]*rz.x+_frustum[i][1]*rz.y+_frustum[i][2]*rz.z;
        float r=max(max(abs(vx),abs(vy)),abs(vz));
        if(_frustum[i][0]*p.x+_frustum[i][1]*p.y+_frustum[i][2]*p.z+_frustum[i][3]<=-r)
            return false;
    }
    return true;
}


void camera::printPlaneDistance(glm::vec3 p,float r){
    for(int i=0;i<6;i++){
        float d=_frustum[i][0]*p.x+_frustum[i][1]*p.y+_frustum[i][2]*p.z+_frustum[i][3];
        flylog("%d d %f -r %f %d",i,d,-r,d<=-r);
    }
}
