//
//  Node.cpp
//  flyEngine
//
//  Created by joe on 24/06/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "node.h"
#include "logUtil.h"
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "action.h"
#include "camera.h"
#include "world.h"
#include "material2.h"

#include "directionLight.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"



USE_NS_FLYENGINE



node::node(){
}

//从当前位置，移动一个指定的距离
void node::moveBy(glm::vec3 v){
    _pos+=v;
    _dirtyPos=true;
    _dirtyUBO=true;
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
    _dirtyUBO=true;
};

void node::setPositionX(float v){
    _pos.x=v;
    _dirtyPos=true;
    _dirtyUBO=true;
};

void node::setPositionY(float v){
    _pos.y=v;
    _dirtyPos=true;
    _dirtyUBO=true;
};

void node::setPositionZ(float v){
    _pos.z=v;
    _dirtyPos=true;
    _dirtyUBO=true;
};

//动画相关
void node::runAction(action* act){
    act->start(this);
}

void node::stopAction(action* act){
    act->stop();
}

//材质
void node::setMaterial(material2 *mt){
    if(m_material!=NULL)
        delete m_material;
    m_material=mt;
    _dirtyMT=true;
    _dirtyUBO=true;
}

void node::setShader(shader* shaderObj){
    _shaderObj=shaderObj;
    _shaderObj->use();
    _gl_program=shaderObj->getProgramID();
}

glm::vec3 node::getSize(){
    glm::vec3 s;
    for(int i=0;i<3;i++){
        s[i]=std::abs(_vertice_arr[0+i])+std::abs(_vertice_arr[8+i]);
    }
    return s;
}
bool node::isOnFrustumBySphere(){
    glm::vec3 scale=getScale();
    float maxScale=std::max(std::max(scale.x,scale.y),scale.z);
    glm::vec3 size=getSize();
    float r=size.x*maxScale*0.5f;
    glm::vec3 center=getPosition();
    return _stFrustum->left.getSignedDistanceToPlane(center)>-r &&
    _stFrustum->right.getSignedDistanceToPlane(center)>-r &&
    _stFrustum->far.getSignedDistanceToPlane(center)>-r &&
    _stFrustum->near.getSignedDistanceToPlane(center)>-r &&
    _stFrustum->top.getSignedDistanceToPlane(center)>-r &&
    _stFrustum->bottom.getSignedDistanceToPlane(center)>-r;
//    return true;
};

void node::print(){
    glm::vec3 scale=getScale();
    float maxScale=std::max(std::max(scale.x,scale.y),scale.z);
    glm::vec3 size=getSize();
    float r=size.x*maxScale*0.5f;
    glm::vec3 center=getPosition();
    float left=_stFrustum->left.getSignedDistanceToPlane(center);
    float right=_stFrustum->right.getSignedDistanceToPlane(center);
    float far=_stFrustum->far.getSignedDistanceToPlane(center);
    float near=_stFrustum->near.getSignedDistanceToPlane(center);
    float top=_stFrustum->top.getSignedDistanceToPlane(center);
    float bottom=_stFrustum->bottom.getSignedDistanceToPlane(center);
    bool result=left>-r && right>-r && far>-r && near>-r && top>-r && bottom>-r;
    flylog("node:pos %f %f %f left %f right %f far %f near %f top %f bottom %f -r %f result %d ",_pos.x,_pos.y,_pos.z,left,right,far,near,top,bottom,-r,result);
}

void node::initVAO(float* arr,int arrSize,int descArr[],int descArrSize){
    _vertice_arr=arr;
    _vertice_arr_size=arrSize;
    _desc_arr=descArr;
    _desc_arr_size=descArrSize;
    if(_gl_vao>0){
        glDeleteVertexArrays(1,&_gl_vao);
        _gl_vao=0;
    }
        
    if(_gl_vbo>0){
        glDeleteBuffers(1,&_gl_vbo);
        _gl_vbo=0;
    }
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

glm::mat4 node::calcModelMatrix(){
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
    return _matModel;
}


void node::glUpdateLight(){
    int i=0;
    
    //平行光
   directionLight* lightDir=world::getInstance()->getDirectiontLight();
   if(lightDir!=NULL)
       lightDir->update(0);
    
    //点光源初始化
     i=0;
    std::vector<pointLight*> pointLightVector=world::getInstance()->getPointLightVector();
    for(auto c : pointLightVector){
        pointLight* lightObj=(pointLight*)c;
        lightObj->update(i++);
    }
    //聚光灯初始化
    i=0;
    std::vector<spotLight*> spotLightVector=world::getInstance()->getSpotLightVector();
    for(auto c : spotLightVector){
       spotLight* lightObj=(spotLight*)c;
       lightObj->update(i++);
    }
}

