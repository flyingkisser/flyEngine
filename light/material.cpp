//
//  material.cpp
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "material.h"

USE_NS_FLYENGINE;

material::material(glm::vec4 ambient,glm::vec4 diffuse,glm::vec4 specular,float shininess){
      m_vec4Ambient=ambient;
      m_vec4Diffuse=diffuse;
      m_vec4Specular=specular;
      m_fShininess=shininess;
  };
