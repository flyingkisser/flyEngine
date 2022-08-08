//
//  test_text.cpp
//  flyEngine
//
//  Created by joe on 11/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include "test_text.h"
#include "uiText.h"
#include "world.h"
#include "timerUtil.h"
#include "repeat.h"
#include "moveBy.h"
#include "scaleTo.h"
#include "state.h"

using namespace std;
using namespace flyEngine;

void test_text1(){
    uiText* textObj=new uiText("res/font/arial.ttf",14,"hello,world");
    textObj->setFontColor(glm::vec3(255,0,0));
    textObj->setPosition(glm::vec3(25,25,0));
    textObj->setFontSize(21);
    world::getInstance()->addChild(textObj);
    return;
    
    repeat* act=new repeat(99,1,new moveBy(1,glm::vec3(10,10,0)));
    textObj->runAction(act);

    uiText* textObj2=new uiText("res/font/Tahoma.ttf",16,"OpenGL text test!");
    textObj2->setFontColor(glm::vec3(0x58,0x5d,0x5d));
    textObj2->setPosition(glm::vec3(600,550,0));
    world::getInstance()->addChild(textObj2);
    
    repeat* act2=new repeat(99,2,new scaleTo(1,glm::vec3(2,2,1)),new scaleTo(1,glm::vec3(1,1,1)));
    textObj2->runAction(act2);
    
    timerUtil::getInstance()->execOnceDelay(2, [](uiText* textObj){
        textObj->setText("changed!");
    }, textObj);
   
    timerUtil::getInstance()->execOnceDelay(4, [](){
        state::setShowFrameRate(false);
        state::setShowDrawCall(false);
        state::setShowVertices(false);
    });
    timerUtil::getInstance()->execOnceDelay(6, [](uiText* textObj){
        state::setShowFrameRate(true);
        state::setShowDrawCall(true);
        state::setShowVertices(true);
        world::getInstance()->removeChild(textObj);
    },textObj);
}
