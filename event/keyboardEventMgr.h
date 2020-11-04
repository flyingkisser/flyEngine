//
//  keyboardEventMgr.h
//  flyEngine
//
//  Created by joe on 20/10/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef keyboardEventMgr_h
#define keyboardEventMgr_h

#include <stdio.h>
#include <map>
#include <string>

#include "defines.h"
#include "keyboardEvent.h"

using namespace std;

class keyboardEventMgr{
public:
    static void init(GLFWwindow* window);
    static void addEvent(std::string name,keyboardEvent* eventObj);
    static void printInfo();
};

#endif

/* keyboardEventMgr_h */

