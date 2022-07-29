//
//  test_skybox.cpp
//  flyEngine
//
//  Created by joe on 22/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_skybox.h"
#include "defines.h"
#include "skybox.h"
#include "world.h"
#include "camera.h"
#include "cubeTex.h"
#include "control.h"
#include "shaderMgr.h"
#include "textureMgr.h"
#include "repeat.h"
#include "moveBy.h"
#include "rotateBy.h"
#include "model.h"

USE_NS_FLYENGINE
void test_skybox_1(){
    skybox* sky=new skybox("res/skybox/right.jpg","res/skybox/left.jpg","res/skybox/top.jpg",
                           "res/skybox/bottom.jpg","res/skybox/front.jpg","res/skybox/back.jpg");
    world::getInstance()->addChild(sky);
    cubeTex* cubeObj=new cubeTex("res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-4));
    cubeObj->setRotation(glm::vec3(0,30,30));
    world::getInstance()->addChild(cubeObj);
    world::getInstance()->getControl()->bindNode(cubeObj);
}

//reflection
void test_skybox_reflect(){
    skybox* sky=new skybox("res/skybox/right.jpg","res/skybox/left.jpg","res/skybox/top.jpg",
                           "res/skybox/bottom.jpg","res/skybox/front.jpg","res/skybox/back.jpg");
    unsigned int texID=sky->getTexID();
    cubeTex* cubeObj=new cubeTex(texID);
   
    cubeObj->setShader(shaderMgr::getShader("./res/shader/cubetex_reflect.vs", "./res/shader/cubetex_reflect.fs"));
    int descArr[]={3,3};
    cubeObj->initByVerticeArr(g_verticeArrWithNormal,sizeof(g_verticeArrWithNormal),descArr,2);
   
    cubeObj->setPosition(glm::vec3(0,0,-4));
    cubeObj->setRotation(glm::vec3(0,30,30));
    world::getInstance()->addChild(cubeObj);
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    world::getInstance()->addChild(sky);
}

void test_skybox_reflect_2(){
    skybox* sky=new skybox("res/skybox/right.jpg","res/skybox/left.jpg","res/skybox/top.jpg",
                           "res/skybox/bottom.jpg","res/skybox/front.jpg","res/skybox/back.jpg");
    unsigned int texID=sky->getTexID();
    cubeTex* cubeObj=new cubeTex(texID);
    int descArr[]={3,3};
    cubeObj->initByVerticeArr(g_verticeArrWithNormal, sizeof(g_verticeArrWithNormal), descArr, 2);
    shader* sh=new shader("./res/shader/cubetex_reflect.vs","./res/shader/cubetex_reflect.fs");
    cubeObj->setShader(sh);
    cubeObj->setPosition(glm::vec3(0,0,-4));
    cubeObj->setRotation(glm::vec3(0,30,30));
    world::getInstance()->addChild(cubeObj);
    world::getInstance()->getControl()->bindNode(cubeObj);
    world::getInstance()->addChild(sky);
    action* ro=new rotateBy(3,glm::vec3(0,30,0));
    action* act=new repeat(99,3,ro,new moveBy(1,glm::vec3(0.2,0,0)),new moveBy(1,glm::vec3(-0.2,0,0)));
    cubeObj->runAction(act);
}

void test_skybox_reflect_3(){
    skybox* sky=new skybox("res/skybox/right.jpg","res/skybox/left.jpg","res/skybox/top.jpg",
                           "res/skybox/bottom.jpg","res/skybox/front.jpg","res/skybox/back.jpg");
    unsigned int texID=sky->getTexID();
    model* modelObj=new model("res/model/backpack/backpack.obj");
    modelObj->init();
    shader* sh=shaderMgr::getShader("res/shader/3d_model.vs","res/shader/3d_model_reflect.fs");
    modelObj->setShader(sh);
    modelObj->setCBBeforeDraw([texID,sh](){
        sh->setInt("texture0", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,texID);
    });
    modelObj->setPosition(glm::vec3(0,0,-10));
    world::getInstance()->addChild(modelObj);
    world::getInstance()->getControl()->bindNode(modelObj);
    world::getInstance()->addChild(sky);
    action* ro=new rotateBy(3,glm::vec3(0,30,0));
    action* act=new repeat(99,3,ro,new moveBy(1,glm::vec3(0.2,0,0)),new moveBy(1,glm::vec3(-0.2,0,0)));
    modelObj->runAction(act);
}


void test_skybox_refract_1(){
    skybox* sky=new skybox("res/skybox/right.jpg","res/skybox/left.jpg","res/skybox/top.jpg",
                           "res/skybox/bottom.jpg","res/skybox/front.jpg","res/skybox/back.jpg");
    unsigned int texID=sky->getTexID();
    model* modelObj=new model("res/model/backpack/backpack.obj");
    modelObj->init();
    shader* sh=shaderMgr::getShader("res/shader/3d_model.vs","res/shader/3d_model_refract.fs");
    modelObj->setShader(sh);
    modelObj->setCBBeforeDraw([texID,sh](){
        sh->setInt("texture0", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,texID);
    });
    modelObj->setPosition(glm::vec3(0,0,-10));
    world::getInstance()->addChild(modelObj);
    world::getInstance()->getControl()->bindNode(modelObj);
    world::getInstance()->addChild(sky);
    action* ro=new rotateBy(3,glm::vec3(0,30,0));
    action* act=new repeat(99,3,ro,new moveBy(1,glm::vec3(0.2,0,0)),new moveBy(1,glm::vec3(-0.2,0,0)));
    modelObj->runAction(act);
}
