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
class texture;

class material {
private:
    glm::vec3 m_vec3Ambient;
    glm::vec3 m_vec3Diffuse;
    glm::vec3 m_vec3Specular;
    float m_fShininess;
    
    texture* m_texDiffuse;
    texture* m_texSpecular;
    
public:
    //shininess越大，则物体反射走的光越多，进入人眼的光越少，光强越弱
    material(glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,float shininess);
    glm::vec3 getAmbient(){return m_vec3Ambient;};
    glm::vec3 getDiffuse(){return m_vec3Diffuse;};
    glm::vec3 getSpecular(){return m_vec3Specular;};
    float getShininess(){return m_fShininess;};
    bool setDiffuseTex(const char* szTex);
    bool setSpecularTex(const char* szTex);
    void glUpdateForCube(int gl_program);
    void glUpdateForPointLight(int gl_program,int i);
    void glUpdateForSpotLight(int gl_program,int i);
};

NS_FLYENGINE_END

#endif /* material_h */
