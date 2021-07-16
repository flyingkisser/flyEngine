//
//  Node.cpp
//  flyEngine
//
//  Created by joe on 24/06/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "node.h"
#include "shaderMgr.h"
#include "defines.h"
using namespace flyEngine;


void node::updateModel(){
    glUniformMatrix4fv(glGetUniformLocation(_gl_program,"matModel"), 1,GL_FALSE,glm::value_ptr(_matModel));
}

void node::print(){
    flylog("node:pos %f %f %f",_pos.x,_pos.y,_pos.z);
}

node::node(const char* texPath){
    _texPath=texPath;
}

bool node::init(){
    _texObj=textureMgr::getInstance()->getTexture(_texPath);
    if(_texObj==nullptr)
        return false;
    _shaderObj=shaderMgr::get3d1texShader();
    if(_shaderObj==nullptr)
       return false;
    return true;
}

void node::moveBy(glm::vec3 v){
    _pos.x+=v.x;
    _pos.y+=v.y;
    _pos.z+=v.z;
    _dirtyPos=true;
    //    _matModel=glm::translate(_matModel, v);
}
void node::scale(glm::vec3 v){
    _scale=v;
    _dirtyPos=true;
//  _matModel=glm::scale(_matModel, v);
}

//v里面是旋转的角度，0到360，函数会转成弧度
void node::rotate(glm::vec3 v){
    _rorate=v;
    _dirtyPos=true;
//    if(v.x)
//        _matModel=glm::rotate(_matModel,glm::radians(v.x),glm::vec3(1,0,0));
//    if(v.y)
//          _matModel=glm::rotate(_matModel,glm::radians(v.y),glm::vec3(0,1,0));
//    if(v.z)
//          _matModel=glm::rotate(_matModel,glm::radians(v.z),glm::vec3(0,0,1));
}

void node::setPosition(glm::vec3 p){
    _pos=p;
    _dirtyPos=true;
    //    _pos.x/=g_winWidth;
    //    _pos.y/=g_winHeight;
    //    _matModel=glm::translate(_matModel,_pos);
};

void node::setPositionX(float v){
    _pos.x=v;
//    _matModel=glm::translate(_matModel,glm::vec3(v,0,0));
    _dirtyPos=true;
};

void node::setPositionY(float v){
    _pos.y=v;
//    _matModel=glm::translate(_matModel,glm::vec3(0,v,0));
    _dirtyPos=true;
};

void node::setPositionZ(float v){
    _pos.z=v;
//    _matModel=glm::translate(_matModel,glm::vec3(0,0,v));
    _dirtyPos=true;
};


void node::glInit(){
    _texObj->glInit();
    _gl_texture0=_texObj->getTextureID();
    _shaderObj->glInit();
    _gl_program=_shaderObj->getProgramID();
    if(!_gl_texture0){
        flylog("node::glInit: _gl_texture0 is 0,error!");
        return;
    }
    if(!_gl_program){
        flylog("node::glInit: _gl_program is 0,error!");
        return;
    }
    
    glRef::glInit();
    unsigned int vbo;
    float* verticeArr=g_verticeArr;
    int verticeArrSize=sizeof(g_verticeArr);
    int numPerVertex=3;  //每个顶点坐标用几个浮点数来表示
    int numPerTex=2;     //每个纹理坐标用几个浮点数来表示
    int stride=5*sizeof(float);
    int offsetVertex=0;   //顶点坐标在数组每小块中的偏移
//    _pos=glm::vec3(0,0,-5);
    
    //生成VAO
    glGenVertexArrays(1,&_gl_vao);
    glBindVertexArray(_gl_vao);
    //生成VBO
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    //顶点数据写进显存
    glBufferData(GL_ARRAY_BUFFER,verticeArrSize,verticeArr,GL_STATIC_DRAW);
    //设置顶点数组的属性
    glVertexAttribPointer(0,numPerVertex,GL_FLOAT,GL_FALSE,stride,(void*)0);
    glVertexAttribPointer(1,numPerTex,GL_FLOAT,GL_FALSE,stride,(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //解除当前VBO绑定
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //解除当前VAO绑定
    glBindVertexArray(0);
    
    glUniform1i(glGetUniformLocation(_gl_program, "texture0"), _gl_texture0);
    
    setPosition(glm::vec3(0,0,-10));
    rotate(glm::vec3(30,0,30));

    _dirtyPos=true;
}

void node::draw(camera* cameraObj){
    _shaderObj->use();
    
    if(_dirtyPos){
        _matModel=glm::mat4(1.0f);
        if(_pos.x || _pos.y || _pos.z)
            _matModel=glm::translate(_matModel,_pos);
        if(_rorate.x)
            _matModel=glm::rotate(_matModel,glm::radians(_rorate.x),glm::vec3(1,0,0));
        if(_rorate.y)
            _matModel=glm::rotate(_matModel,glm::radians(_rorate.y),glm::vec3(0,1,0));
        if(_rorate.z)
            _matModel=glm::rotate(_matModel,glm::radians(_rorate.z),glm::vec3(0,0,1));
        if(_scale.x || _scale.y || _scale.z)
            _matModel=glm::scale(_matModel,_scale);
      
        glUniformMatrix4fv(glGetUniformLocation(_gl_program,"matModel"), 1,GL_FALSE,glm::value_ptr(_matModel));
        _dirtyPos=false;
        logUtil::logMat4(_matModel);
    }
    cameraObj->update(_gl_program);
        
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,_gl_texture0);
    glBindVertexArray(_gl_vao);
    glDrawArrays(GL_TRIANGLES,0,36);
}
