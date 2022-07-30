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
#include "world.h"
#include "material.h"

#include "directionLight.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"



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
    _rotate+=v;
    _dirtyPos=true;
}
void node::setRotation(glm::vec3 v){
    _rotate=v;
    _dirtyPos=true;
}

//设置scale
void node::setScale(glm::vec3 v){
    _scale=v;
    _dirtyPos=true;
}
void node::setScale(float v){
    _scale=glm::vec3(v,v,v);
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
    if(m_material!=NULL)
        delete m_material;
    m_material=mt;
}

void node::setShader(shader* shaderObj){
    _shaderObj=shaderObj;
    _shaderObj->use();
    _gl_program=shaderObj->getProgramID();
}

void node::updateModel(){
    if(_dirtyPos){
        //移动
        _matModel=glm::translate(glm::mat4(1.0f),_pos);
        
        //旋转
        if(_rotate.x)//沿x轴
          _matModel=glm::rotate(_matModel,glm::radians(_rotate.x),glm::vec3(1,0,0));
        if(_rotate.y)//沿y轴
          _matModel=glm::rotate(_matModel,glm::radians(_rotate.y),glm::vec3(0,1,0));
        if(_rotate.z)//沿z轴
          _matModel=glm::rotate(_matModel,glm::radians(_rotate.z),glm::vec3(0,0,1));
        //缩放
        _matModel=glm::scale(_matModel,_scale);
        _dirtyPos=false;
    }
    _shaderObj->setMat4(uniform_name_mat_model, glm::value_ptr(_matModel));
}

void node::initVAO(float* arr,int arrSize,int descArr[],int descArrSize){
    _vertice_arr=arr;
    _vertice_arr_size=arrSize;
    _desc_arr=descArr;
    _desc_arr_size=descArrSize;
    glGenVertexArrays(1,&_gl_vao);
    glBindVertexArray(_gl_vao);
    glGenBuffers(1,&_gl_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER,arrSize,arr,GL_STATIC_DRAW);
    int stride=0;
    for(int i=0;i<descArrSize;i++){
        stride+=descArr[i];
    }
    stride*=sizeof(float);
    for(int i=0,k=0;i<descArrSize;i++){
        glVertexAttribPointer(i,descArr[i],GL_FLOAT,GL_FALSE,stride,(void*)(k*sizeof(float)));
        k+=descArr[i];
        glEnableVertexAttribArray(i);
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void node::glUpdateLight(){
    //平行光
    directionLight* lightDir=world::getInstance()->getDirectiontLight();
    if(lightDir!=NULL)
        lightDir->glUpdate(_gl_program);
    int i=0;
    
//    std::vector<light*> lightVector=world::getInstance()->getLightVector();
//    int i=0;
//    for(auto c : lightVector){
//        light* lightObj=(light*)c;
//        lightObj->glUpdateForCube(_gl_program,i++);
//    }
    //点光源初始化
    std::vector<pointLight*> pointLightVector=world::getInstance()->getPointLightVector();
    i=0;
    for(auto c : pointLightVector){
        pointLight* lightObj=(pointLight*)c;
        if(lightObj->getMaterial()!=nullptr)
            lightObj->glUpdateUseMaterail(_gl_program,i++);
        else
            lightObj->glUpdate(_gl_program,i++);
    }
    //聚光灯初始化
    std::vector<spotLight*> spotLightVector=world::getInstance()->getSpotLightVector();
    i=0;
    for(auto c : spotLightVector){
       spotLight* lightObj=(spotLight*)c;
       lightObj->glUpdateForCube(_gl_program,i++,world::getInstance()->getCamera());
    }
}
