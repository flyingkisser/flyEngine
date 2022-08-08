//
//  material.cpp
//  flyEngine
//
//  Created by joe on 11/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "material.h"
#include "texture.h"
#include "logUtil.h"
#include "shader.h"
#include "shaderMgr.h"

USE_NS_FLYENGINE;

material::material(glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,float shininess){
    m_vec3Ambient=ambient;
    m_vec3Diffuse=diffuse;
    m_vec3Specular=specular;
    m_fShininess=shininess;
    m_texSpecular=NULL;
}

bool material::setDiffuseTex(const char* szTex){
    texture* m_texDiffuse=new texture(szTex);
    if(!m_texDiffuse->init())
        return false;
    m_texDiffuse->glInit();
    return true;
}

bool material::setSpecularTex(const char* szTex){
    m_texSpecular=new texture(szTex);
    if(!m_texSpecular->init())
        return false;
    m_texSpecular->glInit(GL_TEXTURE1);
    return true;
}
