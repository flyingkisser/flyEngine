//
//  control.cpp
//  flyEngine
//
//  Created by joe on 11/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "control.h"
#include "keyboardEvent.h"
#include "keyboardEventMgr.h"
#include "mouseEvent.h"
#include "mouseEventMgr.h"
#include "camera.h"
#include "randUtil.h"
#include "logUtil.h"
#include "node.h"

using namespace flyEngine;

void control::bindCamera(flyEngine::camera* c){
    _camera=c;
    char szBuf[512]={0};
    char szRand[32]={0};
    
    randUtil::getRandStr(6, szRand);
        
    snprintf(szBuf,sizeof(szBuf),"event_control_%s",szRand);
    _kbEventObj=new keyboardEvent();
    keyboardEventMgr::addEvent(string(szBuf),_kbEventObj);

    _msEventObj=new mouseEvent();
    mouseEventMgr::addEvent(szBuf, _msEventObj);
    
    //keyboard event
    _kbEventObj->regEvent('w', [&](){
        _camera->setPositionZ(_camera->getPositionZ()-_move_d);
        if(_bindNodeWithCameraMove!=NULL){
            _bindNodeWithCameraMove->setPositionZ(_camera->getPositionZ()+_cameraMoveNodeZ);
        }
    });

    _kbEventObj->regEvent('s', [&](){
         _camera->setPositionZ(_camera->getPositionZ()+_move_d);
        if(_bindNodeWithCameraMove!=NULL){
            _bindNodeWithCameraMove->setPositionZ(_camera->getPositionZ()+_cameraMoveNodeZ);
        }
    });
    _kbEventObj->regEvent('a', [&](){
        _camera->setPositionX(_camera->getPositionX()-_move_d);
        if(_bindNodeWithCameraMove!=NULL){
            _bindNodeWithCameraMove->setPositionX(_camera->getPositionX());
        }
      });
    _kbEventObj->regEvent('d', [&](){
        _camera->setPositionX(_camera->getPositionX()+_move_d);
        if(_bindNodeWithCameraMove!=NULL){
            _bindNodeWithCameraMove->setPositionX(_camera->getPositionX());
        }
    });
    _kbEventObj->regEvent('z', [&](){
       _camera->setPositionY(_camera->getPositionY()+_move_d);
        if(_bindNodeWithCameraMove!=NULL){
            _bindNodeWithCameraMove->setPositionY(_camera->getPositionY());
        }
    });
    _kbEventObj->regEvent('x', [&](){
       _camera->setPositionY(_camera->getPositionY()-_move_d);
        if(_bindNodeWithCameraMove!=NULL){
            _bindNodeWithCameraMove->setPositionY(_camera->getPositionY());
        }
    });

    _kbEventObj->regEvent('r', [&](){
        _camera->reset();
    });
    
    
    //mouse event
    _msEventObj->regOnLeftClick([&](){
        _mouseLeftLastX=0;
        _mouseLeftLastY=0;
    });
    _msEventObj->regOnRightClick([&](){
//        _mouseRightOriginX=0;
//        _mouseRightOriginY=0;
    });
    _msEventObj->regOnMiddleClick([&](){
        cout<<"middle click!"<<endl;
    });
  
    _msEventObj->regOnMoveWithLeftHold([&](float x,float y){
        if(_mouseLeftLastX==0){
            _mouseLeftLastX=x;
            _mouseLeftLastY=y;
            return;
        }

        float dx=x-_mouseLeftLastX;
        float dy=y-_mouseLeftLastY;
        float rotateX=360*(dx/_width2PI);
        float rotateY=360*(dy/_height2PI);
        _mouseLeftLastX=x;
        _mouseLeftLastY=y;
        
        _yaw+=rotateX;
        _pitch+=rotateY;
        glm::vec3 posFront;
        posFront.x=cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
        posFront.y=sin(glm::radians(_pitch));
        posFront.z=sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
        _camera->setPositionFront(glm::normalize(posFront));
    });

    _msEventObj->regOnMoveWithMiddleHold([&](float x,float y){
      cout<<"on mouse middle move!"<<x<<" "<<y<<endl;
    });

    _msEventObj->regOnScroll([&](float x,float y){
        cout<<"on mouse scroll!"<<x<<" "<<y<<endl;
        //move camera deep or shadow accroding to scroll up or down
        if(y>0)
            _camera->setPositionZ(_camera->getPositionZ()-_move_d);
        else
            _camera->setPositionZ(_camera->getPositionZ()+_move_d);
        if(_bindNodeWithCameraMove!=NULL){
            _bindNodeWithCameraMove->setPositionZ(_camera->getPositionZ()+_cameraMoveNodeZ);
        }
    });
    
    flylog("control::bindCamera finished!");
}

void control::bindNode(flyEngine::node* nodeObj){
    if(_msEventObj==NULL)
        return;
    //move model itself
    _bindNode=nodeObj;
    _msEventObj->regOnMoveWithRightHold([&](float x,float y){
        if(_mouseRightOriginX==0){
          _mouseRightOriginX=x;
          _mouseRightOriginY=y;
          _mouseRightLastX=x;
          _mouseRightLastY=y;
          return;
        }

        float dx=x-_mouseRightLastX;
        float dy=y-_mouseRightLastY;
        float rotateX=360.0f*((dx)/_width2PI);
        float rotateY=360.0f*((dy)/_height2PI);
        _mouseRightLastX=x;
        _mouseRightLastY=y;
        _bindNode->rotateBy(glm::vec3(rotateY,rotateX,0.0f));
    });
    _msEventObj->regOnRightClickRelease([&](){
        _mouseRightOriginX=0;
        _mouseRightOriginY=0;
    });
}

void control::bindNodeWithCameraMove(flyEngine::node* nodeObj){
    _bindNodeWithCameraMove=nodeObj;
    nodeObj->setPosition(_camera->getPosition());
    nodeObj->setPositionZ(_camera->getPositionZ()+_cameraMoveNodeZ);
}

void control::regOnKeyPress(char key, std::function<void ()> cb){
    _kbEventObj->regEvent(key,cb);
}
