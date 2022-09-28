//
//  test_ubo.cpp
//  flyEngine
//
//  Created by joe on 28/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_ubo.h"
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
void test_ubo_1(){
//    skybox* sky=new skybox("res/skybox/right.jpg","res/skybox/left.jpg","res/skybox/top.jpg",
//                           "res/skybox/bottom.jpg","res/skybox/front.jpg","res/skybox/back.jpg");
//    world::getInstance()->addChild(sky);

    shader* sh=new shader("./res/shader/3d_1tex_ubo.vs","./res/shader/3d_1tex.fs");
    
    camera* cam=world::getInstance()->getCamera();
    cam->initUBO();

    cubeTex* cube1=new cubeTex("res/wood.png");
    cube1->setShader(sh);
    cube1->init();
    cube1->setPosition(glm::vec3(0,0,-10));
    cube1->setRotation(glm::vec3(0,30,30));
    world::getInstance()->addChild(cube1);
}
