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

frustum* createFrustumFromCamera(camera* cam, float aspect, float fovY,float zNear, float zFar);



NS_FLYENGINE_END

#endif /* frustum_h */
