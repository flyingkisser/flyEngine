//
//  plane.h
//  flyEngine
//
//  Created by joe on 27/02/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef plane_h
#define plane_h

#include "defines.h"

NS_FLYENGINE_BEGIN

struct plane{
    glm::vec3 normal={0.0f,1.0f,0.0f};
    float distance=0.0f;
    
    plane()=default;
    plane(glm::vec3 pos,glm::vec3 norm){
        normal=glm::normalize(norm);
        distance=glm::dot(normal,pos);
    }
    
    float getSignedDistanceToPlane(glm::vec3 pos){
        return glm::dot(normal,pos)-distance;
    }
};

NS_FLYENGINE_END

#endif /* plane_h */
