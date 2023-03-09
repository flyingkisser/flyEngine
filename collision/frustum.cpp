//
//  frustum.cpp
//  flyEngine
//
//  Created by Joe on 2023/3/6.
//  Copyright © 2023 joe. All rights reserved.
//

#include <stdio.h>
#include "frustum.h"

//USE_NS_FLYENGINE

NS_FLYENGINE_BEGIN
frustum* createFrustumFromCamera(camera* cam, float aspect, float fovY,float zNear, float zFar){
    frustum* stFrustum=new frustum;
    const float halfVSide = zFar * tanf(fovY * .5f);
    const float halfHSide = halfVSide * aspect;
    glm::vec3 camFront=cam->getFront();
    glm::vec3 camUp=cam->getUp();
    glm::vec3 camPos=cam->getPosition();
    glm::vec3 camRight=cam->getRight();

    const glm::vec3 frontMultFar = zFar * camFront;

    stFrustum->near = { camPos + zNear * camFront, camFront,"near" };
    stFrustum->far = { camPos + frontMultFar, -camFront,"far"};
    stFrustum->right = { camPos,glm::cross(camUp,frontMultFar + camRight * halfHSide),"right" };
    stFrustum->left = { camPos, glm::cross(frontMultFar - camRight * halfHSide,camUp),"left" };
    stFrustum->top = { camPos,glm::cross(camRight, frontMultFar - camUp * halfVSide),"top" };
    stFrustum->bottom = { camPos,glm::cross(frontMultFar + camUp * halfVSide, camRight),"bottom" };
    return stFrustum;
}
NS_FLYENGINE_END
