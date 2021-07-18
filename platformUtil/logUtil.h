//
//  logUtil.h
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/6.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef logUtil_h
#define logUtil_h

#include <stdio.h>
#include "defines.h"

class logUtil{
public:
    static void log(const char* szFmt,...);
    static void logMat4(glm::mat4 mat);
};

void flylog(const char* szFmt,...);

#endif /* logUtil_h */
