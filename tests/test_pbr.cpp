//
//  test_pbr.cpp
//  flyEngine
//
//  Created by Joe on 2022/12/14.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_pbr.h"
#include "world.h"
#include "control.h"
#include "logUtil.h"
#include "timerUtil.h"
#include "timeUtil.h"
#include "directionLight.h"
#include "cubeTex.h"
#include "material2.h"
#include "shader.h"
#include "shaderMgr.h"
#include "randUtil.h"
#include "threadUtil.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"
#include "glslUtil.h"
#include "values.h"
#include "camera.h"
#include "model.h"

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

static void init_light_direction(){
    //设置环境光
    directionLight* dirLight=new directionLight(glm::vec3(0.7f,0.7f,0.7f),glm::vec3(-0.2,-1,-0.3),0.1,0.1,0.1,2);
    world::getInstance()->setDirectiontLight(dirLight);
}


void test_PBR_1(){
    init_light_direction();
    
    //普通cube
    node* cubeObj=new cubeTex("res/wood.png");
    if(!cubeObj->init()){
        flylog("node init failed!");
        return;
    }
    cubeObj->setPosition(glm::vec3(0,0,-8));
    cubeObj->setRotation(glm::vec3(30,60,0));
    cubeObj->setMaterial(createMaterial(1,1,1,2));
    world::getInstance()->addChild(cubeObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    
    
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
    
    controlObj->bindNode(cubeObj);
    controlObj->bindNode(lightObj1);
    
    camera* cam=world::getInstance()->getCamera();

    shader* shPBR=new shader("res/shader/3d_1tex_phong.vs","res/shader/3d_1tex_pbr.fs");
    cubeObj->setShader(shPBR);
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        flylog("cam at %f %f %f",pos.x,pos.y,pos.z);
    });
}
