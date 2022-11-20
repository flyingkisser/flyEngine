//
//  cubeTexIns.h
//  flyEngine
//
//  Created by joe on 21/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef cubeTexIns_h
#define cubeTexIns_h

#include <stdio.h>
#include <string>
#include <vector>
#include <functional>

#include "defines.h"
#include "nodeIns.h"

NS_FLYENGINE_BEGIN
class texture2;
class shader;

class cubeTexIns: public nodeIns
{
private:
    texture2* _texObj=NULL;
    const char* _texPath=NULL;
   
protected:
    unsigned int _gl_texture0=0;
public:
    cubeTexIns(const char* texPath,int count);
    cubeTexIns(unsigned int texID,int count);
    bool init();
    void useInstancedByVBO();
  
    
    void draw();
    void setPipelineValue();
    void drawCall();
   
};

NS_FLYENGINE_END

#endif /* cubeTexIns_h */
