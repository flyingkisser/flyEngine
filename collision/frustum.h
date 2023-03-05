//
//  frustum.h
//  flyEngine
//
//  Created by joe on 03/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef frustum_h
#define frustum_h


#include "defines.h"
#include "camera.h"
#include "plane.h"

NS_FLYENGINE_BEGIN

struct frustum{
    plane top;
    plane bottom;
    plane right;
    plane left;
    plane far;
    plane near;
};

frustum createFrustumFromCamera(camera* cam, float aspect, float fovY,float zNear, float zFar){
    frustum stFrustum;
    const float halfVSide = zFar * tanf(fovY * .5f);
    const float halfHSide = halfVSide * aspect;
    glm::vec3 camFront=cam->getFront();
    glm::vec3 camUp=cam->getUp();
    glm::vec3 camPos=cam->getPosition();
    glm::vec3 camRight=cam->getRight();
  
    const glm::vec3 frontMultFar = zFar * camFront;

    stFrustum.near = { camPos + zNear * camFront, camFront };
    stFrustum.far = { camPos + frontMultFar, -camFront};
    stFrustum.right = { camPos,glm::cross(frontMultFar - camRight * halfHSide, camUp) };
    stFrustum.left = { camPos, glm::cross(camUp,frontMultFar + camRight * halfHSide) };
    stFrustum.top = { camPos,glm::cross(camRight, frontMultFar - camUp * halfVSide) };
    stFrustum.bottom = { camPos,glm::cross(frontMultFar + camUp * halfVSide, camRight) };
    return stFrustum;
}

NS_FLYENGINE_END

#endif /* frustum_h */
