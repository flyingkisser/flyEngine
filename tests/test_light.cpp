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

#include "logUtil.h"
#include "timerMgr.h"
#include "ambientLight.h"
#include "cubeTex.h"
#include "material.h"
#include "shader.h"
#include "shaderMgr.h"
#include "randUtil.h"
#include "threadUtil.h"

#include "light.h"
#include "pointLight.h"
#include "spotLight.h"

USE_NS_FLYENGINE;


static void init_light_ambient(){
    //设置环境光
    ambientLight* amLight=new ambientLight(glm::vec3(0.2f,0.2f,0.2f));
    world::getInstance()->setAmbientLight(amLight);
}

void test_oneLight_oneCube(){
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
    light* lightObj1=new light(glm::vec3(1,1,1),mtLight);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addLight(lightObj1);

    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
        float radius=glfwGetTime();
        lightObj1->setPositionX(2*cos(radius));
        lightObj1->setPositionY(2*sin(radius));
    },lightObj1);
}

void test_oneLight_oneCube_changingColor(){
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
    light* lightObj1=new light(glm::vec3(1,1,1),mtLight);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(1,1,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addLight(lightObj1);

    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
//        float radius=glfwGetTime();
//        lightObj1->setPositionX(2*cos(radius));
//        lightObj1->setPositionY(2*sin(radius));
        float v1=(sin(glfwGetTime()*2)+1)/2;
        float v2=(sin(glfwGetTime()*0.7)+1)/2;
        float v3=(sin(glfwGetTime()*1.3)+1)/2;
        light* lightObj=(light*)lightObj1;
        lightObj->setColor(glm::vec3(v1,v2,v3));
    },lightObj1);
}


void test_oneLight_twoCube(){
    init_light_ambient();
    
    //普通cube1
    node* cubeObj1=new cubeTex("res/wood.png");
    if(!cubeObj1->init()){
      flylog("node init failed!");
      return;
    }
    cubeObj1->setPosition(glm::vec3(-1,0,-8));
    cubeObj1->setScale(0.8);
    cubeObj1->setMaterial(new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8));
    world::getInstance()->addChild(cubeObj1);
    
    //普通cube2
    node* cubeObj2=new cubeTex("res/wood.png");
    if(!cubeObj2->init()){
      flylog("node init failed!");
      return;
    }
    cubeObj2->setPosition(glm::vec3(1,0,-8));
    cubeObj2->setScale(0.8);
    cubeObj2->setMaterial(new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),64));
    world::getInstance()->addChild(cubeObj2);
 
    
    //光源1
    material* mtLight=new material(glm::vec3(0.1,0.1,0.1),glm::vec3(1,1,1),glm::vec3(1,1,1),1);
    light* lightObj1=new light(glm::vec3(1,1,1),mtLight);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addLight(lightObj1);

    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
//    timerMgrObj->exec(1,[](node* lightObj1){
//        float radius=glfwGetTime();
//        lightObj1->setPositionX(2*cos(radius));
//        lightObj1->setPositionY(2*sin(radius));
//    },lightObj1);
}


//有两个光源
void test_twoLight_oneCube(){
    init_light_ambient();
    
    //普通cube
    node* cubeObj=new cubeTex("res/wood.png");
    if(!cubeObj->init()){
      flylog("node init failed!");
      return;
    }
    cubeObj->setPosition(glm::vec3(0,0,-5));
    cubeObj->setMaterial(new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8));
    world::getInstance()->addChild(cubeObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);

    timerMgr* timerMgrObj=new timerMgr("cube_test_timer_1");
    timerMgrObj->exec(0.1,[](node* _node){
      _node->rotateBy(glm::vec3(0.5f,0,0));
    },cubeObj);
    
     material* mtLight=new material(glm::vec3(0.1,0.1,0.1),glm::vec3(1,1,1),glm::vec3(1,1,1),1.0f);
    //光源1
    light* lightObj1=new light(glm::vec3(1,0,0),mtLight);
    if(!lightObj1->init()){
        flylog("lightObj1 init failed!");
        return;
    }
    lightObj1->setPosition(glm::vec3(4,4,-15));
    world::getInstance()->addLight(lightObj1);
    
    //光源2
    light* lightObj2=new light(glm::vec3(0,1,0),mtLight);
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

void test_oneLight_oneCube_specularMap(){
    init_light_ambient();
    
    //普通cube
    cubeTex* cubeObj=new cubeTex("res/wood.png");
    if(!cubeObj->init()){
      flylog("node init failed!");
      return;
    }
    cubeObj->setPosition(glm::vec3(0,0,-8));
    material* mt=new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8);
    mt->setSpecularTex("res/wood_specular.png");
    cubeObj->setMaterial(mt);
    cubeObj->setShader(shaderMgr::get3d1texPongWithSpecularTexShader());
    world::getInstance()->addChild(cubeObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);
    
    //光源1
    material* mtLight=new material(glm::vec3(0.1,0.1,0.1),glm::vec3(1,1,1),glm::vec3(1,1,1),1);
    light* lightObj1=new light(glm::vec3(1,1.0,1.0),mtLight);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addLight(lightObj1);

    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
        float radius=glfwGetTime();
        lightObj1->setPositionX(2*cos(radius));
        lightObj1->setPositionY(2*sin(radius));
    },lightObj1);
}

