//
//  Node.cpp
//  flyEngine
//
//  Created by joe on 24/06/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "node.h"
#include "logUtil.h"
#include "texture.h"
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "action.h"
#include "camera.h"
#include "ambientLight.h"
#include "world.h"
#include "material.h"

USE_NS_FLYENGINE

void node::print(){
    flylog("node:pos %f %f %f",_pos.x,_pos.y,_pos.z);
}

node::node(){
}

//从当前位置，移动一个指定的距离
void node::moveBy(glm::vec3 v){
    _pos+=v;
    _dirtyPos=true;
}

//从当前大小，缩放一个指定的系数
void node::scaleBy(glm::vec3 v){
    _scale+=v;
    _dirtyPos=true;
}

//从当前位置，旋转一个指定的角度
//v里面是旋转的角度，0到360，函数会转成弧度
void node::rotateBy(glm::vec3 v){
    _rorate+=v;
    _dirtyPos=true;
}

//设置scale
void node::setScale(glm::vec3 v){
    _scale=v;
    _dirtyPos=true;
}

//设置坐标
void node::setPosition(glm::vec3 p){
    _pos=p;
    _dirtyPos=true;
};

void node::setPositionX(float v){
    _pos.x=v;
    _dirtyPos=true;
};

void node::setPositionY(float v){
    _pos.y=v;
    _dirtyPos=true;
};

void node::setPositionZ(float v){
    _pos.z=v;
    _dirtyPos=true;
};

//动画相关
void node::runAction(action* act){
    act->start(this);
}

void node::stopAction(action* act){
    act->stop();
}

//材质
void node::setMaterial(material *mt){
    m_material=mt;
}

void node::updateModel(camera* cameraObj){
    _shaderObj->use();
    if(_dirtyPos){
        _matModel=glm::translate(glm::mat4(1.0f),_pos);
        if(_rorate.x)//水平方向上旋转
          _matModel=glm::rotate(_matModel,glm::radians(_rorate.x),glm::vec3(0,1,0));
        if(_rorate.y)//垂直方向上旋转
          _matModel=glm::rotate(_matModel,glm::radians(_rorate.y),glm::vec3(1,0,0));
        if(_rorate.z)
          _matModel=glm::rotate(_matModel,glm::radians(_rorate.z),glm::vec3(0,0,1));
        _matModel=glm::scale(_matModel,_scale);
        _dirtyPos=false;
    }
    glUniformMatrix4fv(glGetUniformLocation(_gl_program,uniform_name_mat_model), 1,GL_FALSE,glm::value_ptr(_matModel));
    cameraObj->update(_gl_program);
}



void node::glInitShader(){
    _shaderObj->glInit();
    _gl_program=_shaderObj->getProgramID();
    if(!_gl_program){
        flylog("node::glInitShader: _gl_program is 0,error!");
        return;
    }
}

bool node::glInitMaterial(){
    if(m_material==NULL)
        return false;
    //环境光材质
    int pos=glGetUniformLocation(_gl_program,uniform_name_material_ambient);
    if(pos!=-1){
        glUniform4fv(pos,1,glm::value_ptr(m_material->getAmbient()));
    }
    //漫反射材质
    pos=glGetUniformLocation(_gl_program,uniform_name_material_diffuse);
    if(pos!=-1){
       glUniform4fv(pos,1,glm::value_ptr(m_material->getDiffuse()));
    }
    //镜面反射材质
    pos=glGetUniformLocation(_gl_program,uniform_name_material_specular);
    if(pos!=-1){
       glUniform4fv(pos,1,glm::value_ptr(m_material->getSpecular()));
    }
    //镜面光滑系数
    pos=glGetUniformLocation(_gl_program,uniform_name_material_shininess);
    if(pos!=-1){
       glUniform1f(pos,m_material->getShininess());
    }
    return true;
}

void node::glInitLight(){
    //初始化材质
    if(!glInitMaterial())
        return;
    //写入环境光颜色
    ambientLight* lightAM=world::getInstance()->getAmbientLight();
    if(lightAM!=NULL){
        int pos=glGetUniformLocation(_gl_program,uniform_name_light_ambient_color);
        if(pos!=-1)
            glUniform4fv(pos,1,glm::value_ptr(lightAM->getColor()));
    }
    //写入漫反射光源位置与颜色
    std::vector<node*> lightVector=world::getInstance()->getLightVector();
    int i=0;
    for(auto c : lightVector){
        light* lightObj=(light*)c;
        char szBuf[128]={0};
        //光源位置
        snprintf(szBuf, sizeof(szBuf), "light_pos_%d",i);
        int pos=glGetUniformLocation(_gl_program,szBuf);
        if(pos!=-1)
            glUniform3fv(pos,1,glm::value_ptr(lightObj->getPosition()));
        //光源颜色
        memset(szBuf,0,sizeof(szBuf));
        snprintf(szBuf, sizeof(szBuf), "light_color_%d",i);
        pos=glGetUniformLocation(_gl_program,szBuf);
        if(pos!=-1)
            glUniform4fv(pos,1,glm::value_ptr(lightObj->getColor()));
    }
}

//只带顶点坐标数据
void node::glInitVAO(){
    unsigned int vbo;
    float* verticeArr=g_verticeArr;
    int verticeArrSize=sizeof(g_verticeArr);
    int numPerVertex=3;  //每个顶点坐标用几个浮点数来表示
    int stride=3*sizeof(float);
  
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
    glEnableVertexAttribArray(0);
    //解除当前VBO绑定
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //解除当前VAO绑定
    glBindVertexArray(0);
}

//顶点坐标，纹理坐标
void node::glInitVAOWithTexCoord(){
    unsigned int vbo;
    float* verticeArr=g_verticeArrWithTexCoord;
    int verticeArrSize=sizeof(g_verticeArrWithTexCoord);
    int numPerVertex=3;  //每个顶点坐标用几个浮点数来表示
    int numPerTex=2;     //每个纹理坐标用几个浮点数来表示
    int stride=5*sizeof(float);

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
}

//顶点坐标，纹理坐标，法向量
void node::glInitVAOWithTexCoordAndNormal(){
    unsigned int vbo;
    float* verticeArr=g_verticeArrWithTexCoordAndNormal;
    int verticeArrSize=sizeof(g_verticeArrWithTexCoordAndNormal);
    int numPerVertex=3;  //每个顶点坐标用几个浮点数来表示
    int numPerTex=2;     //每个纹理坐标用几个浮点数来表示
    int numPerNormal=3;  //每个法向量用几个浮点数来表示
    int stride=8*sizeof(float);

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
    glVertexAttribPointer(2,numPerNormal,GL_FLOAT,GL_FALSE,stride,(void*)(5*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    //解除当前VBO绑定
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //解除当前VAO绑定
    glBindVertexArray(0);
}
