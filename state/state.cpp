//
//  state.cpp
//  flyEngine
//
//  Created by Joe on 2022/7/14.
//  Copyright © 2022 joe. All rights reserved.
//

#include "state.h"
#include "timeUtil.h"
#include "uiText.h"
#include "world.h"

USE_NS_FLYENGINE;
void state::setShowFrameRate(bool s){
    s_bShowFrameRate=s;
    if(!s && s_textRate!=NULL)
        s_textRate->setVisible(false);
    else if(s && s_textRate!=NULL)
        s_textRate->setVisible(true);
};
void state::setShowDrawCall(bool s){
    s_bShowDrawCall=s;
    if(!s && s_textDrawCall!=NULL)
        s_textDrawCall->setVisible(false);
    else if(s && s_textDrawCall!=NULL)
        s_textDrawCall->setVisible(true);
};
void state::setShowVertices(bool s){
    s_bShowVertices=s;
    if(!s && s_textVertices!=NULL)
        s_textVertices->setVisible(false);
    else if(s && s_textVertices!=NULL)
        s_textVertices->setVisible(true);
};

bool state::isShowFrameRate(){return s_bShowFrameRate;};
bool state::isShowDrawCall(){return s_bShowDrawCall;};
bool state::isShowVertices(){return s_bShowVertices;};

void state::displayFrameRate(){
    if(s_textRate==NULL){
        s_textRate=new uiText("res/font/arial.ttf",20,"");
        s_textRate->setPosition(glm::vec3(3,4,0));
        s_textRate->setLogInStatebool(false);
//        world::getInstance()->addChild(s_textRate);
    }
    long long time=timeUtil::getTimeMS();
    if(s_lasttime==0){
        s_lasttime=time;
        return;
    }
    long long diff=time-s_lasttime;
    s_lasttime=time;
    //在diff时长内，渲染了一帧，则每秒可渲染1/diff
    float rate=1000.0/(float)diff;
    s_strRate="frame:"+std::to_string(rate);
    s_textRate->setText(s_strRate.c_str());
    s_textRate->draw();
}

void state::displayDrawCall(){
    if(s_textDrawCall==NULL){
        s_textDrawCall=new uiText("res/font/arial.ttf",20,"");
        s_textDrawCall->setPosition(glm::vec3(3,24,0));
        s_textDrawCall->setLogInStatebool(false);
//        world::getInstance()->addChild(s_textDrawCall);
    }
    s_strDrawCall="draw_call:"+std::to_string(s_draw_call);
    s_textDrawCall->setText(s_strDrawCall.c_str());
    s_textDrawCall->draw();
}

void state::displayVertices(){
    if(s_textVertices==NULL){
        s_textVertices=new uiText("res/font/arial.ttf",20,"");
        s_textVertices->setPosition(glm::vec3(3,44,0));
        s_textVertices->setLogInStatebool(false);
//        world::getInstance()->addChild(s_textVertices);
    }
    s_strVertices="vertices:"+std::to_string(s_vertices);
    s_textVertices->setText(s_strVertices.c_str());
    s_textVertices->draw();
}

void state::reset(){
    s_draw_call=0;
    s_vertices=0;
};

void state::incDrawCall(){s_draw_call++;};
void state::addVertices(int v){s_vertices+=v;};

void state::log(int v){
    s_draw_call++;
    s_vertices+=v;
};
