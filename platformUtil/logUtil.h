//
//  logUtil.h
//  flyEngine
//
//  Created by joe on 29/06/2021.
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

void flylogErrno(const char* fmt,...);
void flylog(const char* szFmt,...);
void flylogNoTime(const char* fmt,...);
void flylogVec3Float(glm::vec3 v);
void flylogVec3Int(glm::vec3 v);
void flylogMat4(glm::mat4 v);
void flylogMat4(const char* szPrefix,glm::mat4 v);

#endif /* logUtil_h */
