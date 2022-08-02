//
//  directionLight.h
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef directionLight_h
#define directionLight_h

#include <stdio.h>
#include "node.h"

NS_FLYENGINE_BEGIN

class directionLight {
    
public:
    directionLight(glm::vec3 color=glm::vec3(1.0,1.0,1.0),glm::vec3 dir=glm::vec3(-0.2,-1,-0.3),
                                  float am=1.0f,float diff=0.1,float spec=0,int shine=0);
    ~directionLight();
    void glUpdate(int program_id);
private:
    glm::vec3 m_vec3Dir;
    glm::vec3 m_vec3Color;
    float m_fAmbient;
    float m_fDiffuse;
    float m_fSpecular;
    int m_IntShiness;
    int _ubo;
};

NS_FLYENGINE_END

#endif /* directionLight_h */
