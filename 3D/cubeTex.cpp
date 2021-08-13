//
//  cubeTex.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/8/1.
//  Copyright © 2021 joe. All rights reserved.
//

#include "cubeTex.h"

#include "logUtil.h"
#include "texture.h"
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "action.h"
#include "camera.h"
#include "world.h"
#include "ambientLight.h"
#include "material.h"

USE_NS_FLYENGINE

cubeTex::cubeTex(const char* texPath){
    _texPath=texPath;
}

bool cubeTex::init(){
    _texObj=textureMgr::getInstance()->getTexture(_texPath);
    if(_texObj==NULL)
        return false;
    _shaderObj=shaderMgr::get3d1texPongShader();
    if(_shaderObj==NULL)
       return false;
    glInit();
    return true;
}

void cubeTex::glInit(){
    node::glInitShader();
    node::glInitVAOWithTexCoordAndNormal();
//    node::glInitVAOWithTexCoord();
    
    _texObj->glInit();
    _gl_texture0=_texObj->getTextureID();
    if(!_gl_texture0){
      flylog("cube::glInit: _gl_texture0 is 0,error!");
      return;
    }
    glUniform1i(glGetUniformLocation(_gl_program, "texture0"), _gl_texture0);

    node::setPosition(glm::vec3(0,0,-10));
    node::rotateBy(glm::vec3(30,0,30));
}

void cubeTex::draw(camera* cameraObj){
    node::updateModel(cameraObj);
    node::glInitLight();
    
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,_gl_texture0);
    glBindVertexArray(_gl_vao);
    glDrawArrays(GL_TRIANGLES,0,36);
}
