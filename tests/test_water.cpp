//
//  test_water.cpp
//  flyEngine
//
//  Created by joe on 12/05/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#include "test_water.h"
#include "sprite.h"
//#include "quad.h"
#include "world.h"
#include "timerUtil.h"
#include "window.h"
#include "rotateBy.h"
#include "moveBy.h"
#include "forever.h"
#include "quadColor.h"
#include "quadSize.h"
#include "camera.h"
#include "control.h"
#include "shader.h"
#include "texture2.h"
#include "quad.h"
#include "quadSize.h"
#include "cubeTex.h"
#include "logUtil.h"
#include "fbo.h"
#include "skybox.h"
#include "timeUtil.h"
#include "timerUtil.h"
#include "light.h"
#include "directionLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "material2.h"

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

static void init_light_direction(){
    //设置环境光
    directionLight* dirLight=new directionLight(glm::vec3(0.7f,0.7f,0.7f),glm::vec3(-0.2,-1,-0.3),0.1,0.1,0.1,2);
    world::getInstance()->setDirectiontLight(dirLight);
}

void test_water1(){
    quadColor* sp1=new quadColor(glm::vec4(0,0,1,1),10,20);

    sp1->setPosition(glm::vec3(0,0,-9));
    sp1->setRotation(glm::vec3(-82,-179,88));

    world::getInstance()->addChild(sp1);
   

    world::getInstance()->getCamera()->getControl()->bindNode(sp1);
    
    world::getInstance()->getControl()->regOnKeyPress('g', [sp1](){
        glm::vec3 pos=sp1->getRotation();
        flylog("%f %f %f",pos.x,pos.y,pos.z);
    });
}