void test_oneLight_twoCube_specularMap(){
    init_light_ambient();
    
    //普通cube1
    cubeTex* cubeObj1=new cubeTex("res/wood.png");
    if(!cubeObj1->init()){
      flylog("node1 init failed!");
      return;
    }
    cubeObj1->setPosition(glm::vec3(-1,0,-8));
    material* mt1=new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8);
    cubeObj1->setMaterial(mt1);
    world::getInstance()->addChild(cubeObj1);
    
    //普通cube2
    cubeTex* cubeObj2=new cubeTex("res/wood.png");
    if(!cubeObj2->init()){
        flylog("node2 init failed!");
        return;
    }
    cubeObj2->setPosition(glm::vec3(1,0,-8));
    material* mt2=new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8);
    mt2->setSpecularTex("res/wood_specular.png");
    cubeObj2->setMaterial(mt2);
    cubeObj2->setShader(shaderMgr::get3d1texPongWithSpecularTexShader());
    world::getInstance()->addChild(cubeObj2);
    
    //通过按住鼠标右键，控制模型旋转
//    control* controlObj=world::getInstance()->getControl();
//    controlObj->bindNode(cubeObj);
    
    //光源1
    material* mtLight=new material(glm::vec3(0.1,0.1,0.1),glm::vec3(1,1,1),glm::vec3(1,1,1),1);
    light* lightObj1=new light(glm::vec3(1,1.0,1.0),mtLight);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addLight(lightObj1);

    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
        float radius=glfwGetTime();
        lightObj1->setPositionX(2*cos(radius));
        lightObj1->setPositionY(2*sin(radius));
    },lightObj1);
}

void test_oneLight_multiCube_specularMap(){
    init_light_ambient();
    //普通cube
    for(int i=0;i<50;i++){
        cubeTex* cubeObj=new cubeTex("res/wood.png");
        if(!cubeObj->init()){
            flylog("node%d init failed!",i);
            return;
        }
       // float t=glfwGetTime();
        float x=randUtil::getRand(-4.0f,4.0f);
//        threadUtil::sleepMS(100);
        float y=randUtil::getRand(-4.0f,4.0f);
        
        cubeObj->setPosition(glm::vec3(x,y,-8));
        flylog("add %f %f",x,y);
        material* mt=new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8);
        cubeObj->setMaterial(mt);
        cubeObj->setShader(shaderMgr::get3d1texPongWithSpecularTexShader());
        world::getInstance()->addChild(cubeObj);
    }
   
    //光源1
    material* mtLight=new material(glm::vec3(0.1,0.1,0.1),glm::vec3(1,1,1),glm::vec3(1,1,1),1);
    light* lightObj1=new light(glm::vec3(1,1.0,1.0),mtLight);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,0,-4));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addLight(lightObj1);

    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1){
        float radius=glfwGetTime();
        lightObj1->setPositionX(2*cos(radius));
        lightObj1->setPositionY(2*sin(radius));
    },lightObj1);
}


//void test_multiLight_multiCube_specularMap(){
//    init_light_ambient();
//    //普通cube
//    for(int i=0;i<50;i++){
//        cubeTex* cubeObj=new cubeTex("res/wood.png");
//        if(!cubeObj->init()){
//            flylog("node%d init failed!",i);
//            return;
//        }
//       // float t=glfwGetTime();
//        float x=randUtil::getRand(-4.0f,4.0f);
////        threadUtil::sleepMS(100);
//        float y=randUtil::getRand(-4.0f,4.0f);
//
//        cubeObj->setPosition(glm::vec3(x,y,-8));
//        flylog("add %f %f",x,y);
//        material* mt=new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8);
//        cubeObj->setMaterial(mt);
//        cubeObj->setShader(shaderMgr::get3d1texPongWithSpecularTexShader());
//        world::getInstance()->addChild(cubeObj);
//    }
//
//    //光源1
//    material* mtLight=new material(glm::vec3(0.1,0.1,0.1),glm::vec3(1,1,1),glm::vec3(1,1,1),1);
//    light* lightObj1=new light(glm::vec3(1,1.0,1.0),mtLight);
//    if(!lightObj1->init()){
//       flylog("lightObj1 init failed!");
//       return;
//    }
//    lightObj1->setPosition(glm::vec3(0,0,-4));
//    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
//    world::getInstance()->addLight(lightObj1);
//
//    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
//    timerMgrObj->exec(0.1,[](node* lightObj1){
//        float radius=glfwGetTime();
//        lightObj1->setPositionX(2*cos(radius));
//        lightObj1->setPositionY(2*sin(radius));
//    },lightObj1);
//}


