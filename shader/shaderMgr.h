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
    static shader* get3dColorShader();
    static shader* getDefaultShader();
    static shader* get3d1texShader();
    static shader* get3d2texShader();
    static shader* get3d1texPongShader();
    
    static shader* get3d1texInsPongShader();
    static shader* get3d1texInsByVBOPongShader();
    
    static shader* get3d1texPongWithSpecularTexShader();
    static shader* getModelShader();
    static shader* getModelInsShader();
    static shader* getModelInsByVBOShader();
    
    static shader* get2d1texShader();
    
    static shader* getShader(const char* szVertFileName,const char* szFragFileName);
    static shader* getShader(int programID);
    static shader* getShaderUniqueue(const char* szVertFileName,const char* szFragFileName);
    
    static shader* createAndCacheShader(const char* szVertFileName,const char* szFragFileName);
    
    static void setBool(unsigned int idProgram,const char *name, bool v);
    static void setInt(unsigned int idProgram,const char *name, int v);
    static void setFloat(unsigned int idProgram,const char *name, float v);
    static void setMat4(unsigned int idProgram,const char *name, float* v);
    static void setVec3(unsigned int idProgram,const char *name, float* v);

    
};

NS_FLYENGINE_END


#endif /* shaderMgr_h */
