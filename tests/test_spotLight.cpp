//
//  test_spotLight.cpp
//  flyEngine
//
//  Created by joe on 13/09/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "test_spotLight.h"
#include "world.h"
#include "control.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"
#include "logUtil.h"
#include "timerMgr.h"
#include "ambientLight.h"
#include "cubeTex.h"
#include "material.h"
#include "shader.h"
#include "shaderMgr.h"
#include "randUtil.h"
#include "threadUtil.h"

USE_NS_FLYENGINE;


static void init_light_ambient(){
    //设置环境光
    ambientLight* amLight=new ambientLight(glm::vec3(0.2f,0.2f,0.2f));
    world::getInstance()->setAmbientLight(amLight);
}

void test_oneSpotLight_oneCube(){
    init_light_ambient();
    
    //普通cube
    node* cubeObj=new cubeTex("res/wood.png");
    if(!cubeObj->init()){
      flylog("node init failed!");
      return;
    }
    cubeObj->setPosition(glm::vec3(0,0,-8));
    cubeObj->setMaterial(new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8));
    world::getInstance()->addChild(cubeObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);
    
    //光源1
    material* mtLight=new material(glm::vec3(0.1,0.1,0.1),glm::vec3(1,1,1),glm::vec3(1,1,1),1);
    spotLight* lightObj1=new spotLight(glm::vec3(1,1,1),mtLight,15,25);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addSpotLight(lightObj1);

    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
        float radius=glfwGetTime();
        lightObj1->setPositionX(2*cos(radius));
        lightObj1->setPositionY(2*sin(radius));
    },lightObj1);
}

void test_oneSpotLight_multiCube_specularMap(){
    init_light_ambient();
    //普通cube
    for(int i=0;i<50;i++){
        cubeTex* cubeObj=new cubeTex("res/wood.png");
        if(!cubeObj->init()){
            flylog("node%d init failed!",i);
            return;
        }
        float x=randUtil::getRand(-2.0f,2.0f);
        float y=randUtil::getRand(-2.0f,2.0f);
        float z=0;
        cubeObj->setScale(glm::vec3(0.2,0.2,0.2));
        cubeObj->setPosition(glm::vec3(x,y,z));
        flylog("add %f %f",x,y);
        
        material* mt=new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8);
        mt->setSpecularTex("res/wood_specular.png");
        cubeObj->setMaterial(mt);
        cubeObj->setShader(shaderMgr::get3d1texPongWithSpecularTexShader());
        world::getInstance()->addChild(cubeObj);
    }
   
    //光源1
    material* mtLight=new material(glm::vec3(0.1,0.1,0.1),glm::vec3(1,1,1),glm::vec3(1,1,1),1);
    spotLight* lightObj1=new spotLight(glm::vec3(1,1.0,1.0),mtLight,5,10);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    float z=-4;
    lightObj1->setPosition(glm::vec3(0,0,z));
    lightObj1->setScale(glm::vec3(0.1,0.1,0.1));
    world::getInstance()->addSpotLight(lightObj1);

    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
    timerMgrObj->exec(0.4,[](node* lightObj1){
        float radius=glfwGetTime();
        lightObj1->setPositionX(2*cos(radius));
        lightObj1->setPositionZ(2*sin(radius));
    },lightObj1);
}
