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
    pointLight(glm::vec3 color,material* mt,float constant=1,float linear=0.09,float quadratic=0.032);
    pointLight(glm::vec3 color,glm::vec3 pos,float am,float diff,float spe,int shiness);
    void glUpdate(int program_id, int light_index);
    void glUpdateUseMaterail(int program_id,int light_index);
    void setAttenuationParam(float constant,float linear,float quadratic){
        m_fConstant=constant;
        m_fLinear=linear;
        m_fQuadratic=quadratic;
    };
    
private:
    float m_fConstant;
    float m_fLinear;
    float m_fQuadratic;
    
    float m_fAmbient;
    float m_fDiffuse;
    float m_fSpecular;
    float m_IntShiness;
};

NS_FLYENGINE_END

#endif /* pointLight_h */
