//
//  test_model.cpp
//  flyEngine
//
//  Created by joe on 30/05/2022.
//  Copyright © 2022 joe. All rights reserved.
//


#include <math.h>
#include "defines.h"
#include "test_model.h"
#include "textureMgr.h"
#include "VAOMgr.h"
#include "shader.h"
#include "shaderMgr.h"

//#include "keyboardEventMgr.h"
//#include "mouseEventMgr.h"
#include "camera.h"
#include "control.h"
#include "world.h"

#include "material2.h"

#include "cubeTex.h"
#include "cubeColor.h"
#include "model.h"
#include "pointLight.h"
#include "directionLight.h"

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
#include "timerUtil.h"
#include "threadUtil.h"
#include "window.h"
#include "timeUtil.h"

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
//    return new material(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
    return NULL;
}


static void init_light_direction(){
    //设置环境光
//    directionLight* dirLight=new directionLight(glm::vec3(1,1,1),glm::vec3(-0.2,-1,-0.3),0.1,0.5,0.1,8);
    glm::vec3 light_pos=glm::vec3(0,0.2,0.2);
    glm::vec3 light_dir=light_pos-glm::vec3(0,0,0);
    directionLight* dirLight=new directionLight(glm::vec3(1,1,1),light_dir,0.1,0.5,0.8,8);
    world::getInstance()->setDirectiontLight(dirLight);

    //因为只有环境光，所以设置的比较亮
//    dirLight->setMaterial(createMaterial(1.0,1.0,1.0,0.2));
}

void test_one_model(){
    init_light_direction();
    
    pointLight* pLight=new pointLight(glm::vec3(1,1,1),createMaterial(1.0, 1.0, 1.0, 2));
    if(!pLight->init()){
         flylog("pLight init failed!");
         return;
    }
    pLight->setPosition(glm::vec3(-0.7,0.6,0));
    world::getInstance()->addPointLight(pLight);

    model* modelObj=new model("res/model/backpack/backpack.obj");
    modelObj->setPosition(glm::vec3(0,0,-0.5));
    if(!modelObj->init()){
       flylog("modelObj init failed!");
       return;
    }
    world::getInstance()->addChild(modelObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(modelObj);

    timerUtil* timerMgrObj=new timerUtil("model_test_timer");
    timerMgrObj->exec(0.1,[](node* _node,node* lightObj){
        _node->rotateBy(glm::vec3(0,0.5f,0));
        float radius=timeUtil::getTimeFloatSinceRun();
        lightObj->setPositionX(2*cos(radius));
        lightObj->setPositionZ(8*sin(radius));
    },modelObj,pLight);
}

