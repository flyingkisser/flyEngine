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
//    glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameModel.c_str()), 1,GL_FALSE,glm::value_ptr(_matModel));
}

void node::print(){
    cout<<"Node: "<<endl;
}

node::node(const char* texPath){
    initTexture(texPath);
    initShader(shaderMgr::get3d1texShader());
    _glInit();
}

bool node::initTexture(const char* texPath){
    texture* texObj=textureMgr::getInstance()->getTexture(texPath);
    _gl_texture0=texObj->getTextureID();
    if(!_gl_texture0)
      return false;
}

void node::initShader(int id){
        _gl_program=id;
}
void node::initShader(const char* vsPath,const char* fsPath){
    _gl_program=shaderMgr::createShader(vsPath,fsPath);
}

void node::_glInit(){
    unsigned int vbo;
    float* verticeArr=g_verticeArr;
    int verticeArrSize=sizeof(g_verticeArr);
    int numPerVertex=3;  //每个顶点坐标用几个浮点数来表示
    int numPerTex=2;     //每个纹理坐标用几个浮点数来表示
    int stride=5*sizeof(float);
    int offsetVertex=0;   //顶点坐标在数组每小块中的偏移
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
    
    _matModel=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-3));
    _matModelOrigin=_matModel;
    glUniformMatrix4fv(glGetUniformLocation(_gl_program,"matModel"), 1,GL_FALSE,glm::value_ptr(_matModel));
}

void node::draw(camera* cameraObj){
    shaderMgr::useShader(_gl_program);
    
//    if(cameraObj)
//        cameraObj->updateCamera();
    
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,_gl_texture0);
    glBindVertexArray(_gl_vao);
    glDrawArrays(GL_TRIANGLES,0,36);
}

