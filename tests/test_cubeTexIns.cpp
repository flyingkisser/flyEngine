//
//  test_cubeTexIns.cpp
//  flyEngine
//
//  Created by joe on 23/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_cubeTexIns.h"

#include "world.h"
#include "control.h"
#include "logUtil.h"
#include "timerUtil.h"
#include "timeUtil.h"
#include "directionLight.h"
#include "cubeTex.h"
#include "cubeTexIns.h"
#include "model.h"
#include "modelIns.h"

#include "material2.h"
#include "shader.h"
#include "shaderMgr.h"
#include "randUtil.h"
#include "threadUtil.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"
#include "glslUtil.h"
#include "window.h"
#include "model.h"
USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

static void init_light_direction(){
    //设置环境光
    directionLight* dirLight=new directionLight(glm::vec3(0.7f,0.7f,0.7f),glm::vec3(-0.2,-1,-0.3),0.1,0.8,1,2);
    world::getInstance()->setDirectiontLight(dirLight);
}

void test_cubeIns_cube_0(){
    init_light_direction();
    
    //普通cube
    float inner=1;
    float indexI=0;
    int max=100;
    for(int i=0,j=0;i<max;i++){
        node* cubeObj=new cubeTex("res/wood.png");
        if(!cubeObj->init()){
          flylog("node init failed!");
          return;
        }
        cubeObj->setMaterial(createMaterial(1,1,1,2));
        world::getInstance()->addChild(cubeObj);
        
        float x=-2+inner*indexI;
        indexI++;
        if(indexI>=9){
            indexI=0;
            j++;
        }
        float y=-2+inner*j;
        cubeObj->setPosition(glm::vec3(x,y,-8));
        cubeObj->setRotation(glm::vec3(10*i,5*i,8*i));
    }
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
//    controlObj->bindNode(cubeObj);
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!lightObj1->init()){
       flylog("point light init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addPointLight(lightObj1);

    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
        float radius=timeUtil::getTimeFloatSinceRun();
        lightObj1->setPositionX(1.5*cos(radius));
        lightObj1->setPositionY(1.5*sin(radius));
    },lightObj1);
}


//在vertex shader中定义模型变换矩阵的数组，使用uniform的方式从cpu往gpu写数据
//但uniform变量的总数是有限制的，比如说在我的mbp里是1024
void test_cubeIns_cube_1(){
    init_light_direction();
    
    //普通cube
    int curIndex=0;
    int max=100;
    cubeTexIns* cubeObj=new cubeTexIns("res/wood.png",max);
    if(!cubeObj->init()){
      flylog("node init failed!");
      return;
    }
    cubeObj->setMaterial(createMaterial(1,1,1,2));
    world::getInstance()->addChild(cubeObj);

    float inner=1;
    float indexI=0;
    for(int i=0,j=0;i<max;i++){
        float x=-2+inner*indexI;
        indexI++;
        if(indexI>=9){
            indexI=0;
            j++;
        }
        float y=-2+inner*j;
        cubeObj->setPosition(i,glm::vec3(x,y,-8));
        cubeObj->setRotation(i,glm::vec3(10*i,5*i,8*i));
    }
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!lightObj1->init()){
       flylog("point light init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addPointLight(lightObj1);

    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
        float radius=timeUtil::getTimeFloatSinceRun();
        lightObj1->setPositionX(1.5*cos(radius));
        lightObj1->setPositionY(1.5*sin(radius));
    },lightObj1);
}

//在vertex attribute里使用instanced array
//把模型变换矩阵放入到instanced array中，进而实现一个DrawCall，渲染多个6边形
//因此可以突破uniform变量数量的限制，基本上限只和内存总大小有关
void test_cubeIns_cube_2(){
    init_light_direction();
    
    //普通cube
    int curIndex=0;
    int max=100;
    cubeTexIns* cubeObj=new cubeTexIns("res/wood.png",max);
    if(!cubeObj->init()){
      flylog("node init failed!");
      return;
    }
    cubeObj->useInsByVBO();
    
    cubeObj->setMaterial(createMaterial(1,1,1,2));
    world::getInstance()->addChild(cubeObj);

    float inner=1;
    float indexI=0;
    for(int i=0,j=0;i<max;i++){
        float x=-2+inner*indexI;
        indexI++;
        if(indexI>=9){
            indexI=0;
            j++;
        }
        float y=-2+inner*j;
        cubeObj->setPosition(i,glm::vec3(x,y,-8));
        cubeObj->setRotation(i,glm::vec3(10*i,5*i,8*i));
    }
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!lightObj1->init()){
       flylog("point light init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addPointLight(lightObj1);

    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
        float radius=timeUtil::getTimeFloatSinceRun();
        lightObj1->setPositionX(1.5*cos(radius));
        lightObj1->setPositionY(1.5*sin(radius));
    },lightObj1);
}


