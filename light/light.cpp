//
//  light.cpp
//  flyEngine
//
//  Created by joe on 10/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "light.h"
#include "material2.h"
#include "shader.h"
#include "shaderMgr.h"

USE_NS_FLYENGINE

light::light(glm::vec3 color,material2* mt):cubeColor(color){
    setMaterial(mt);
}

light::light(glm::vec3 color):cubeColor(color){
    setMaterial(new material2(glm::vec3(1,1,1),glm::vec3(1,1,1),glm::vec3(1,1,1),1.0f));
}

bool light::init(){
    return cubeColor::init();
}

void light::draw(){
    cubeColor::draw();
}

void light::setColor(glm::vec3 color){
    cubeColor::setColor(color);
}
