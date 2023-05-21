//
//  test_areaLight.cpp
//  flyEngine
//
//  Created by joe on 22/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#include "test_areaLight.h"
#include "material2.h"
#include "world.h"
#include "control.h"
#include "light.h"
#include "pointLight.h"
#include "areaLight.h"
#include "logUtil.h"
#include "timerUtil.h"
#include "timeUtil.h"
#include "directionLight.h"
#include "cubeTex.h"

#include "shader.h"
#include "shaderMgr.h"
#include "randUtil.h"
#include "threadUtil.h"
#include "camera.h"
#include "glslUtil.h"

#include "colors.h"
#include "ltc_matrix.h"

USE_NS_FLYENGINE;

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

static void init_light_direction(){
    //设置环境光
    directionLight* dirLight=new directionLight(glm::vec3(0.2f,0.2f,0.2f));
    world::getInstance()->setDirectiontLight(dirLight);
    
    //因为只有环境光，所以设置的比较亮
    //dirLight->setMaterial(createMaterial(1.0,1.0,1.0,0.2));
}

void test_areaLight_multi(){
    //init_light_direction();
    camera* cam=world::getInstance()->getCamera();
    cam->setPosition(glm::vec3(0,1,0.5));
    cam->setYaw(180.0f);
   
    //普通cube
    cubeTex* planeObj=new cubeTex("res/concrete.png");
    int descArr[]={3,2,3};
    if(!planeObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane3,sizeof(g_verticeArrWithTexCoordAndNormal_plane3),descArr,3)){
      flylog("cubeTex init failed!");
      return;
    }
    shader* shAreaLight=new shader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex_areaLight.fs");
    planeObj->setShader(shAreaLight);
    planeObj->setPosition(glm::vec3(0,0,-6));
    world::getInstance()->addChild(planeObj);
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(planeObj);

    //光源1
    areaLight* lightObj1=new areaLight(Color::IndianRed,glm::vec3(0.0,1.0,0.0),nullptr);
    if(!lightObj1->init()){
       flylog("lightObj1 init failed!");
       return;
    }
    //lightObj1->setPosition(glm::vec3(0,0,-2));
    world::getInstance()->addAreaLight(lightObj1);
    controlObj->bindNodeWithCameraMove(lightObj1);
//    lightObj1->update(0);
    
    //光源2
    areaLight* lightObj2=new areaLight(Color::WhiteSmoke,glm::vec3(0.0,1.0,0.0),nullptr);
    if(!lightObj2->init()){
       flylog("lightObj2 init failed!");
       return;
    }
    lightObj2->setPosition(glm::vec3(0.5,0.5,-2));
    world::getInstance()->addAreaLight(lightObj2);
//    controlObj->bindNodeWithCameraMove(lightObj2);

    //光源3
    areaLight* lightObj3=new areaLight(Color::WhiteSmoke,glm::vec3(0.0,1.0,0.0),nullptr);
    if(!lightObj3->init()){
       flylog("lightObj2 init failed!");
       return;
    }
    lightObj3->setPosition(glm::vec3(0.5,0.5,-4));
    world::getInstance()->addAreaLight(lightObj3);
//    controlObj->bindNodeWithCameraMove(lightObj3);
    
    //光源4
    areaLight* lightObj4=new areaLight(Color::WhiteSmoke,glm::vec3(0.0,1.0,0.0),nullptr);
    if(!lightObj4->init()){
       flylog("lightObj4 init failed!");
       return;
    }
    lightObj4->setPosition(glm::vec3(0.5,0.5,-6));
    world::getInstance()->addAreaLight(lightObj4);
//    controlObj->bindNodeWithCameraMove(lightObj4);
    
    shAreaLight->use();
    shAreaLight->setInt("texture0",0);
    shAreaLight->setInt("texLTC1",1);
    shAreaLight->setInt("texLTC2",2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, lightObj1->getTex1());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, lightObj1->getTex2());
  
//    world::getInstance()->setCBBeforeDrawCall([planeObj](){
//        planeObj->draw();
//    });
    
    control* conObj=world::getInstance()->getControl();
    conObj->bindNodeWithCameraMove(lightObj1);
    conObj->regOnKeyPress('9', [lightObj1](){
        lightObj1->setRoughness(lightObj1->getRoughness()+0.1);
    });
    conObj->regOnKeyPress('0', [lightObj1](){
        lightObj1->setRoughness(lightObj1->getRoughness()-0.1);
    });
    conObj->regOnKeyPress('1', [lightObj1](){
        lightObj1->setIntensity(lightObj1->getIntensity()+0.1);
    });
    conObj->regOnKeyPress('2', [lightObj1](){
        lightObj1->setIntensity(lightObj1->getIntensity()-0.1);
    });
    conObj->regOnKeyPress('3', [lightObj1](){
        lightObj1->setTowSided(!lightObj1->getTowSided());
    });
    conObj->regOnKeyPress('g', [lightObj1](){
//        flylogVec3Float(lightObj1->getPosition());
        world::getInstance()->printRenderTime();
    });
    
    planeObj->getShader()->use();
    glslUtil::printAllUniformAndBlock(planeObj->getShader()->getProgramID());
    
}