void test_cubeIns_model_0(){
    init_light_direction();
    
    //普通cube
    float inner=1;
    float indexI=0;
    int max=10;
    for(int i=0,j=0;i<max;i++){
        model* modelObj=new model("res/model/backpack/backpack.obj");
        modelObj->setPosition(glm::vec3(0,0,-0.5));
        if(!modelObj->init()){
          flylog("modelObj init failed!");
          return;
        }
        world::getInstance()->addChild(modelObj);
        
        float x=-2+inner*indexI;
        indexI++;
        if(indexI>=9){
            indexI=0;
            j++;
        }
        float y=-2+inner*j;
        modelObj->setPosition(glm::vec3(x,y,-8));
        modelObj->setRotation(glm::vec3(10*i,5*i,8*i));
    }
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!lightObj1->init()){
       flylog("point light init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addPointLight(lightObj1);

    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
        float radius=timeUtil::getTimeFloatSinceRun();
        lightObj1->setPositionX(1.5*cos(radius));
        lightObj1->setPositionY(1.5*sin(radius));
    },lightObj1);
}

void test_cubeIns_model_1(){
    init_light_direction();
    int max=20;
    modelIns* modelObj=new modelIns("res/model/backpack/backpack.obj",max);
    if(!modelObj->init()){
      flylog("modelObj init failed!");
      return;
    }
    world::getInstance()->addChild(modelObj);

    float inner=1;
    float indexI=0;
    for(int i=0,j=0;i<max;i++){
        float x=-2+inner*indexI;
        indexI++;
        if(indexI>=9){
            indexI=0;
            j++;
        }
        float y=-2+inner*j;
        modelObj->setPosition(i,glm::vec3(x,y,-8));
        modelObj->setRotation(i,glm::vec3(10*i,5*i,8*i));
    }
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(modelObj);
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!lightObj1->init()){
       flylog("point light init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addPointLight(lightObj1);

    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1,modelIns* modelObj){
        float radius=timeUtil::getTimeFloatSinceRun();
        lightObj1->setPositionX(1.5*cos(radius));
        lightObj1->setPositionY(1.5*sin(radius));
        
        modelObj->setRotation(glm::vec3(10*cos(radius),5*cos(radius),0));
        
    },lightObj1,modelObj);
}

void test_cubeIns_model_2(){
    init_light_direction();
    int max=20;
    modelIns* modelObj=new modelIns("res/model/backpack/backpack.obj",max);
    if(!modelObj->init()){
      flylog("modelObj init failed!");
      return;
    }
    world::getInstance()->addChild(modelObj);
    modelObj->useInsByVBO();

    float inner=1;
    float indexI=0;
    for(int i=0,j=0;i<max;i++){
        float x=-2+inner*indexI;
        indexI++;
        if(indexI>=9){
            indexI=0;
            j++;
        }
        float y=-2+inner*j;
        modelObj->setPosition(i,glm::vec3(x,y,-8));
        modelObj->setRotation(i,glm::vec3(10*i,5*i,8*i));
    }
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(modelObj);
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!lightObj1->init()){
       flylog("point light init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addPointLight(lightObj1);

    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1,modelIns* modelObj){
        float radius=timeUtil::getTimeFloatSinceRun();
        lightObj1->setPositionX(1.5*cos(radius));
        lightObj1->setPositionY(1.5*sin(radius));
        modelObj->setRotation(glm::vec3(10*cos(radius),5*cos(radius),0));
    },lightObj1,modelObj);
}
