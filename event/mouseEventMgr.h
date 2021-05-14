//
//  mouseEventMgr.h
//  flyEngine
//
//  Created by joe on 10/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef mouseEventMgr_h
#define mouseEventMgr_h

#include <stdio.h>
#include <map>
#include <string>

#include "defines.h"
#include "mouseEvent.h"

using namespace std;

class mouseEventMgr{
public:
    static void init(GLFWwindow* window);
    static void addEvent(std::string name,mouseEvent* eventObj);
    static void printInfo();
};

#endif /* mouseEventMgr_h */
