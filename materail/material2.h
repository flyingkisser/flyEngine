//
//  material2.h
//  flyEngine
//
//  Created by joe on 04/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef material2_h
#define material2_h

#include <stdio.h>
#include "defines.h"

NS_FLYENGINE_BEGIN

class texture2;

class material2 {
private:
    glm::vec3 m_vec3Ambient;
    glm::vec3 m_vec3Diffuse;
    glm::vec3 m_vec3Specular;
    float m_fShininess;
    
    texture2* m_texDiffuse;
    texture2* m_texSpecular;
    
public:
    //shininess越大，则物体反射走的光越多，进入人眼的光越少，光强越弱
    material2(glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,float shininess);
    glm::vec3 getAmbient(){return m_vec3Ambient;};
    glm::vec3 getDiffuse(){return m_vec3Diffuse;};
    glm::vec3 getSpecular(){return m_vec3Specular;};
    texture2* getTexSpecular(){return m_texSpecular;};
    texture2* getTexDiffuse(){return m_texDiffuse;};
    float getShininess(){return m_fShininess;};
    bool setDiffuseTex(const char* szTex);
    bool setSpecularTex(const char* szTex);
    void glUpdateForCube(int gl_program);
};

NS_FLYENGINE_END

#endif /* material2_h */
