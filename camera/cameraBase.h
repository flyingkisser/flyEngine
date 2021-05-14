//
//  kbmCameraBase.h
//  flyEngine
//
//  Created by joe on 18/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef cameraBase_h
#define cameraBase_h

#include <stdio.h>

#include "camera.h"

class cameraBase :  public camera {
    
public:
    cameraBase(int shader,
           string matNameModel,glm::mat4 matModel,
           string matNameView,glm::vec3 cameraPos,glm::vec3 cameraUp,
           string matNameProj,float screenRatio);
};


#endif /* cameraBase_h */
