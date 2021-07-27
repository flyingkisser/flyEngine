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
#include "defines.h"
#include "shader.h"


NS_FLYENGINE_BEGIN

class shaderMgr{
public:
    static void initDefaultShader();
    static unsigned int createShaderFromFile(const char* szVertFileName,const char* szFragFileName);
    static void useDefaultShader();
    static shader* getDefaultShader();
    static shader* get3d1texShader();
    static shader* get3d2texShader();
    
    static shader* getShader(const char* szVertFileName,const char* szFragFileName);
    static shader* getShaderUniqueue(const char* szVertFileName,const char* szFragFileName);
};

NS_FLYENGINE_END


#endif /* shaderMgr_h */