void test_multiLight_multiCube_specularMap(){
    float x,y,z;
    init_light_ambient();
    
    material* mtLight=new material(glm::vec3(0.1,0.1,0.1),glm::vec3(1,1,1),glm::vec3(1,1,1),1);
    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
    //普通cube
    for(int i=0;i<30;i++){
        float scale=0.7;
        cubeTex* cubeObj=new cubeTex("res/wood.png");
        if(!cubeObj->init()){
            flylog("node%d init failed!",i);
            return;
        }
        float x=randUtil::getRand(-3.0f,3.0f);
        float y=randUtil::getRand(-3.0f,3.0f);
        cubeObj->setScale(glm::vec3(scale,scale,scale));
        cubeObj->setPosition(glm::vec3(x,y,-8));
        material* mt=new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8);
        mt->setSpecularTex("res/wood_specular.png");
        cubeObj->setMaterial(mt);
        cubeObj->setShader(shaderMgr::get3d1texPongWithSpecularTexShader());
        world::getInstance()->addChild(cubeObj);
        flylog("add cube %f %f",x,y);
    }
    
    
    //点光源
//    for(int i=0;i<4;i++){
//       pointLight* pLight1=new pointLight(glm::vec3(1,1.0,1.0),mtLight);
//       if(!pLight1->init()){
//          flylog("point light %d init failed!",i);
//          return;
//       }
//       x=randUtil::getRand(-2.0f,2.0f);
//       y=randUtil::getRand(-2.0f,2.0f);
//       //z=randUtil::getRand(-2.0f,-4.0f);
//       pLight1->setPosition(glm::vec3(x,y,-4));
//       pLight1->setScale(glm::vec3(0.2,0.2,0.2));
//       world::getInstance()->addPointLight(pLight1);
//       flylog("add point light %d %f %f %f",i,x,y,z);
//    }
    
    //聚光灯
    for(int i=0;i<4;i++){
        spotLight* spLight1=new spotLight(glm::vec3(1,1.0,1.0),mtLight,5,10);
        if(!spLight1->init()){
            flylog("spot light %d init failed!",i);
            return;
        }
        x=randUtil::getRand(-2.0f,2.0f);
        y=randUtil::getRand(-2.0f,2.0f);
        //z=randUtil::getRand(-4.0f,-3);
        z=-4;
        spLight1->setPosition(glm::vec3(x,y,z));
        spLight1->setScale(glm::vec3(0.1,0.1,0.1));
        world::getInstance()->addSpotLight(spLight1);
        flylog("add spot light %d %f %f %f",i,x,y,z);
        
        float timeInner=randUtil::getRand(0.1f,0.5f);
        timerMgrObj->exec(timeInner,[](node* lightObj){
            float radius=glfwGetTime();
            lightObj->setPositionX(2*cos(radius));
            lightObj->setPositionY(2*sin(radius));
        },spLight1);
    }
}

//void test_oneSpotLight_multiCube_specularMap(){
//    init_light_ambient();
//    //普通cube
//    for(int i=0;i<50;i++){
//        cubeTex* cubeObj=new cubeTex("res/wood.png");
//        if(!cubeObj->init()){
//            flylog("node%d init failed!",i);
//            return;
//        }
//        float x=randUtil::getRand(-2.0f,2.0f);
//        float y=randUtil::getRand(-2.0f,2.0f);
//        cubeObj->setScale(glm::vec3(0.2,0.2,0.2));
//        cubeObj->setPosition(glm::vec3(x,y,0));
//        flylog("add %f %f",x,y);
//
//        material* mt=new material(glm::vec3(1.0f,0.9,0.9),glm::vec3(0.8,0.8,0.8),glm::vec3(0.8,0.5,0.5),8);
//        mt->setSpecularTex("res/wood_specular.png");
//        cubeObj->setMaterial(mt);
//        cubeObj->setShader(shaderMgr::get3d1texPongWithSpecularTexShader());
//        world::getInstance()->addChild(cubeObj);
//    }
//
//    //光源1
//    material* mtLight=new material(glm::vec3(0.1,0.1,0.1),glm::vec3(1,1,1),glm::vec3(1,1,1),1);
//    spotLight* lightObj1=new spotLight(glm::vec3(1,1.0,1.0),mtLight,5,10);
//    if(!lightObj1->init()){
//       flylog("lightObj1 init failed!");
//       return;
//    }
//    lightObj1->setPosition(glm::vec3(0,0,-4));
//    lightObj1->setScale(glm::vec3(0.1,0.1,0.1));
//    world::getInstance()->addSpotLight(lightObj1);
//
//    timerMgr* timerMgrObj=new timerMgr("light_test_timer");
//    timerMgrObj->exec(0.4,[](node* lightObj1){
//        float radius=glfwGetTime();
//        lightObj1->setPositionX(2*cos(radius));
//        lightObj1->setPositionZ(2*sin(radius));
//    },lightObj1);
//}
