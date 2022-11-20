//
//  control.cpp
//  flyEngine
//
//  Created by joe on 11/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "control.h"
#include "camera.h"
#include "randUtil.h"
#include "logUtil.h"
#include "node.h"

#ifdef BUILD_MAC
#include "keyboardEvent.h"
#include "keyboardEventMgr.h"
#include "mouseEvent.h"
#include "mouseEventMgr.h"
#include "nodeIns.h"
#endif

#ifdef BUILD_IOS
#include "fingerEvent.h"
#endif



using namespace flyEngine;


void control::bindCamera(flyEngine::camera* c){
    _camera=c;
    char szBuf[512]={0};
    char szRand[32]={0};
    
    randUtil::getRandStr(6, szRand);
    snprintf(szBuf,sizeof(szBuf),"event_control_%s",szRand);
    
#ifdef BUILD_IOS
    _fingerEventObj=fingerEvent::getInstance();
    _fingerEventObj->regOnZoom([&](float scale){
        if(scale>1)
            scale=1;
        else
            scale=-1;
        _camera->setPositionZ(_camera->getPositionZ()-_move_d*scale/2);
        if(_bindNodeWithCameraMove!=NULL){
            _bindNodeWithCameraMove->setPositionZ(_camera->getPositionZ()+_cameraMoveNodeZ);
        }
    });
    _fingerEventObj->regOnMove([&](float x,float y){
        int isRight=x>0?1:-1;
        int isUp=y>0?-1:1;
        if(x!=0){
            _camera->setPositionX(_camera->getPositionX()+_move_d*isRight/3);
            if(_bindNodeWithCameraMove!=NULL){
                _bindNodeWithCameraMove->setPositionX(_camera->getPositionX());
            }
        }
        if(y!=0){
            _camera->setPositionY(_camera->getPositionY()+_move_d*isUp/3);
            if(_bindNodeWithCameraMove!=NULL){
             _bindNodeWithCameraMove->setPositionY(_camera->getPositionY());
            }
        }
    });
    fingerEvent::getInstance()->regOnTap([&](int c){
        if(c==2)
            _camera->reset();
    });
  
#endif
    
#ifdef BUILD_MAC
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
        _camera->setYaw(_yaw);
        _camera->setPitch(_pitch);
//        glm::vec3 posFront;
//        posFront.x=cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
//        posFront.y=sin(glm::radians(_pitch));
//        posFront.z=sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
//        _camera->setPositionFront(glm::normalize(posFront));
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
#endif
    flylog("control::bindCamera finished!");
}

static bool s_bNodeBind;
void control::bindNode(flyEngine::node* nodeObj){
    _vecBindNode.push_back(nodeObj);
    if(s_bNodeBind)
        return;
    s_bNodeBind=true;
#ifdef BUILD_MAC
    if(_msEventObj==NULL)
        return;
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
        for(auto node:_vecBindNode){
            nodeIns* nodeInstanced=dynamic_cast<nodeIns*>(node);
            if(nodeInstanced==NULL)
                node->rotateBy(glm::vec3(rotateY,rotateX,0.0f));
            else
                nodeInstanced->rotateBy(glm::vec3(rotateY,rotateX,0.0f));

        }
            
    });
    _msEventObj->regOnRightClickRelease([&](){
        _mouseRightOriginX=0;
        _mouseRightOriginY=0;
    });
#endif
    
#ifdef BUILD_IOS
    fingerEvent::getInstance()->regOnRotate([&](float r){
        for(auto node:_vecBindNode){
            if(r>0)
                node->rotateBy(glm::vec3(0,0,r));
            else
                node->rotateBy(glm::vec3(0,r,0));
        }
    });
#endif
}

void control::bindNodeWithCameraMove(flyEngine::node* nodeObj){
    _bindNodeWithCameraMove=nodeObj;
    nodeObj->setPosition(_camera->getPosition());
    nodeObj->setPositionZ(_camera->getPositionZ()+_cameraMoveNodeZ);
}

void control::regOnKeyPress(char key, std::function<void ()> cb){
#ifdef BUILD_MAC
    _kbEventObj->regEvent(key,cb);
#endif
}
