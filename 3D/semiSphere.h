//
//  semiSphere.h
//  flyEngine
//
//  Created by joe on 16/12/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef semiSphere_h
#define semiSphere_h

#include <stdio.h>

#include <stdio.h>
#include <string>
#include <functional>

#include "defines.h"
#include "node.h"

using namespace std;

NS_FLYENGINE_BEGIN

class semiSphere: public node
{
private:
    glm::vec3 m_vec3Color;
    bool _dirtyColor=false;
    unsigned int _gl_ebo=0;
    unsigned int _index_size=0;
    
public:
    semiSphere(glm::vec3 color);
    ~semiSphere(){};
    
    glm::vec3 getColor(){return m_vec3Color;};
    
    bool init();
    
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

#endif /* semiSphere_h */
