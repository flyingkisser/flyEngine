//
//  pointLight.h
//  flyEngine
//
//  Created by joe on 06/09/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef pointLight_h
#define pointLight_h

#include <stdio.h>
#include "defines.h"
#include "light.h"

NS_FLYENGINE_BEGIN

class pointLight : public light{
public:
    pointLight(glm::vec3 color,material2* mt,float constant=1,float linear=0.09,float quadratic=0.032);
    void glUpdate(int light_index);
    void update(int light_index);
    
private:
    float m_fConstant=0;
    float m_fLinear=0;
    float m_fQuadratic=0;
    float m_fRadius=0;
    
    int _ubo=0;
};

NS_FLYENGINE_END

#endif /* pointLight_h */
