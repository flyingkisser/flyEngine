//
//  test_spotLight.cpp
//  flyEngine
//
//  Created by joe on 13/09/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "test_spotLight.h"
#include "material2.h"
#include "world.h"
#include "control.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"
#include "logUtil.h"
#include "timerUtil.h"
#include "timeUtil.h"
#include "directionLight.h"
#include "cubeTex.h"

#include "shader.h"
#include "shaderMgr.h"
#include "randUtil.h"
#include "threadUtil.h"
#include "camera.h"
#include "glslUtil.h"

USE_NS_FLYENGINE;

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

static void init_light_direction(){
    //设置环境光
    directionLight* dirLight=new directionLight(glm::vec3(0.2f,0.2f,0.2f));
    world::getInstance()->setDirectiontLight(dirLight);
    
    //因为只有环境光，所以设置的比较亮
    //dirLight->setMaterial(createMaterial(1.0,1.0,1.0,0.2));
}

void test_oneSpotLight_oneCube(){
    //init_light_direction();
    
    //普通cube
    node* cubeObj=new cubeTex("res/wood.png");
    if(!cubeObj->init()){
      flylog("node init failed!");
      return;
    }
    cubeObj->setPosition(glm::vec3(0,0,-6));
    material2* mt=createMaterial(1.0, 0.8, 0.8, 8);
//    mt->setSpecularTex("res/wood_specular.png");
    cubeObj->setMaterial(mt);
    world::getInstance()->addChild(cubeObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);

    //光源1
    float am=0.1f;
    float diff=0.5f;
    float sp=0.9f;
    material2* mtLight=createMaterial(am, diff, sp, 2);
    spotLight* lightObj1=new spotLight(glm::vec3(0.8,0.8,0.8),mtLight,30,60);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-2));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    lightObj1->setDirection(glm::vec3(0,0,-1));
    world::getInstance()->addSpotLight(lightObj1);
    lightObj1->update(cubeObj->getShader()->getProgramID(),0);
    
    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
        float radius=timeUtil::getTimeFloatSinceRun();
        lightObj1->setPositionX(1*cos(radius));
        lightObj1->setPositionY(1*sin(radius));
        
//        lightObj1->setPositionX(1*cos(radius));
//        lightObj1->setPositionZ(2*sin(radius));
    },lightObj1);
    
    cubeObj->getShader()->use();
    glslUtil::printAllUniformAndBlock(cubeObj->getShader()->getProgramID());
}

void test_oneSpotLight_multiCube_specularMap(){
    init_light_direction();
    //普通cube
    float am=0.1f;
    float diff=0.3f;
    float sp=0.8f;
    material2* mt=createMaterial(am,diff,sp,2);
    mt->setSpecularTex("res/wood_specular.png");
    
    for(int i=0;i<50;i++){
        cubeTex* cubeObj=new cubeTex("res/wood.png");
        if(!cubeObj->init()){
            flylog("node%d init failed!",i);
            return;
        }
        float x=randUtil::getRand(-2.0f,2.0f);
        float y=randUtil::getRand(-2.0f,2.0f);
        float z=0;
        z=-6;
        cubeObj->setScale(glm::vec3(0.2,0.2,0.2));
        cubeObj->setPosition(glm::vec3(x,y,z));
        cubeObj->setMaterial(mt);
        world::getInstance()->addChild(cubeObj);
        flylog("add %f %f",x,y);
    }
   
    //光源1
    am=0.1f;
    diff=0.8f;
    sp=0.8f;
    material2* mtLight=createMaterial(am,diff,sp,2);
    spotLight* lightObj1=new spotLight(glm::vec3(1,1.0,1.0),mtLight,10,30);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    float z=-4;
//    z=-2;
    lightObj1->setPosition(glm::vec3(0,0,z));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    lightObj1->setDirection(glm::vec3(0,0,-1));
    world::getInstance()->addSpotLight(lightObj1);

    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.15,[](node* lightObj1){
        float radius=(float)timeUtil::getTimeFloatSinceRun();
        lightObj1->setPositionX(2*cos(radius));
//        lightObj1->setPositionZ(2*sin(radius));
        lightObj1->setPositionY(2*sin(radius));
    },lightObj1);
}

void test_oneSpotLight_multiCube_specularMap_cameraControl(){
    //init_light_direction();
    //普通cube
    float am=0.1f;
    float diff=0.3f;
    float sp=0.8f;
    material2* mt=createMaterial(am,diff,sp,2);
    mt->setSpecularTex("res/wood_specular.png");
    int programID=0;
    
    for(int i=0;i<50;i++){
        cubeTex* cubeObj=new cubeTex("res/wood.png");
        if(!cubeObj->init()){
            flylog("node%d init failed!",i);
            return;
        }
        float x=randUtil::getRand(-2.0f,2.0f);
        float y=randUtil::getRand(-2.0f,2.0f);
        float z=-4;
        cubeObj->setScale(glm::vec3(0.2,0.2,0.2));
        cubeObj->setPosition(glm::vec3(x,y,z));
        flylog("add %f %f",x,y);
        cubeObj->setMaterial(mt);
        world::getInstance()->addChild(cubeObj);
        if(!programID)
            programID=cubeObj->getShader()->getProgramID();
    }
    //光源1
    am=0.0f;
    diff=0.3f;
    sp=1.0f;
    material2* mtLight=createMaterial(am,diff,sp,2);
    spotLight* lightObj1=new spotLight(glm::vec3(1,1.0,1.0),mtLight,10,20);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    float z=-2;
    lightObj1->setPosition(glm::vec3(0,0,z));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    lightObj1->setDirection(glm::vec3(0,0,-1));
    world::getInstance()->addSpotLight(lightObj1);

    control* conObj=world::getInstance()->getControl();
    conObj->bindNodeWithCameraMove(lightObj1);
    conObj->regOnKeyPress('=', [lightObj1,programID](){
        lightObj1->setCutOffInner(lightObj1->getCutOffInner()+1);
        glslUtil::printAllUniformAndBlock(programID);
    });
    conObj->regOnKeyPress('-', [lightObj1](){
        lightObj1->setCutOffInner(lightObj1->getCutOffInner()-1);
    });
    glslUtil::printAllUniformAndBlock(programID);
}
