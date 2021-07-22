//
//  camera.cpp
//  flyEngine
//
//  Created by joe on 16/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "camera.h"

void camera::updateCamera(){
    if(_modeFPS)
        _cameraPos.y=0;
    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
    glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameCamera.c_str()), 1,GL_FALSE,glm::value_ptr(_matCamera));
}

void camera::updateProjection(){
    _matProj=glm::perspective(glm::radians((double)_fov),(double)_screenRatio, 0.1, 100.0);
    glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameProj.c_str()), 1,GL_FALSE,glm::value_ptr(_matProj));
}

void camera::updateModel(){
    glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameModel.c_str()), 1,GL_FALSE,glm::value_ptr(_matModel));
}

void camera::print(){
    cout<<"_matNameCamera is "<<_matNameCamera<<endl;
}

camera::camera(int shaderID,
                    string matNameModel,glm::mat4 matModel,
                    string matNameCamera,glm::vec3 cameraPos,glm::vec3 cameraUp,
                    string matNameProj,float screenRatio){
    _shaderID=shaderID;
    _screenRatio=screenRatio;
    
    _matModel=matModel;
    _matModelOrigin=matModel;
    _matNameModel=matNameModel;
    
    _matNameCamera=matNameCamera;
    _cameraPos=cameraPos;
    _cameraFront.x=cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
    _cameraFront.y=sin(glm::radians(_pitch));
    _cameraFront.z=sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
    _cameraFront=glm::normalize(_cameraFront);
    _cameraUp=cameraUp;
    _matCamera=glm::lookAt(_cameraPos, _cameraPos+_cameraFront, _cameraUp);
    _matCameraOrigin=_matCamera;
    
    _matProj=glm::perspective(glm::radians(double(_fov)), (double)_screenRatio, 0.1, 100.0);
    _matProjOrigin=_matProj;
    _matNameProj=matNameProj;
    _fovOrigin=_fov;
    
    
    updateCamera();
    updateProjection();
    updateModel();
    
    
    keyboardEvent* kbEventObj=new keyboardEvent();
    kbEventObj->regEvent('w', [=,this](){
        //_cameraPos.z-=_move_d;
        this->_cameraPos.z-=0.2;
        cout<<"_cameraPos.z is "<<this->_cameraPos.z<<endl;
//        _matCamera=glm::translate(_matCamera, glm::vec3(0,0,_move_d));
        this->updateCamera();
    });
    
    kbEventObj->regEvent('s', [&](){
        _cameraPos.z+=_move_d;
        cout<<"_cameraPos.z is "<<this->_cameraPos.z<<endl;
//        _matCamera=glm::translate(_matCamera, glm::vec3(0,0,-_move_d));
        updateCamera();
    });
    kbEventObj->regEvent('a', [&](){
//         _cameraPos.x-=_move_d;

        _cameraPos.x-=0.2;
        cout<<"_cameraPos.x is "<<this->_cameraPos.x<<endl;
//         _matCamera=glm::translate(_matCamera, glm::vec3(-_move_d,0,0));
         updateCamera();
      });
    kbEventObj->regEvent('d', [&](){
//         _cameraPos.x+=_move_d;
        _cameraPos.x+=0.2;
        cout<<"_cameraPos.x is "<<this->_cameraPos.x<<endl;
//         _matCamera=glm::translate(_matCamera, glm::vec3(_move_d,0,0));
        updateCamera();
    });
    kbEventObj->regEvent('z', [&](){
         _cameraPos.y+=_move_d;
        cout<<"_cameraPos.y is "<<this->_cameraPos.y<<endl;
//         _matCamera=glm::translate(_matCamera, glm::vec3(0,_move_d,0));
        updateCamera();
    });
    kbEventObj->regEvent('x', [&](){
         _cameraPos.y-=_move_d;
        cout<<"_cameraPos.y is "<<this->_cameraPos.y<<endl;
//        _matCamera=glm::translate(_matCamera, glm::vec3(0,-_move_d,0));
        updateCamera();
    });

    kbEventObj->regEvent('r', [&](){
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameModel.c_str()), 1,GL_FALSE,glm::value_ptr(_matModelOrigin));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameCamera.c_str()), 1,GL_FALSE,glm::value_ptr(_matCameraOrigin));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameProj.c_str()), 1,GL_FALSE,glm::value_ptr(_matProjOrigin));
    });
    char szBuf[512]={0};
    snprintf(szBuf,sizeof(szBuf),"kbmCamera_%d_%s",_shaderID,_matNameCamera.c_str());
    keyboardEventMgr::addEvent(string(szBuf), kbEventObj);
        
    mouseEvent* msEventObj=new mouseEvent();
    msEventObj->regOnLeftClick([&](){
        _mouseLeftOriginX=0;
        _mouseLeftOriginY=0;
    });
    msEventObj->regOnRightClick([&](){
        _mouseRightOriginX=0;
        _mouseRightOriginY=0;
    });
    msEventObj->regOnMiddleClick([&](){
        cout<<"middle click!"<<endl;
    });
    //move camera
    msEventObj->regOnMoveWithLeftClick([&](float x,float y){
        if(_mouseLeftOriginX==0){
            _mouseLeftOriginX=x;
            _mouseLeftOriginY=y;
            return;
        }

        float dx=x-_mouseLeftOriginX;
        float dy=y-_mouseLeftOriginY;
        float rotateX=360*(dx/_width2PI);
        float rotateY=360*(dy/_height2PI);
        _mouseLeftOriginX=x;
        _mouseLeftOriginY=y;
        
//        _matCamera=glm::rotate(_matCamera,glm::radians(rotateX),glm::vec3(0,1,0));
//        _matCamera=glm::rotate(_matCamera,glm::radians(rotateY),glm::vec3(1,0,0));
//        glUniformMatrix4fv(glGetUniformLocation(_shaderID,_matNameCamera.c_str()), 1,GL_FALSE,glm::value_ptr(_matCamera));
        
        _yaw+=rotateX;
        _pitch+=rotateY;
        _cameraFront.x=cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
        _cameraFront.y=sin(glm::radians(_pitch));
        _cameraFront.z=sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
        _cameraFront=glm::normalize(_cameraFront);
        updateCamera();
    });
    //move model itself
    msEventObj->regOnMoveWithRightClick([&](float x,float y){
        if(_mouseRightOriginX==0){
            _mouseRightOriginX=x;
            _mouseRightOriginY=y;
            return;
        }

        float dx=x-_mouseRightOriginX;
        float dy=y-_mouseRightOriginY;
        float rotateX=360*(dx/_width2PI);
        float rotateY=360*(dy/_height2PI);
        _mouseRightOriginX=x;
        _mouseRightOriginY=y;
        _matModel=glm::rotate(_matModel,glm::radians(rotateX),glm::vec3(0,1,0));
        _matModel=glm::rotate(_matModel,glm::radians(rotateY),glm::vec3(1,0,0));
        updateModel();
    });
    msEventObj->regOnMoveWithMiddleClick([&](float x,float y){
        cout<<"on mouse middle move!"<<x<<" "<<y<<endl;
    });

    msEventObj->regOnScroll([&](float x,float y){
//        cout<<"on mouse scroll!"<<x<<" "<<y<<endl;
        float move=y*0.1;
        _fov+=move;
        updateProjection();
        
//        _matCamera=glm::translate(_matCamera,glm::vec3(0,0,move));
//        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameCamera.c_str()), 1,GL_FALSE,glm::value_ptr(_matCamera));
        
    });

    mouseEventMgr::addEvent("test3d_move_camera", msEventObj);
}
