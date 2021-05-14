//
//  keyboardEvent_h.h
//  flyEngine
//
//  Created by joe on 19/10/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef keyboardEvent_h
#define keyboardEvent_h

#include <stdio.h>
#include <map>
#include <tuple>
#include "defines.h"

class keyboardEvent{
private:
    std::map<char,std::function<void(void)>> _cbMap;
public:
    keyboardEvent();
    ~keyboardEvent(){_cbMap.clear();};
    void regEvent(char key,std::function<void(void)>);
    void onKeyPress(char key);
};


#endif /* keyboardEvent_h */
