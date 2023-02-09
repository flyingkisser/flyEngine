//
//  test_sprite_quad.cpp
//  flyEngine
//
//  Created by joe on 19/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_sprite_quad.h"
#include "sprite.h"
//#include "quad.h"
#include "world.h"
#include "timerUtil.h"
#include "window.h"
#include "rotateBy.h"
#include "moveBy.h"
#include "forever.h"
#include "quadColor.h"
#include "camera.h"
#include "control.h"

USE_NS_FLYENGINE
void test_sprite_1(){
    sprite* sp=new sprite("res/wood.png");
    world::getInstance()->addChild(sp);
    timerUtil::getInstance()->execOnceDelay(3, [](sprite* sp){
        flyEngine::size s=sp->getContentSize();
        sp->setPosition(glm::vec3((g_winWidth-s.width)/2,(g_winHigh-s.height)/2,0));
    }, sp);
    timerUtil::getInstance()->execOnceDelay(5, [](sprite* sp){
        flyEngine::size s=sp->getContentSize();
        sp->setContentSize(flyEngine::size{s.width+20.0f,s.height+10.0f});
    }, sp);
    timerUtil::getInstance()->execOnceDelay(7, [](sprite* sp){
        sp->setContentSize(flyEngine::size{(float)g_winWidth,(float)g_winHigh});
        sp->setPosition(glm::vec3(0,0,0));
    }, sp);
}

void test_sprite_2(){
    sprite* sp=new sprite("res/wood.png");
    world::getInstance()->addChild(sp);
    timerUtil::getInstance()->execOnceDelay(1.5, [](sprite* sp){
        flyEngine::size s=sp->getContentSize();
        sp->setPosition(glm::vec3((g_winWidth-s.width)/2,(g_winHigh-s.height)/2,0));
        rotateBy* r=new rotateBy(1,glm::vec3(0,0,60));
        moveBy* m1=new moveBy(1,glm::vec3(100,50,0));
        moveBy* m2=new moveBy(1,glm::vec3(-100,-50,0));
        sp->runAction(new forever(3,r,m1,m2));
    }, sp);
}

//void test_quad_1(){
//    quad* sp=new quad("res/wood.png");
//    world::getInstance()->addChild(sp);
//    timerUtil::getInstance()->execOnceDelay(3, [](sprite* sp){
//        flyEngine::size s=sp->getContentSize();
//        sp->setPosition(glm::vec3((g_winWidth-s.width)/2,(g_winHigh-s.height)/2,0));
//    }, sp);
//    timerUtil::getInstance()->execOnceDelay(5, [](sprite* sp){
//        flyEngine::size s=sp->getContentSize();
//        sp->setContentSize(flyEngine::size{s.width+20.0f,s.height+10.0f});
//    }, sp);
//}

void test_quad_color(){
    quadColor* sp1=new quadColor(glm::vec4(1,0,0,1),10,20);
    quadColor* sp2=new quadColor(glm::vec4(0,1,0,1),10,20);
    quadColor* sp3=new quadColor(glm::vec4(0,0,1,1),10,20);
    
    sp1->setPosition(glm::vec3(0,0,-9));
    sp1->setRotation(glm::vec3(0,30,0));
    
    sp2->setPosition(glm::vec3(0,0,-10));
    sp2->setRotation(glm::vec3(0,30,0));
    
    sp3->setPosition(glm::vec3(0,0,-11));
    sp3->setRotation(glm::vec3(0,30,0));

    world::getInstance()->addChild(sp1);
    world::getInstance()->addChild(sp2);
    world::getInstance()->addChild(sp3);

    world::getInstance()->getCamera()->getControl()->bindNode(sp1);
    world::getInstance()->getCamera()->getControl()->bindNode(sp2);
    world::getInstance()->getCamera()->getControl()->bindNode(sp3);
}
