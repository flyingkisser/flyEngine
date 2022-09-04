//
//  material2.cpp
//  flyEngine
//
//  Created by joe on 04/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "material2.h"
#include "texture2.h"
#include "logUtil.h"
#include "shader.h"
#include "shaderMgr.h"

USE_NS_FLYENGINE

material2::material2(glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,float shininess){
    m_vec3Ambient=ambient;
    m_vec3Diffuse=diffuse;
    m_vec3Specular=specular;
    m_fShininess=shininess;
    m_texSpecular=NULL;
    m_texDiffuse=NULL;
}

bool material2::setDiffuseTex(const char* szTex){
    texture2* m_texDiffuse=new texture2(szTex);
    if(!m_texDiffuse->init())
       return false;
    m_texDiffuse->glInit();
    return true;
}

bool material2::setSpecularTex(const char* szTex){
    m_texSpecular=new texture2(szTex);
    if(!m_texSpecular->init())
       return false;
    m_texSpecular->glInit(GL_TEXTURE1);
    return true;
}
