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
    ambientLight(glm::vec4 color);
    ~ambientLight();
    
    bool init();
    void glInit();

    void draw(camera* cameraObj);
};

NS_FLYENGINE_END

#endif /* ambientLight_h */
