//
//  material.h
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef material_h
#define material_h

#include <stdio.h>
#include "defines.h"

NS_FLYENGINE_BEGIN

class material {
private:
    glm::vec4 m_vec4Ambient;
    glm::vec4 m_vec4Diffuse;
    glm::vec4 m_vec4Specular;
    float m_fShininess;
    
public:
    material(glm::vec4 ambient,glm::vec4 diffuse,glm::vec4 specular,float shininess);
    glm::vec4 getAmbient(){return m_vec4Ambient;};
    glm::vec4 getDiffuse(){return m_vec4Diffuse;};
    glm::vec4 getSpecular(){return m_vec4Specular;};
    float getShininess(){return m_fShininess;};
};

NS_FLYENGINE_END

#endif /* material_h */
