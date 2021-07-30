//
//  test_action.cpp
//  flyEngine
//
//  Created by joe on 29/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "test_action.h"
#include "defines.h"

#include "texture.h"
#include "textureMgr.h"
#include "VAOMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "keyboardEventMgr.h"
#include "mouseEventMgr.h"
#include "camera.h"
#include "control.h"
#include "world.h"
#include "node.h"
#include "moveBy.h"
#include "scaleBy.h"
#include "scaleTo.h"
#include "rotateTo.h"
#include "rotateBy.h"

#include "sequence.h"
#include "spawn.h"
#include "repeat.h"
#include "forever.h"


#include "logUtil.h"
#include "timerMgr.h"
#include "threadUtil.h"
#include "window.h"

USE_NS_FLYENGINE

void test_actionMove(){
    node* nodeObj=new node("res/fire.png");
    if(!nodeObj->init()){
        flylog("node init failed!");
        return;
    }
    nodeObj->setPosition(glm::vec3(0,0,-5));
       
    world::getInstance()->addChild(nodeObj);
 
    action* moveAct=new flyEngine::moveBy(1,glm::vec3(0.5,0,-5));
    nodeObj->runAction(moveAct);
}

void test_actionSequence(){
    node* nodeObj=new node("res/fire.png");
    if(!nodeObj->init()){
        flylog("node init failed!");
        return;
    }
    nodeObj->setPosition(glm::vec3(0,0,-5));
    world::getInstance()->addChild(nodeObj);
 
    action* moveAct1=new moveBy(1,glm::vec3(0.5,0,0));
    action* moveAct2=new rotateTo(1,glm::vec3(0.5,0.5,1));
    action* moveAct3=new scaleTo(1,glm::vec3(1.5,1.5,1));
    
    flyEngine::sequence* seq=new flyEngine::sequence(3,moveAct1,moveAct2,moveAct3);
    
    nodeObj->runAction(seq);
}

void test_actionSpawn(){
    node* nodeObj=new node("res/fire.png");
    if(!nodeObj->init()){
        flylog("node init failed!");
        return;
    }
    nodeObj->setPosition(glm::vec3(0,0,-5));
    world::getInstance()->addChild(nodeObj);
    
    action* moveAct1=new moveBy(1,glm::vec3(0.5,0,0));
    action* moveAct2=new moveBy(3,glm::vec3(0.5,0.5,0.5));
    action* moveAct3=new scaleTo(5,glm::vec3(0.3,0.3,1));
    flyEngine::spawn* spawnAct=new flyEngine::spawn(3,moveAct1,moveAct2,moveAct3);
    
    nodeObj->runAction(spawnAct);
}

void test_actionRepeat(){
    node* nodeObj=new node("res/fire.png");
    if(!nodeObj->init()){
        flylog("node init failed!");
        return;
    }
    nodeObj->setPosition(glm::vec3(0,0,-5));
    world::getInstance()->addChild(nodeObj);
    
    action* act1=new moveBy(1,glm::vec3(0.5,0,0));
    action* act2=new moveBy(1,glm::vec3(0,0.5,0));
    action* act3=new scaleBy(1,glm::vec3(0.1,0.1,0.1));
    repeat* multiAct=new repeat(3,2,act1,act2,act3);
    
    nodeObj->runAction(multiAct);
}

void test_actionForever(){
    node* nodeObj=new node("res/fire.png");
    if(!nodeObj->init()){
        flylog("node init failed!");
        return;
    }
    nodeObj->setPosition(glm::vec3(0,0,-5));
    world::getInstance()->addChild(nodeObj);
    
    action* act1=new moveBy(1,glm::vec3(0.5,0,0));
    action* act2=new moveBy(1,glm::vec3(-0.5,0,0));
    action* act3=new scaleBy(2,glm::vec3(0.1,0.1,0.1));
    action* act4=new scaleBy(2,glm::vec3(-0.1,-0.1,-0.1));
    forever* multiAct=new flyEngine::forever(4,act1,act2,act3,act4);
    
    nodeObj->runAction(multiAct);
}

void test_actionForeverAndStop(){
    node* nodeObj=new node("res/fire.png");
    if(!nodeObj->init()){
        flylog("node init failed!");
        return;
    }
    nodeObj->setPosition(glm::vec3(0,0,-5));
    world::getInstance()->addChild(nodeObj);
    
    action* act1=new moveBy(1,glm::vec3(0.5,0,0));
    action* act2=new moveBy(1,glm::vec3(-0.5,0,0));
    action* act3=new scaleBy(2,glm::vec3(0.1,0.1,0.1));
    action* act4=new scaleBy(2,glm::vec3(-0.1,-0.1,-0.1));
    forever* multiAct=new flyEngine::forever(4,act1,act2,act3,act4);
    
    nodeObj->runAction(multiAct);
    
    int id=timerMgr::getInstance()->execOnceDelay(3, [&](node* nodeObj,action* act){
        nodeObj->stopAction(act);
    },nodeObj,multiAct);
    
    timerMgr::getInstance()->execOnceDelay(2.9, [&](int timerID){
        timerMgr::getInstance()->stop(timerID);
    },id);
}
