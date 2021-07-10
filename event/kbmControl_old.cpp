//
//  kbmControl.cpp
//  flyEngine
//
//  Created by joe on 11/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "kbmControl.h"
#include "flyEngine.h"

kbmControl::kbmControl(int shaderID,
                    string matNameModel,glm::mat4 matModel,
                    string matNameView,glm::mat4 matView,
                    string matNameProj,glm::mat4 matProj){
    _shaderID=shaderID;
    
    _matModel=matModel;
    _matModelOrigin=matModel;
    _matNameModel=matNameModel;
    
    _matView=matView;
    _matViewOrigin=matView;
    _matNameView=matNameView;
    
    _matProj=matProj;
    _matProjOrigin=matProj;
    _matNameProj=matNameProj;
    
    keyboardEvent* kbEventObj=new keyboardEvent();
    kbEventObj->regEvent('w', [&](){
        _matProj=glm::translate(_matProj,glm::vec3(0,0,_move_d));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameProj.c_str()), 1,GL_FALSE,glm::value_ptr(_matProj));
    });
    
    kbEventObj->regEvent('s', [&](){
        _matProj=glm::translate(_matProj,glm::vec3(0,0,-_move_d));
       glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameProj.c_str()), 1,GL_FALSE,glm::value_ptr(_matProj));
    });
    kbEventObj->regEvent('a', [&](){
        _matProj=glm::translate(_matProj,glm::vec3(-_move_d,0,0));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameProj.c_str()), 1,GL_FALSE,glm::value_ptr(_matProj));
      });
    kbEventObj->regEvent('d', [&](){
        _matProj=glm::translate(_matProj,glm::vec3(_move_d,0,0));
       glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameProj.c_str()), 1,GL_FALSE,glm::value_ptr(_matProj));
    });
    kbEventObj->regEvent('z', [&](){
        _matProj=glm::translate(_matProj,glm::vec3(0,_move_d,0));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameProj.c_str()), 1,GL_FALSE,glm::value_ptr(_matProj));
    });
    kbEventObj->regEvent('x', [&](){
        _matProj=glm::translate(_matProj,glm::vec3(0,-_move_d,0));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameProj.c_str()), 1,GL_FALSE,glm::value_ptr(_matProj));
    });

    kbEventObj->regEvent('r', [&](){
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameModel.c_str()), 1,GL_FALSE,glm::value_ptr(_matModelOrigin));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameView.c_str()), 1,GL_FALSE,glm::value_ptr(_matViewOrigin));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameProj.c_str()), 1,GL_FALSE,glm::value_ptr(_matProjOrigin));
    });
    char szBuf[512]={0};
    snprintf(szBuf,sizeof(szBuf),"kbmControl_%d_%s",_shaderID,_matNameProj.c_str());
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
    //            cout<<"on mouse left move!"<<x<<" "<<y<<" rotate "<<rotateX<<" "<<rotateY<<endl;
        _matView=glm::rotate(_matView,glm::radians(rotateX),glm::vec3(0,1,0));
        _matView=glm::rotate(_matView,glm::radians(rotateY),glm::vec3(1,0,0));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID,_matNameView.c_str()), 1,GL_FALSE,glm::value_ptr(_matView));

    });
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
//        cout<<"on mouse right move!"<<x<<" "<<y<<" rotate "<<rotateX<<" "<<rotateY<<endl;
        _matModel=glm::rotate(_matModel,glm::radians(rotateX),glm::vec3(0,1,0));
        _matModel=glm::rotate(_matModel,glm::radians(rotateY),glm::vec3(1,0,0));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameModel.c_str()), 1,GL_FALSE,glm::value_ptr(_matModel));

    });
    msEventObj->regOnMoveWithMiddleClick([&](float x,float y){
        cout<<"on mouse middle move!"<<x<<" "<<y<<endl;
    });
    
    msEventObj->regOnScroll([&](float x,float y){
        cout<<"on mouse scroll!"<<x<<" "<<y<<endl;
        float move=y*0.1;
        _matProj=glm::translate(_matProj,glm::vec3(0,0,move));
        glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameProj.c_str()), 1,GL_FALSE,glm::value_ptr(_matProj));
    });
    
    mouseEventMgr::addEvent("test3d_move_camera", msEventObj);
}
