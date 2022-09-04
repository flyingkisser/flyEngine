//
//  cubeColor.h
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef cubeColor_h
#define cubeColor_h

#include <stdio.h>
#include <string>
#include <functional>

#include "defines.h"
#include "node.h"

using namespace std;

NS_FLYENGINE_BEGIN

class cubeColor: public node
{
private:
    glm::vec3 m_vec3Color;
    bool _dirtyColor=false;
    
public:
    cubeColor(glm::vec3 color);
    ~cubeColor(){};
    
    glm::vec3 getColor(){return m_vec3Color;};
    
    bool init();
    void glInit();
    void setColor(glm::vec3 color){
        m_vec3Color=color;
        _dirtyColor=true;
        setDirtyUBO(true);
    };
    
    bool isDirtyColor(){return _dirtyColor;};
    void setDirtyColor(bool v){_dirtyColor=v;};
    
    void draw();
};

NS_FLYENGINE_END

#endif /* cubeColor_h */
