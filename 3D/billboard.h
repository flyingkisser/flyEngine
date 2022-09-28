//
//  billboard.h
//  flyEngine
//
//  Created by joe on 21/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef billboard_h
#define billboard_h

#include <stdio.h>

#include <stdio.h>
#include <string>
#include <functional>

#include "defines.h"
#include "node.h"

using namespace std;

NS_FLYENGINE_BEGIN

class billboard: public node
{
private:
    glm::vec3 m_vec3Color;
    bool _dirtyColor=false;
    
public:
    billboard(glm::vec3 color);
    ~billboard(){};
    
    glm::vec3 getColor(){return m_vec3Color;};
    
//    void setPosition(glm::vec3 pos);
    
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

#endif /* billboard_h */
