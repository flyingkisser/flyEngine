//
//  keyboardEvent.cpp
//  flyEngine
//
//  Created by joe on 19/10/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "keyboardEvent.h"
#include "logUtil.h"

USE_NS_FLYENGINE

keyboardEvent::keyboardEvent(){
    
}

void keyboardEvent::regEvent(char key, std::function<void ()> cb){
    flylog("keyboardEvent.regEvent: %c reged!",key);
    _cbMap[key]=cb;
}

void keyboardEvent::onKeyPress(char key){
    for(auto &it:_cbMap){
        if(it.first==key){
            flylog("keyboardEvent.onKeyPress:call %c",key);
            it.second();
            return;
        }
    }
}
