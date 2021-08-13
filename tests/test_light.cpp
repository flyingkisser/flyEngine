//
//  test_light.cpp
//  flyEngine
//
//  Created by joe on 10/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "test_light.h"
#include "world.h"
#include "control.h"
#include "light.h"

#include "logUtil.h"
#include "timerMgr.h"
#include "ambientLight.h"
#include "cubeTex.h"
#include "material.h"

USE_NS_FLYENGINE;


void init_light_ambient(){
    //设置环境光
    ambientLight* amLight=new ambientLight(glm::vec4(0.2f,0.2f,0.2f,1.0f));
    world::getInstance()->setAmbientLight(amLight);
}

void test_light_phong(){
    init_light_ambient();
    
    //普通cube
    node* cubeObj=new cubeTex("res/fire.png");
    if(!cubeObj->init()){
      flylog("node init failed!");
      return;
    }
    cubeObj->setPosition(glm::vec3(0,0,-5));
    cubeObj->setMaterial(new material(glm::vec4(1.0f,0.9,0.9,1),glm::vec4(0.8,0.8,0.8,1),glm::vec4(0.8,0.5,0.5,1),8));
    world::getInstance()->addChild(cubeObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);

    timerMgr* timerMgrObj=new timerMgr("cube_test_timer_1");
    timerMgrObj->exec(0.1,[](node* _node){
      _node->rotateBy(glm::vec3(0.5f,0,0));
    },cubeObj);
    
    
    //光源1
    node* lightObj1=new light(glm::vec4(1,0,0,1));
    if(!lightObj1->init()){
        flylog("lightObj1 init failed!");
        return;
    }
    lightObj1->setPosition(glm::vec3(4,4,-15));
    world::getInstance()->addLight(lightObj1);
    
    //光源2
    node* lightObj2=new light(glm::vec4(0,1,0,1));
    if(!lightObj2->init()){
        flylog("lightObj2 init failed!");
        return;
    }
    lightObj2->setPosition(glm::vec3(-4,4,-15));
    world::getInstance()->addLight(lightObj2);
    
    timerMgr* timerMgrObj2=new timerMgr("cube_test_timer_2");
    timerMgrObj2->exec(0.1,[](node* _node){
      _node->rotateBy(glm::vec3(0.5f,0,0));
    },lightObj1);
    
    
}

//void test_oneNode(){
//    node* cubeObj=new cubeTex("res/fire.png");
//    if(!cubeObj->init()){
//        flylog("node init failed!");
//        return;
//    }
//    cubeObj->setPosition(glm::vec3(0,0,-5));
//
//    world::getInstance()->addChild(cubeObj);
//    cubeObj->print();
//
//    //通过按住鼠标右键，控制模型旋转
//    control* controlObj=world::getInstance()->getControl();
//    controlObj->bindNode(cubeObj);
//
//    timerMgr* timerMgrObj=new timerMgr("cube_test_timer");
//    timerMgrObj->exec(0.1,[](node* _node){
//        _node->rotateBy(glm::vec3(0.5f,0,0));
//    },cubeObj);
//}