void test_water2(){
    camera* cam=world::getInstance()->getCamera();
    fboWater st=fbo::createFBOWater();
    
    cam->setPosition(glm::vec3(0,0,11));
    cam->setYaw(-89.8);
    cam->setPitch(-4);
    
//    skybox* sky=new skybox("res/skybox/right.jpg","res/skybox/left.jpg","res/skybox/top.jpg",
//                           "res/skybox/bottom.jpg","res/skybox/front.jpg","res/skybox/back.jpg");
    skybox* sky=new skybox("res/skybox/top.jpg","res/skybox/top.jpg","res/skybox/top.jpg",
                           "res/skybox/top.jpg","res/skybox/top.jpg","res/skybox/top.jpg");
 
    cubeTex* floorObj=new cubeTex("./res/floor.png");
    int descArr[]={3,2,3};
    floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane, sizeof(g_verticeArrWithTexCoordAndNormal_plane), descArr, 3);
    floorObj->setPosition(glm::vec3(0,0,0));
    floorObj->setRotation(glm::vec3(15,-1.29,0));
    
    cubeTex* woodObj1=new cubeTex("./res/wood.png");
    woodObj1->init();
    woodObj1->setPosition(glm::vec3(-0.5,0.3,6));
    woodObj1->setScale(0.3);
    
    cubeTex* woodObj2=new cubeTex("./res/wood.png");
    woodObj2->init();
    woodObj2->setPosition(glm::vec3(0.5,0.55,5));
    woodObj2->setScale(0.3);
    
    cubeTex* woodObj3=new cubeTex("./res/wood.png");
    woodObj3->init();
    woodObj3->setPosition(glm::vec3(0.8,0.7,5.5));
    woodObj3->setScale(0.3);

    quadColor* spWater=new quadColor(glm::vec4(0,0,1,1),400,200);
    spWater->setPosition(glm::vec3(0,0.05,5));
    spWater->setRotation(glm::vec3(-82,-179,88));
    float heightWater=abs(spWater->getPositionY());
    
    quadSize* spSize1=new quadSize(st.tex1,1,1);
    spSize1->setPosition(glm::vec3(-2,2,0));
    quadSize* spSize2=new quadSize(st.tex2,1,1);
    spSize2->setPosition(glm::vec3(2,2,0));

    world::getInstance()->getControl()->bindNode(floorObj);
    world::getInstance()->getControl()->bindNode(spWater);
    
    shader* shDepth=new shader("./res/shader/depth_shadow.vs","./res/shader/draw_null.fs");
   
    
    shader* sh=new shader("./res/shader/3d_1tex_water.vs","./res/shader/3d_1tex_phong.fs");
    woodObj1->setShader(sh);
    woodObj2->setShader(sh);
    woodObj3->setShader(sh);
    floorObj->setShader(sh);
  
    texture2* texDuDv=new texture2("./res/waterDuDv.png");
    texDuDv->init();
    texDuDv->glInit();
    texture2* texNormal=new texture2("./res/waterNormal.png");
    texNormal->init();
    texNormal->glInit();
    
    shader* shWater=new shader("./res/shader/3d_quad_water.vs","./res/shader/3d_quad_water.fs");
    spWater->setShader(shWater);
    shWater->setInt("texReflection", 0);
    shWater->setInt("texReftraction", 1);
    shWater->setInt("texDuDv", 2);
    shWater->setInt("texNormal", 3,true);
    shWater->setInt("texDepth", 4,true);
 
    quadSize* spSize3=new quadSize(st.texDepth,1,1);
    spSize3->setPosition(glm::vec3(3,2,0));
    
    material2* mtLight=createMaterial(0.9, 0.9, 0.5, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!lightObj1->init()){
       flylog("point light init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(0,1.8,-0.9));
    lightObj1->setScale(glm::vec3(0.2,0.2,0.2));
    lightObj1->setVisible(false);
    world::getInstance()->addPointLight(lightObj1);
    
    shader* shDepth2d=new shader("./res/shader/2d_1tex.vs","./res/shader/2d_depth.fs");
    sprite* sp=new sprite(st.texDepth);
    sp->flipY(true);
    sp->setPosition(glm::vec3(3,2,0));
    sp->setShader(shDepth2d);
    sp->setScale(0.5);
    
//    spSize3->setShader(shDepth2d);
    
    world::getInstance()->setCBBeforeDrawCall([sp,texDuDv,texNormal,cam,heightWater,sh,shDepth,shWater,st,sky,floorObj,woodObj1,woodObj2,woodObj3,spWater,spSize1,spSize2,spSize3,lightObj1](){
        
        float time=timeUtil::getTimeFloatSinceRun();
        shWater->use();
        float moveFactor=shWater->getFloat("moveFactor");
        moveFactor+=0.0001*time;
        if(moveFactor>1.0){
            int moveInt=(int)moveFactor;
            moveFactor=moveFactor -moveInt;
        }
        shWater->setFloat("moveFactor", moveFactor);

        float move=2*(cam->getPositionY()-heightWater);
        cam->setPositionY(cam->getPositionY()-move);
        cam->setPitch(-cam->getPitch());
        cam->update();
        
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo3);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
        cam->updateUBOForShadow(lightObj1->getPosition());
        floorObj->setShader(shDepth);
        woodObj1->setShader(shDepth);
        woodObj2->setShader(shDepth);
        woodObj3->setShader(shDepth);
        
        floorObj->draw();
        woodObj1->draw();
        woodObj2->draw();
        woodObj3->draw();
        spWater->draw();
        
        
//        glViewport(0,0,g_winWidth,g_winHigh);
//        glBindFramebuffer(GL_FRAMEBUFFER,0);
//        glEnable(GL_DEPTH_TEST);
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        sp->draw();
//        return;
        
        
        floorObj->setShader(sh);
        woodObj1->setShader(sh);
        woodObj2->setShader(sh);
        woodObj3->setShader(sh);
        glViewport(0,0,g_winWidth,g_winHigh);
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo1);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CLIP_DISTANCE0);
        sh->use();//只保留水平面以上的顶点，水面反射
        sh->setVec4("plane",glm::vec4(0,1,0,-heightWater));
        sky->draw();
        floorObj->draw();
        woodObj1->draw();
        woodObj2->draw();
        woodObj3->draw();
        spWater->draw();
        
        cam->setPositionY(cam->getPositionY()+move);
        cam->setPitch(-cam->getPitch());
        cam->update();
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo2);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CLIP_DISTANCE0);
        sh->use();//只保留水平面以下的顶点
        sh->setVec4("plane",glm::vec4(0,-1,0,heightWater));
        sky->draw();
        floorObj->draw();
        woodObj1->draw();
        woodObj2->draw();
        woodObj3->draw();
        spWater->draw();
        
        //最后正常绘制场景，同时绘制水面
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sh->use();
        sh->setVec4("plane",glm::vec4(0,-1,0,100000));
        sky->draw();
        lightObj1->draw();
        floorObj->draw();
        woodObj1->draw();
        woodObj2->draw();
        woodObj3->draw();
       
        shWater->use();
        shWater->setFloat("far_plane", cam->getFarPlane());
        shWater->setFloat("far_near", cam->getNearPlane());
  
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,st.tex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,st.tex2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,texDuDv->getTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,texNormal->getTextureID());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,st.texDepth);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        spWater->draw();
        
//        spSize1->draw();
//        spSize2->draw();
        sp->draw();
        glDisable(GL_BLEND);
        
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g', [floorObj,cam,lightObj1](){
        glm::vec3 pos=floorObj->getRotation();
        flylog("floor %f %f %f",pos.x,pos.y,pos.z);
        glm::vec3 pos2=cam->getPosition();
        flylog("cam %f %f %f",pos2.x,pos2.y,pos2.z);
        flylog("yaw %f pitch %f",cam->getYaw(),cam->getPitch());
        glm::vec3 pos3=lightObj1->getPosition();
        flylog("lightObj1 %f %f %f",pos3.x,pos3.y,pos3.z);
    });
  
    pointLight* plight=lightObj1;
    world::getInstance()->getControl()->regOnKeyPress('1', [plight](){
        glm::vec3 pos=plight->getPosition();
        plight->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('2', [plight](){
        glm::vec3 pos=plight->getPosition();
        plight->setPositionX(pos.x-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('3', [plight](){
        glm::vec3 pos=plight->getPosition();
        plight->setPositionY(pos.y+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('4', [plight](){
        glm::vec3 pos=plight->getPosition();
        plight->setPositionY(pos.y-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('5', [plight](){
        glm::vec3 pos=plight->getPosition();
        plight->setPositionZ(pos.z+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('6', [plight](){
        glm::vec3 pos=plight->getPosition();
        plight->setPositionZ(pos.z-0.1);
    });
    
}
