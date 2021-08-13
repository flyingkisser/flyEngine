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
    glm::vec4 m_vec4Color;

public:
    cubeColor(glm::vec4 color);
    ~cubeColor(){};
    
    glm::vec4 getColor(){return m_vec4Color;};
    
    bool init();
    void glInit();

    void draw(camera* cameraObj);
};

NS_FLYENGINE_END

#endif /* cubeColor_h */
