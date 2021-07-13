//
//  control.cpp
//  flyEngine
//
//  Created by joe on 11/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "control.h"
#include "flyEngine.h"

void control::bindCamera(flyEngine::camera* c){
    _camera=c;
    char szBuf[512]={0};
        
    snprintf(szBuf,sizeof(szBuf),"keyboard_control");
    keyboardEvent* kbEventObj=new keyboardEvent();
    keyboardEventMgr::addEvent(string(szBuf), kbEventObj);

    mouseEvent* msEventObj=new mouseEvent();

    kbEventObj->regEvent('w', [&](){
        _camera->setPositionZ(_camera->getPositionZ()-_move_d);
    });

    kbEventObj->regEvent('s', [&](){
         _camera->setPositionZ(_camera->getPositionZ()+_move_d);
    });
    kbEventObj->regEvent('a', [&](){
        _camera->setPositionX(_camera->getPositionX()-_move_d);
      });
    kbEventObj->regEvent('d', [&](){
        _camera->setPositionX(_camera->getPositionX()+_move_d);
    });
    kbEventObj->regEvent('z', [&](){
       _camera->setPositionY(_camera->getPositionY()+_move_d);
    });
    kbEventObj->regEvent('x', [&](){
       _camera->setPositionY(_camera->getPositionY()-_move_d);
    });

    kbEventObj->regEvent('r', [&](){
        _camera->reset();
    });

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
        
        _yaw+=rotateX;
        _pitch+=rotateY;
        glm::vec3 posFront;
        posFront.x=cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
        posFront.y=sin(glm::radians(_pitch));
        posFront.z=sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
        posFront=glm::normalize(posFront);
        _camera->setPositionFront(glm::vec3(posFront.x, posFront.y, posFront.z));
    });

    //move model itself
    //    msEventObj->regOnMoveWithRightClick([&](float x,float y){
    //        if(_mouseRightOriginX==0){
    //            _mouseRightOriginX=x;
    //            _mouseRightOriginY=y;
    //            return;
    //        }
    //
    //        float dx=x-_mouseRightOriginX;
    //        float dy=y-_mouseRightOriginY;
    //        float rotateX=360*(dx/_width2PI);
    //        float rotateY=360*(dy/_height2PI);
    //        _mouseRightOriginX=x;
    //        _mouseRightOriginY=y;
    //        _matModel=glm::rotate(_matModel,glm::radians(rotateX),glm::vec3(0,1,0));
    //        _matModel=glm::rotate(_matModel,glm::radians(rotateY),glm::vec3(1,0,0));
    //        updateModel();
    //    });

    msEventObj->regOnMoveWithMiddleClick([&](float x,float y){
        cout<<"on mouse middle move!"<<x<<" "<<y<<endl;
    });

    msEventObj->regOnScroll([&](float x,float y){
        cout<<"on mouse scroll!"<<x<<" "<<y<<endl;
    });

    mouseEventMgr::addEvent("control_mouse", msEventObj);
}
