//
//  shader.h
//  flyEngine
//
//  Created by joe on 10/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef shader_h
#define shader_h

#include "flyEngine.h"

namespace flyEngine {

class shader{
private:
    unsigned int _idProgram;
    
    
public:
    shader(const char* szVertFileName,const char* szFragFileName);
    bool isSuccess();
    void use();
    void setBool(const char* name,bool v);
    void setInt(const char* name,int v);
    void setFloat(const char* name,float v);
    
};

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


#endif /* shader_h */
