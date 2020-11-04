//
//  keyboardEvent.cpp
//  flyEngine
//
//  Created by joe on 19/10/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "keyboardEvent.h"


keyboardEvent::keyboardEvent(){
    
}

void keyboardEvent::regEvent(char key, std::function<void ()> cb){
    _cbMap[key]=cb;
}

void keyboardEvent::onKeyPress(char key){
    for(auto &it:_cbMap){
        if(it.first==key){
            it.second();
            return;
        }
    }
}
