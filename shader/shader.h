//
//  shader.h
//  flyEngine
//
//  Created by joe on 10/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef shader_h
#define shader_h

#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "glRef.h"

NS_FLYENGINE_BEGIN

class shader : public glRef{
private:
    unsigned int _idProgram;
    char* _szVert;
    char* _szFrag;
    char* _szVertFileName;
    char* _szFragFileName;

public:
    shader(const char* szVertFileName,const char* szFragFileName);
    ~shader();
    bool isSuccess();
    void use();
    void setBool(const char* name,bool v,bool debug=true);
    void setInt(const char* name,int v,bool debug=true);
    void setFloat(const char* name,float v,bool debug=true);
    void setMat4(const char* name,float* v,bool debug=true);
    void setVec3(const char* name,float* v,bool debug=true);
        
    unsigned int getProgramID(){return _idProgram;};

    void compile();
    bool init();
};
 
NS_FLYENGINE_END


#endif /* shader_h */
