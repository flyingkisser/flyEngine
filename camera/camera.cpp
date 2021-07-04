//
//  camera.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/7/4.
//  Copyright © 2021 joe. All rights reserved.
//

#include "camera.h"
#include "flyEngine.h"

void camera::_updateCamera(){
    if(!_program)
        return;
    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
    glUniformMatrix4fv(glGetUniformLocation(_program, "matCamera"), 1,GL_FALSE,glm::value_ptr(_matCamera));
}

void camera::updateCameraPos(float x, float y, float z){
    _cameraPos.x=x;
    _cameraPos.y=y;
    _cameraPos.z=z;
}

void camera::_updateProjection(){
    if(!_program)
        return;
    _matProj=glm::perspective(glm::radians((double)_fov),(double)_screenRatio, 0.1, 100.0);
    glUniformMatrix4fv(glGetUniformLocation(_program, "matProj"), 1,GL_FALSE,glm::value_ptr(_matProj));
}


void camera::print(){
    flylog("camera pos is %f %f %f",_cameraPos.x,_cameraPos.y,_cameraPos.z);
}
camera::camera(int program){
    _program=program;
    _init();
}
camera::camera(){
    _init();
}

void camera::_init(){
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
    
    if(_program){
        _updateProjection();
        _updateCamera();
    }
}
