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
#include "glRef.h"

namespace flyEngine {
//class glRef;
class shader : glRef{
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
    void setBool(const char* name,bool v);
    void setInt(const char* name,int v);
    void setFloat(const char* name,float v);
    unsigned int getProgramID(){return _idProgram;};

    void glInit();
    bool init();
};
 
}


#endif /* shader_h */
