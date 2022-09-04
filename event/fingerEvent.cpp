//
//  fingerEvent.cpp
//  flyEngine_ios
//
//  Created by joe on 19/08/2022.
//

#include "fingerEvent.h"
USE_NS_FLYENGINE
static fingerEvent* s_instance=NULL;

fingerEvent* fingerEvent::getInstance(){
    if(s_instance==NULL)
        s_instance=new fingerEvent();
    return s_instance;
}

void fingerEvent::regOnZoom(std::function<void(float scale)> cb){
    _vecCBOnZoom.push_back(cb);
}
void fingerEvent::regOnMove(std::function<void(float x,float y)> cb){
    _vecCBOnMove.push_back(cb);
}
void fingerEvent::regOnRotate(std::function<void(float r)> cb){
    _vecCBOnRotate.push_back(cb);
}
void fingerEvent::regOnTap(std::function<void(int c)> cb){
    _vecCBOnTap.push_back(cb);
}

void fingerEvent::onZoom(float scale){
    for(auto cb:_vecCBOnZoom)
        cb(scale);
}
void fingerEvent::onRotate(float r){
    for(auto cb:_vecCBOnRotate)
        cb(r);
}
void fingerEvent::onMove(float x,float y){
    for(auto cb:_vecCBOnMove)
        cb(x,y);
}
void fingerEvent::onTap(int c){
    for(auto cb:_vecCBOnTap)
        cb(c);
}
