//
//  test_gamma.cpp
//  flyEngine
//
//  Created by joe on 09/10/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_gamma.h"
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

//enable/disable gamma correct by fragment shader
void test_gamma_1(){
    init_light_direction();
    //普通cube
    cubeTex* cubeObj=new cubeTex("res/floor.png");
    int descArr[]={3,2,3};
    if(!cubeObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane,sizeof(g_verticeArrWithTexCoordAndNormal_plane),descArr,3)){
        flylog("init plainObj failed!");
        return;
    }
    //cubeObj的实际位置已经在g_verticeArrWithTexCoordAndNormal_plane进行了定义，实际y值为-0.5
    cubeObj->setPosition(glm::vec3(0,-0.5,0));
    cubeObj->setRotation(glm::vec3(10,0,0));
    cubeObj->setMaterial(createMaterial(1,1,1,2));
    shader* sh=cubeObj->getShader();
    sh->setBool("bUseBlinnPhong",true);
    world::getInstance()->addChild(cubeObj);
    
    model* modelObj=new model("res/model/planet/planet.obj");
    modelObj->init();
    modelObj->setPosition(glm::vec3(0,2,0));
    modelObj->setRotation(glm::vec3(0,30,0));
    modelObj->setScale(0.2);
    shader* sh2=modelObj->getShader();
    sh2->setBool("bUseBlinnPhong",true);
    world::getInstance()->addChild(modelObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);
    
    //光源1
    material2* mtLight=createMaterial(0.9, 0.9, 1, 2);
    float s=0.2;
    float x=1.5;
    float y=1;
    pointLight* lightObj1=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,1);
    if(!lightObj1->init()){
       flylog("point light1 init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(-x,y,0));
    lightObj1->setScale(s);
    pointLight* lightObj2=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,1);
    if(!lightObj2->init()){
       flylog("point light2 init failed!");
       return;
    }
    lightObj2->setPosition(glm::vec3(x,y,0));
    lightObj2->setScale(s);
    
    world::getInstance()->addPointLight(lightObj1);
    world::getInstance()->addPointLight(lightObj2);

    world::getInstance()->getCamera()->setPosition(glm::vec3(0,0,10));
    
    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1,model* modelObj){
//        float radius=timeUtil::getTimeFloatSinceRun();
//        lightObj1->setPositionX(1.5*cos(radius));
//        lightObj1->setPositionY(1.5*sin(radius));
        modelObj->rotateBy(glm::vec3(0,1,0));
    },lightObj1,modelObj);
    
    world::getInstance()->getControl()->regOnKeyPress('b', [cubeObj,modelObj](){
        shader* sh=cubeObj->getShader();
        sh->use();
        bool blined1=sh->getBool("bUseBlinnPhong");
        sh->setBool("bUseBlinnPhong",!blined1);
        cubeObj->setDirtyPos(true);
        shader* sh2=modelObj->getShader();
        sh2->use();
        bool blined2=sh2->getBool("bUseBlinnPhong");
        sh2->setBool("bUseBlinnPhong",!blined2);
        modelObj->setDirtyPos(true);
        flylog("set blinn1 %d blinn2 %d",!blined1,!blined2);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cubeObj,modelObj](){
        shader* sh=cubeObj->getShader();
        sh->use();
        float gamma1=sh->getFloat("gamma_correct",true);
        gamma1=gamma1>=2.2 ? 0.0f : 2.2f;
        sh->setFloat("gamma_correct",gamma1,true);
        cubeObj->setDirtyPos(true);
        
        shader* sh2=modelObj->getShader();
        sh2->use();
        float gamma2=sh2->getFloat("gamma_correct",true);
        gamma2=gamma2>=2.2 ? 0.0f : 2.2f;
        sh2->setFloat("gamma_correct",gamma2,true);
        modelObj->setDirtyPos(true);
        
        flylog("set gamma1 %f gamma2 %f",gamma1,gamma2);
    });
}

//enable/disable gamma correct by glEnable()
void test_gamma_2(){
    init_light_direction();
    //普通cube
    cubeTex* cubeObj=new cubeTex("res/floor.png");
    int descArr[]={3,2,3};
    if(!cubeObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane,sizeof(g_verticeArrWithTexCoordAndNormal_plane),descArr,3)){
        flylog("init plainObj failed!");
        return;
    }
    //cubeObj的实际位置已经在g_verticeArrWithTexCoordAndNormal_plane进行了定义，实际y值为-0.5
    cubeObj->setPosition(glm::vec3(0,-0.5,0));
    cubeObj->setRotation(glm::vec3(10,0,0));
    cubeObj->setMaterial(createMaterial(1,1,1,2));
    shader* sh=cubeObj->getShader();
    sh->setBool("bUseBlinnPhong",true);
    world::getInstance()->addChild(cubeObj);
    
    model* modelObj=new model("res/model/planet/planet.obj");
    modelObj->init();
    modelObj->setPosition(glm::vec3(0,2,0));
    modelObj->setRotation(glm::vec3(0,30,0));
    modelObj->setScale(0.2);
    shader* sh2=modelObj->getShader();
    sh2->setBool("bUseBlinnPhong",true);
    world::getInstance()->addChild(modelObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);
    
    //光源1
    material2* mtLight=createMaterial(0.9, 0.9, 1, 2);
    float s=0.2;
    float x=1.5;
    float y=1;
    pointLight* lightObj1=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,1);
    if(!lightObj1->init()){
       flylog("point light1 init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(-x,y,0));
    lightObj1->setScale(s);
    pointLight* lightObj2=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,1);
    if(!lightObj2->init()){
       flylog("point light2 init failed!");
       return;
    }
    lightObj2->setPosition(glm::vec3(x,y,0));
    lightObj2->setScale(s);
    
    world::getInstance()->addPointLight(lightObj1);
    world::getInstance()->addPointLight(lightObj2);

    world::getInstance()->getCamera()->setPosition(glm::vec3(0,0,10));
    
    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* lightObj1,model* modelObj){
//        float radius=timeUtil::getTimeFloatSinceRun();
//        lightObj1->setPositionX(1.5*cos(radius));
//        lightObj1->setPositionY(1.5*sin(radius));
        modelObj->rotateBy(glm::vec3(0,1,0));
    },lightObj1,modelObj);
    
    world::getInstance()->setCBBeforeDrawCall([](){
        glEnable(GL_FRAMEBUFFER_SRGB);
    });
}
