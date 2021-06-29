//
//  shaderMgr.h
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/27.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef shaderMgr_h
#define shaderMgr_h

#include <stdio.h>
#include <string>
#include "flyEngine.h"
#inlcude "shader.h"

namespace flyEngine {
class shaderMgr{
public:
    static unsigned int createShaderWithoutCache(const char* szVertFileName,const char* szFragFileName);
    static unsigned int createShader(const char* szVertFileName,const char* szFragFileName);
    static void useShader(unsigned int id);
    static void useDefaultShader();
    static unsigned int getDefaultShader();
    static void setBool(unsigned int id,const char* name,bool v);
    static void setInt(unsigned int id,const char* name,int v);
    static void setFloat(unsigned int id,const char* name,float v);
};

}


#endif /* shaderMgr_h */
