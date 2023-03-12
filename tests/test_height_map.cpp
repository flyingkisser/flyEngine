//
//  test_height_map.cpp
//  flyEngine
//
//  Created by joe on 10/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#include "test_height_map.h"
#include "world.h"
#include "window.h"
#include "cubeTex.h"
#include "logUtil.h"
#include "mathUtil.h"
#include "shader.h"
#include "model.h"
#include "material2.h"
#include "camera.h"
#include "pointLight.h"
#include "timerUtil.h"
#include "timeUtil.h"
#include "glslUtil.h"
#include "control.h"
#include "forever.h"
#include "rotateBy.h"
#include "fbo.h"
#include "sprite.h"
#include "quad.h"
#include "quadColor.h"
#include "shaderMgr.h"
#include "heightMap.h"

#ifdef BUILD_IOS
#import "ViewController.h"
#endif

USE_NS_FLYENGINE

void test_height_map_by_cpu(){
    camera* cam=world::getInstance()->getCamera();
    cam->setPosition(glm::vec3(67.0f, 627.5f, 169.9f));
    cam->setFarPlane(100000.0);
    cam->setYaw(-128.1);
    cam->setPitch(-42.4);
//    cam->setScreenRatio(2560.0/1440.0);
    
//    heightMap* heightObj=new heightMap("res/heightmap/iceland_heightmap.png");
    heightMap* heightObj=new heightMap("res/heightmap/river2_heightmap.png");
//    heightMap* heightObj=new heightMap("res/heightmap/river2_heightmap.png");
    if(!heightObj->init()){
      flylog("heightObj init failed!");
      return;
    }
    heightObj->setPosition(glm::vec3(0,0,0));
    heightObj->setLineMode(true);
    world::getInstance()->addChild(heightObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(heightObj);
}

void test_height_map_by_gpu(){
    heightMap* heightObj=new heightMap("res/wood.png");
    if(!heightObj->init()){
      flylog("heightObj init failed!");
      return;
    }
    heightObj->setPosition(glm::vec3(0,0,0));
    world::getInstance()->addChild(heightObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(heightObj);
}
