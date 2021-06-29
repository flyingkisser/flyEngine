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
    
}

bool node::init(const char* texPath){
    _gl_program=shaderMgr::getDefaultShader();
    texture* texObj=textureMgr::getInstance()->getTexture(texPath);
    _gl_texture0=texObj->getTextureID();
    if(!_gl_texture0)
        return false;
    glUniform1i(glGetUniformLocation(_gl_program, "texture0"), _gl_texture0);
    _matModel=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-3));
    _matModelOrigin=_matModel;
    glUniformMatrix4fv(glGetUniformLocation(_gl_program,"matView"), 1,GL_FALSE,glm::value_ptr(_matModel));

}

void node::_glInit(){
    unsigned int vbo;
    float* verticeArr=g_verticeArr;
    int verticeArrSize=sizeof(g_verticeArr);
    int numPerVertex=3;     //每个顶点坐标用几个浮点数来表示
    int stride=3;
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
    glVertexAttribPointer(0,numPerVertex,GL_FLOAT,GL_FALSE,stride,(void*)offsetVertex);
    glEnableVertexAttribArray(0);
    //解除当前VBO绑定
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //解除当前VAO绑定
    glBindVertexArray(0);
}

void node::draw(camera* cameraObj){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    shaderMgr::useShader(_gl_program);
    
    if(cameraObj)
        cameraObj->updateCamera();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,_gl_texture0);
    glBindVertexArray(_gl_vao);
    glDrawArrays(GL_TRIANGLES,0,36);
}

