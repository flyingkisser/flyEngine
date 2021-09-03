//
//  ambientLight.h
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef ambientLight_h
#define ambientLight_h

#include <stdio.h>
#include "light.h"

NS_FLYENGINE_BEGIN

class ambientLight: public light{
    
public:
    ambientLight(glm::vec3 color);
    ~ambientLight();
    
    bool init();
    void glUpdate(int program_id);

    void draw(camera* cameraObj);
private:
    glm::vec3 m_vec3Direction;
};

NS_FLYENGINE_END

#endif /* ambientLight_h */
