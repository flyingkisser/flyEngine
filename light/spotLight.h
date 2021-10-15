//
//  spotLight.h
//  flyEngine
//
//  Created by joe on 06/09/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef spotLight_h
#define spotLight_h

#include <stdio.h>
#include "defines.h"
#include "light.h"

NS_FLYENGINE_BEGIN

class spotLight:public light{
  
public:
    spotLight(glm::vec3 color,material* mt,float cutOffEngleInner=15,float cutOffEngleOuter=20);
    void glUpdateForCube(int program_id,int index,camera* c);
private:
    float m_fcufOffInner;
    float m_fcufOffOuter;
};

NS_FLYENGINE_END

#endif /* spotLight_h */
