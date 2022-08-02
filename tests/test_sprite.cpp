//
//  test_sprite.cpp
//  flyEngine
//
//  Created by joe on 19/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_sprite.h"
#include "sprite.h"
#include "world.h"
#include "timerUtil.h"
#include "window.h"

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
}
