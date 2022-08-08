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
    void setAttenuationParam(float constant,float linear,float quadratic){
        m_fConstant=constant;
        m_fLinear=linear;
        m_fQuadratic=quadratic;
    };
    void setCutOffOuter(float cutOffEngleOuter){
        m_fcutOffOuter=glm::cos(glm::radians(cutOffEngleOuter));
        m_fEngleOuter=cutOffEngleOuter;
    };
    void setCutOffInner(float cutOffEngleInner){
        m_fcutOffInner=glm::cos(glm::radians(cutOffEngleInner));
        m_fEngleInner=cutOffEngleInner;
    };
    float getCutOffOuter(){return m_fEngleOuter;};
    float getCutOffInner(){return m_fEngleInner;};


    
    void glUpdate(int light_index);
    void update(int light_index);
    glm::vec3 getDirection(){return _vec3Direction;};
    void setDirection(glm::vec3 d){_vec3Direction=d;};
    
    void setRotation(glm::vec3 v);
    void rotateBy(glm::vec3 v);
    
private:
    float m_fcutOffInner;
    float m_fcutOffOuter;
    float m_fEngleInner;
    float m_fEngleOuter;
    
    float m_fConstant=0;
    float m_fLinear=0;
    float m_fQuadratic=0;
    
    int _ubo=0;
    glm::vec3 _vec3Direction;
};

NS_FLYENGINE_END

#endif /* spotLight_h */
