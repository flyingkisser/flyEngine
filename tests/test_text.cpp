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



using namespace std;
using namespace flyEngine;

void test_text1(){
    uiText* textObj=new uiText("res/font/Tahoma.ttf",14,"hello,world");
    textObj->setPosition(glm::vec3(50,60,0));
    world::getInstance()->addChild(textObj);
}
