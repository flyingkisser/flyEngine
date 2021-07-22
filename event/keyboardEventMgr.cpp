//
//  keyboardEventMgr.cpp
//  flyEngine
//
//  Created by joe on 20/10/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "keyboardEventMgr.h"
#include "logUtil.h"
#include "keyboardEvent.h"

USE_NS_FLYENGINE

static map<std::string,keyboardEvent*> s_eventChainMap;

//static void s_onEvent(GLFWwindow* window, int key, int scancode, int action, int mods){
//    const char* keyName = glfwGetKeyName(key, scancode);
//    cout<<"onKey: s_eventChainMap size is "<<s_eventChainMap.size()<<endl;
//    for(auto it : s_eventChainMap){
//        it.second->onKeyPress((char)keyName[0]);
//    }
//};

void keyboardEventMgr::init(GLFWwindow* window){
    glfwSetKeyCallback(window,[](GLFWwindow* window, int key, int scancode, int action, int mods){
        const char* keyName = glfwGetKeyName(key, scancode);
        if(keyName==nullptr || !keyName[0] || action==0)
            return;
        for(auto it : s_eventChainMap){
            flylog("glfwSetKeyCallback: call onKeyPress for %c",char(keyName[0]));
            it.second->onKeyPress((char)keyName[0]);
        }
    });
};

void keyboardEventMgr::addEvent(std::string name,keyboardEvent* eventObj){
    s_eventChainMap.insert(pair<string,keyboardEvent*>(name,eventObj));
    flylog("keyboardEventMgr.addEvent:%s add!",name.c_str());
    cout<<"s_eventChainMap size is "<<s_eventChainMap.size()<<endl;
};

void keyboardEventMgr::printInfo(){
    cout<<"printInfo s_eventChainMap size is "<<s_eventChainMap.size()<<endl;
};
