//
//  keyboardEvent.cpp
//  flyEngine
//
//  Created by joe on 19/10/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "keyboardEvent.h"
#include "logUtil.h"

keyboardEvent::keyboardEvent(){
    
}

void keyboardEvent::regEvent(char key, std::function<void ()> cb){
    log("keyboardEvent.regEvent: %c reged!",key);
    _cbMap[key]=cb;
}

void keyboardEvent::onKeyPress(char key){
    for(auto &it:_cbMap){
        if(it.first==key){
            log("keyboardEvent.onKeyPress:call %c",key);
            it.second();
            return;
        }
    }
}
