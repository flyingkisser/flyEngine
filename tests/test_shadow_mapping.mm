//
//  test_shadow_mapping.cpp
//  flyEngine
//
//  Created by joe on 09/10/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_shadow_mapping.h"
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
#include "control.h"
#include "forever.h"
#include "rotateBy.h"
#include "fbo.h"
#include "sprite.h"
#include "shaderMgr.h"

#ifdef BUILD_IOS
#import "ViewController.h"
#endif

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

void test_shadow_mapping_1(){
    camera* cam=world::getInstance()->getCamera();
    cam->setPosition(glm::vec3(0,0,3));
    
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(-0.1,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    cubeTex* cubeObj2=new cubeTex("./res/floor.png");
    cubeObj2->init();
    cubeObj2->setPosition(glm::vec3(0.1,1.5,-3));
    cubeObj2->setRotation(glm::vec3(60,60,0));
    world::getInstance()->getControl()->bindNode(cubeObj2);
   
    
    world::getInstance()->addChild(cubeObj);
    world::getInstance()->addChild(cubeObj2);
}

//显示出深度图
void test_shadow_mapping_to_tex(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    cubeTex* cubeObj2=new cubeTex("./res/floor.png");
    cubeObj2->init();
    cubeObj2->setPosition(glm::vec3(0,2,-3));
    cubeObj2->setRotation(glm::vec3(60,60,0));
    world::getInstance()->getControl()->bindNode(cubeObj2);
    
    cubeTex* floorObj=new cubeTex("./res/floor.png");
    int descArr[]={3,2,3};
    if(!floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane,sizeof(g_verticeArrWithTexCoordAndNormal_plane),descArr,3)){
        flylog("init plainObj failed!");
        return;
    }
    floorObj->setPosition(glm::vec3(0,-1,0));
    floorObj->setRotation(glm::vec3(10,0,0));

    fboStruct stDepth=fbo::createFBOForDepth();
    sprite* sp=new sprite(stDepth.texID);
    sp->flipY(true);
    glm::vec3 lightPos=glm::vec3(0,3,1);
    
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0,0.5,0.5));
    plight->setScale(glm::vec3(0.2,0.2,0.2));
    plight->setVisible(false);
    world::getInstance()->addPointLight(plight);
    
    pointLight* plight2=new pointLight(glm::vec3(0.8,0.9,0.8),mtLight,0);
    if(!plight2->init()){
       flylog("point light init failed!");
       return;
    }
    plight2->setPosition(glm::vec3(0,0,-4));
    plight2->setScale(glm::vec3(0.2,0.2,0.2));
    plight2->setVisible(false);
    world::getInstance()->addPointLight(plight);
    
    cam->setPosition(glm::vec3(0,0,3));
    glm::vec3 camPos=glm::vec3(0,4,12.7);
    cam->setPosition(camPos);
    cam->setYaw(-89);
    cam->setPitch(-16);
    cam->updateUBOForShadow(plight->getPosition());
    
    world::getInstance()->setCBBeforeAnyGLCall([stDepth](){
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
//        glViewport(0,0,g_winHigh,g_winHigh);
        glBindFramebuffer(GL_FRAMEBUFFER,stDepth.fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    shader* shDepth=new shader("./res/shader/depth_shadow.vs","./res/shader/draw_null.fs");
    shader* sh=cubeObj->getShader();
    shader* shDepth2d=new shader("./res/shader/2d_1tex.vs","./res/shader/2d_depth.fs");
    sp->setShader(shDepth2d);

    world::getInstance()->setCBBeforeDrawCall([stDepth,shDepth,sh,cam,sp,cubeObj,cubeObj2,floorObj,plight,plight2,view](){
        cubeObj->setShader(shDepth);
        cubeObj->draw();
        cubeObj2->setShader(shDepth);
        cubeObj2->draw();
        floorObj->setShader(shDepth);
        floorObj->draw();
        plight->setShader(shDepth);
        plight->draw();
        plight2->setShader(shDepth);
        plight2->draw();
        
        glViewport(0,0,g_winWidth,g_winHigh);
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        cubeObj->setShader(sh);
//        glBindTexture(GL_TEXTURE_2D,stDepth.texID);
        sp->draw();
    });
}


//在world::getInstance()->setCBBeforeDrawCall()回调里自己对对象逐个渲染
void test_shadow_mapping_normal(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
//    world::getInstance()->getControl()->bindNode(cubeObj);
    
    cubeTex* cubeObj2=new cubeTex("./res/floor.png");
    cubeObj2->init();
    cubeObj2->setPosition(glm::vec3(0,2,-3));
    cubeObj2->setRotation(glm::vec3(60,60,0));
//    world::getInstance()->getControl()->bindNode(cubeObj2);
    
    cubeTex* floorObj=new cubeTex("./res/floor.png");
    int descArr[]={3,2,3};
    if(!floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane,sizeof(g_verticeArrWithTexCoordAndNormal_plane),descArr,3)){
        flylog("init plainObj failed!");
        return;
    }
    floorObj->setPosition(glm::vec3(0,-1,0));

    fboStruct stDepth=fbo::createFBOForDepth();
    sprite* sp=new sprite(stDepth.texID);
    sp->flipY(true);

    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0,0.5,0.5));
    plight->setScale(glm::vec3(0.2,0.2,0.2));
    plight->setVisible(false);
    world::getInstance()->addPointLight(plight);
    
    pointLight* plight2=new pointLight(glm::vec3(0.8,0.1,0.1),mtLight,0);
    if(!plight2->init()){
       flylog("point light init failed!");
       return;
    }
    plight2->setPosition(glm::vec3(0,0,-4));
    plight2->setScale(glm::vec3(0.2,0.2,0.2));
    plight2->setVisible(false);
    world::getInstance()->addPointLight(plight2);
    
    glm::vec3 camPos=glm::vec3(0,4,12.7);
    glm::vec3 lightViewPos=plight->getPosition();
    cam->setPosition(camPos);
    cam->setYaw(-89);
    cam->setPitch(-16);
    cam->updateUBOForShadow(lightViewPos);
  
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    shader* shDepth=new shader("./res/shader/depth_shadow.vs","./res/shader/draw_null.fs");
    shader* shNormal=cubeObj->getShader();
    
    float near_plane=1.0f;
    float far_plane=7.5f;
    glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
    glm::mat4 matCamera=glm::lookAt(lightViewPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));   //look at center of scene
    glm::mat4 matLightSpace=matProj*matCamera;
    
    shNormal->use();
    shNormal->setBool("bCheckShadow",true,true);
    shNormal->setMat4("matLightSpace",glm::value_ptr(matLightSpace),true);
    glActiveTexture(GL_TEXTURE0+3);
    glBindTexture(GL_TEXTURE_2D, stDepth.texID);
    shNormal->setInt("texture_shadow",texture_shadow,true);
   
    
    world::getInstance()->setCBBeforeAnyGLCall([stDepth](){
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
        glBindFramebuffer(GL_FRAMEBUFFER,stDepth.fbo);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });
    
    world::getInstance()->setCBBeforeDrawCall([stDepth,shDepth,shNormal,cam,sp,cubeObj,cubeObj2,floorObj,plight,plight2,view](){
        //first pass
        cubeObj->setShader(shDepth);
        cubeObj->draw();
        cubeObj2->setShader(shDepth);
        cubeObj2->draw();
        floorObj->setShader(shDepth);
        floorObj->draw();
        plight->setShader(shDepth);
        plight->draw();
        plight2->setShader(shDepth);
        plight2->draw();

        //second pass
        glViewport(0,0,g_winWidth,g_winHigh);
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cubeObj->setShader(shNormal);
        cubeObj->draw();
        cubeObj2->setShader(shNormal);
        cubeObj2->draw();
        floorObj->setShader(shNormal);
        floorObj->draw();
        plight->setShader(shNormal);
        plight->draw();
        plight2->setShader(shNormal);
        plight2->draw();
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
    
    timerUtil::getInstance()->exec(0.2, [](camera* cam){
        float v=0.06;
        cam->setYaw(cam->getYaw()+v*360/(2*3.1415));
        float x0=cam->getPositionX();
        float z0=cam->getPositionZ();
 
        float x1=x0*cos(v)-z0*sin(v);
        float z1=z0*cos(v)+x0*sin(v);
        
        cam->setPositionX(x1);
        cam->setPositionZ(z1);
    }, cam);
}

//使用world::getInstance()->addPass()函数建立渲染到指定pass的流程
void test_shadow_mapping_use_pass(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
//    world::getInstance()->getControl()->bindNode(cubeObj);
    
    cubeTex* cubeObj2=new cubeTex("./res/floor.png");
    cubeObj2->init();
    cubeObj2->setPosition(glm::vec3(0,2,-3));
    cubeObj2->setRotation(glm::vec3(60,60,0));
//    world::getInstance()->getControl()->bindNode(cubeObj2);
    
    cubeTex* floorObj=new cubeTex("./res/floor.png");
    int descArr[]={3,2,3};
    if(!floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane,sizeof(g_verticeArrWithTexCoordAndNormal_plane),descArr,3)){
        flylog("init plainObj failed!");
        return;
    }
    floorObj->setPosition(glm::vec3(0,-1,0));

    fboStruct stDepth=fbo::createFBOForDepth();
    sprite* sp=new sprite(stDepth.texID);
    sp->flipY(true);

    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0,0.5,0.5));
    plight->setScale(glm::vec3(0.2,0.2,0.2));
//    plight->setVisible(false);
    world::getInstance()->addPointLight(plight);
    
    pointLight* plight2=new pointLight(glm::vec3(0.8,0.1,0.1),mtLight,0);
    if(!plight2->init()){
       flylog("point light init failed!");
       return;
    }
    plight2->setPosition(glm::vec3(0,0,-4));
    plight2->setScale(glm::vec3(0.2,0.2,0.2));
//    plight2->setVisible(false);
    world::getInstance()->addPointLight(plight2);
    
    glm::vec3 camPos=glm::vec3(0,4,12.7);
    glm::vec3 lightViewPos=plight->getPosition();
    cam->setPosition(camPos);
    cam->setYaw(-89);
    cam->setPitch(-16);
    cam->updateUBOForShadow(lightViewPos);
  
    shader* shDepth=new shader("./res/shader/depth_shadow.vs","./res/shader/draw_null.fs");
    shader* shNormal=cubeObj->getShader();
    
    float near_plane=1.0f;
    float far_plane=7.5f;
    glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
    glm::mat4 matCamera=glm::lookAt(lightViewPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));   //look at center of scene
    glm::mat4 matLightSpace=matProj*matCamera;
    
    shNormal->use();
    shNormal->setBool("bCheckShadow",true,true);
    shNormal->setMat4("matLightSpace",glm::value_ptr(matLightSpace),true);
    glActiveTexture(GL_TEXTURE0+3);
    glBindTexture(GL_TEXTURE_2D, stDepth.texID);
    shNormal->setInt("texture_shadow",texture_shadow,true);
    
    world::getInstance()->addChild(cubeObj);
    world::getInstance()->addChild(cubeObj2);
    world::getInstance()->addChild(floorObj);

    world::getInstance()->addPass(stDepth.fbo, shDepth, [stDepth](){
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
    });
    world::getInstance()->addPass(0, shNormal, [stDepth](){
        glViewport(0,0,g_winWidth,g_winHigh);
    });
    
    
    timerUtil::getInstance()->exec(0.2, [](camera* cam){
        float v=0.06;
        cam->setYaw(cam->getYaw()+v*360/(2*3.1415));
        float x0=cam->getPositionX();
        float z0=cam->getPositionZ();
 
        float x1=x0*cos(v)-z0*sin(v);
        float z1=z0*cos(v)+x0*sin(v);
        
        cam->setPositionX(x1);
        cam->setPositionZ(z1);
    }, cam);
}

//先渲染出深度图，然后灯光不移动
void test_shadow_mapping_static_shadow(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
//    world::getInstance()->getControl()->bindNode(cubeObj);
    
    cubeTex* cubeObj2=new cubeTex("./res/floor.png");
    cubeObj2->init();
    cubeObj2->setPosition(glm::vec3(0,2,-3));
    cubeObj2->setRotation(glm::vec3(60,60,0));
//    world::getInstance()->getControl()->bindNode(cubeObj2);
    
    cubeTex* floorObj=new cubeTex("./res/floor.png");
    int descArr[]={3,2,3};
    if(!floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane,sizeof(g_verticeArrWithTexCoordAndNormal_plane),descArr,3)){
        flylog("init plainObj failed!");
        return;
    }
    floorObj->setPosition(glm::vec3(0,-1,0));

    fboStruct stDepth=fbo::createFBOForDepth();
    sprite* sp=new sprite(stDepth.texID);
    sp->flipY(true);

    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0,0.5,0.5));
    plight->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addPointLight(plight);
    
    pointLight* plight2=new pointLight(glm::vec3(0.8,0.9,0.8),mtLight,0);
    if(!plight2->init()){
       flylog("point light init failed!");
       return;
    }
    plight2->setPosition(glm::vec3(0,0,-4));
    plight2->setScale(glm::vec3(0.2,0.2,0.2));
    world::getInstance()->addPointLight(plight2);
    
    glm::vec3 camPos=glm::vec3(0,4,12.7);
    glm::vec3 lightViewPos=plight->getPosition();
    cam->setPosition(camPos);
    cam->setYaw(-89);
    cam->setPitch(-16);
    cam->updateUBOForShadow(lightViewPos);
  
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
   
    //绘制深度纹理
    shader* shNormal=cubeObj->getShader();
    shader* shDepth=new shader("./res/shader/depth_shadow.vs","./res/shader/draw_null.fs");
    glViewport(0,0,g_shadowWidth,g_shadowHigh);
    glBindFramebuffer(GL_FRAMEBUFFER,stDepth.fbo);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cubeObj->setShader(shDepth);
    cubeObj->draw();
    cubeObj2->setShader(shDepth);
    cubeObj2->draw();
    floorObj->setShader(shDepth);
    floorObj->draw();
    plight->setShader(shDepth);
    plight->draw();
    plight2->setShader(shDepth);
    plight2->draw();
    glViewport(0,0,g_winWidth,g_winHigh);
#ifdef BUILD_MAC
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
    [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif

    //配置启用阴影
    float near_plane=1.0f;
    float far_plane=7.5f;
    glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
    glm::mat4 matCamera=glm::lookAt(lightViewPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));   //look at center of scene
    glm::mat4 matLightSpace=matProj*matCamera;
    shNormal->use();
    shNormal->setBool("bCheckShadow",true,true);
    shNormal->setMat4("matLightSpace",glm::value_ptr(matLightSpace),true);
    glActiveTexture(GL_TEXTURE0+3);
    glBindTexture(GL_TEXTURE_2D, stDepth.texID);
    shNormal->setInt("texture_shadow",texture_shadow,true);
    cubeObj->setShader(shNormal);
    cubeObj2->setShader(shNormal);
    floorObj->setShader(shNormal);
    plight->setShader(shNormal);
    plight2->setShader(shNormal);
    world::getInstance()->addChild(cubeObj);
    world::getInstance()->addChild(cubeObj2);
    world::getInstance()->addChild(floorObj);
 
    //相机旋转
    timerUtil::getInstance()->exec(0.2, [](camera* cam){
        float v=0.06;
        cam->setYaw(cam->getYaw()+v*360/(2*3.1415));
        float x0=cam->getPositionX();
        float z0=cam->getPositionZ();
        float x1=x0*cos(v)-z0*sin(v);
        float z1=z0*cos(v)+x0*sin(v);
        cam->setPositionX(x1);
        cam->setPositionZ(z1);
    }, cam);
    
}

//灯光一直在移动
void test_shadow_mapping_light_move(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
//    world::getInstance()->getControl()->bindNode(cubeObj);
    
    cubeTex* cubeObj2=new cubeTex("./res/floor.png");
    cubeObj2->init();
    cubeObj2->setPosition(glm::vec3(0,2,-3));
    cubeObj2->setRotation(glm::vec3(60,60,0));
//    world::getInstance()->getControl()->bindNode(cubeObj2);
    
    cubeTex* floorObj=new cubeTex("./res/floor.png");
    int descArr[]={3,2,3};
    if(!floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane,sizeof(g_verticeArrWithTexCoordAndNormal_plane),descArr,3)){
        flylog("init plainObj failed!");
        return;
    }
    floorObj->setPosition(glm::vec3(0,-1,0));

    fboStruct stDepth=fbo::createFBOForDepth();
    sprite* sp=new sprite(stDepth.texID);
    sp->flipY(true);

    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0,0.5,0.5));
    plight->setScale(glm::vec3(0.2,0.2,0.2));
//    plight->setVisible(false);
    world::getInstance()->addPointLight(plight);
    
    pointLight* plight2=new pointLight(glm::vec3(0.8,0.5,0.1),mtLight,0);
    if(!plight2->init()){
       flylog("point light init failed!");
       return;
    }
    plight2->setPosition(glm::vec3(0,0,-4));
    plight2->setScale(glm::vec3(0.2,0.2,0.2));
//    plight2->setVisible(false);
    world::getInstance()->addPointLight(plight2);
   
    glm::vec3 camPos=glm::vec3(0,4,12.7);
    glm::vec3 lightViewPos=plight->getPosition();
    cam->setPosition(camPos);
    cam->setYaw(-89);
    cam->setPitch(-16);
    cam->updateUBOForShadow(lightViewPos);
  
    shader* shNormal=cubeObj->getShader();
    shader* shDepth=new shader("./res/shader/depth_shadow.vs","./res/shader/draw_null.fs");
//    shader* shNormal=new shader("./res/shader/3d_1tex_phong.vs","./res/shader/3d_1tex_phong_shadow_bias.fs");
 
    float near_plane=1.0f;
    float far_plane=7.5f;
    glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
    glm::mat4 matCamera=glm::lookAt(lightViewPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));   //look at center of scene
    glm::mat4 matLightSpace=matProj*matCamera;
    
    shNormal->use();
    shNormal->setBool("bCheckShadow",true,true);
    shNormal->setMat4("matLightSpace",glm::value_ptr(matLightSpace),true);
    glActiveTexture(GL_TEXTURE0+3);
    glBindTexture(GL_TEXTURE_2D, stDepth.texID);
    shNormal->setInt("texture_shadow",texture_shadow,true);
    
    world::getInstance()->addChild(cubeObj);
    world::getInstance()->addChild(cubeObj2);
    world::getInstance()->addChild(floorObj);

    world::getInstance()->addPass(stDepth.fbo, shDepth, [stDepth](){
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
    });
    world::getInstance()->addPass(0, shNormal, [stDepth](){
        glViewport(0,0,g_winWidth,g_winHigh);
    });
    
    world::getInstance()->setCBBeforeAnyGLCall([stDepth,shNormal,plight,cam](){
        glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,1.0f,7.5f);
        glm::mat4 matCamera=glm::lookAt(plight->getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));   //look at center of scene
        glm::mat4 matLightSpace=matProj*matCamera;
        shNormal->setMat4("matLightSpace",glm::value_ptr(matLightSpace),true);
        cam->updateUBOForShadow(plight->getPosition());
    });
 
    
    float r=1.6;
    plight->setPositionX(plight->getPositionX()*r);
    plight->setPositionZ(plight->getPositionZ()*r);
    timerUtil::getInstance()->exec(0.1, [](camera* cam, pointLight* plight,cubeTex* cubeObj){
        float v=0.08;
        float x0=0;
        float z0=0;
        float x1=0;
        float z1=0;
        //灯光移动
        x0=plight->getPositionX();
        z0=plight->getPositionZ();
        x1=x0*cos(v)-z0*sin(v);
        z1=z0*cos(v)+x0*sin(v);
        plight->setPositionX(x1);
        plight->setPositionZ(z1);
        //cube移动
//        cubeObj->moveBy(glm::vec3(0.1,0,0));
    }, cam,plight,cubeObj);
}


//灯光一直在移动
void test_shadow_cubemap_light_move(){
    camera* cam=world::getInstance()->getCamera();
    //house
    cubeTex* huouseObj=new cubeTex("./res/floor.png");
   
    int descArr[]={3,2,3};
    huouseObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormalReverse, sizeof(g_verticeArrWithTexCoordAndNormalReverse), descArr, 3);
    huouseObj->setScale(5.0f);
    world::getInstance()->addChild(huouseObj);
    
    cubeTex* cubeObj1=new cubeTex("./res/wood.png");
    cubeObj1->init();
    cubeObj1->setPosition(glm::vec3(0.1,0.1,0));
    cubeObj1->setScale(0.4);
    world::getInstance()->addChild(cubeObj1);
    
    cubeTex* cubeObj2=new cubeTex("./res/wood.png");
    cubeObj2->init();
    cubeObj2->setPosition(glm::vec3(0.2,0.2,0.1));
    cubeObj2->setScale(0.2);
    world::getInstance()->addChild(cubeObj2);

    cubeTex* cubeObj3=new cubeTex("./res/wood.png");
    cubeObj3->init();
    cubeObj3->setPosition(glm::vec3(-0.5,-0.5,0));
    cubeObj3->setScale(0.22);
    world::getInstance()->addChild(cubeObj3);

    cubeTex* cubeObj4=new cubeTex("./res/wood.png");
    cubeObj4->init();
    cubeObj4->setPosition(glm::vec3(-0.7,0.5,0.2));
    cubeObj4->setScale(0.3);
    world::getInstance()->addChild(cubeObj4);

    cubeTex* cubeObj5=new cubeTex("./res/wood.png");
    cubeObj5->init();
    cubeObj5->setPosition(glm::vec3(-1,1,0.3));
    cubeObj5->rotateBy(glm::vec3(60,0,60));
    cubeObj5->setScale(0.15);
    world::getInstance()->addChild(cubeObj5);
    
    fboStruct stDepth=fbo::createFBOForDepthWithCubemap();

    material2* mtLight=createMaterial(1, 1, 1, 1);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(-2.1,0,0.2));
   
    plight->setScale(0.02);
    world::getInstance()->addPointLight(plight);
    
    glm::vec3 lightViewPos=plight->getPosition();
    glm::vec3 camPos=glm::vec3(-2.2,0.1,0.3);
    cam->setPosition(camPos);
//    cam->setYaw(-89);
//    cam->setPitch(-16);
    cam->setYaw(-9.03);
    cam->setPitch(4.78);
    cam->updateUBOForShadow(lightViewPos);
  
    shader* shNormal=huouseObj->getShader();
    shader* shDepth=new shader("./res/shader/depth_cubetex_shadow.vs","./res/shader/depth_cubetex_shadow.fs","./res/shader/depth_cubetex_shadow.gs");
    glm::mat4 matProj=glm::perspective(glm::radians(90.0f),(float)g_shadowWidth/(float)g_shadowHigh,1.0f,25.0f);
    std::vector<glm::mat4> vecMat;
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(1,0,0), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(-1,0,0), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,1,0), glm::vec3(0,0,1)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,-1,0), glm::vec3(0,0,-1)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,1), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,-1), glm::vec3(0,-1,0)));
 
    
    shDepth->use();
    char szName[128]={0};
    for(int i=0;i<6;i++){
        snprintf(szName, sizeof(szName), "matShadowCube[%d]",i);
        shDepth->setMat4(szName, glm::value_ptr(vecMat[i]),true);
    }
    shDepth->setFloat("far_plane", 25.0f,true);
    shDepth->setVec3("posLight", glm::value_ptr(lightViewPos),true);
   
    shNormal->use();
    shNormal->setBool("bCheckShadowByCubemap",true,true);
    shNormal->setFloat("far_plane", 25.0f,true);
    glActiveTexture(GL_TEXTURE0+4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, stDepth.texID);
    shNormal->setInt("texture_depth_cube",texture_depth_cube,true);
//    shNormal->setBool("g_debug", true,true);
    shNormal->setBool("bUseBlinnPhong", true,true);
    world::getInstance()->getControl()->bindNode(huouseObj);

    world::getInstance()->addPass(stDepth.fbo, shDepth, [](){
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
    });
    world::getInstance()->addPass(0, shNormal, [](){
        glDisable(GL_CULL_FACE);
        glViewport(0,0,g_winWidth,g_winHigh);
    });
    
//    huouseObj->setCBBeforeDrawCall([](int progrmaID){
//        glDisable(GL_CULL_FACE);
//    });
    
//    cubeObj1->setCBBeforeDrawCall([](int progrmaID){
//        glEnable(GL_CULL_FACE);
//    });
    
    world::getInstance()->setCBBeforeAnyGLCall([stDepth,shDepth,plight,cam,matProj](){
//        glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,1.0f,7.5f);
//        glm::mat4 matCamera=glm::lookAt(plight->getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));   //look at center of scene
//        glm::mat4 matLightSpace=matProj*matCamera;
//        shNormal->setMat4("matLightSpace",glm::value_ptr(matLightSpace),true);
//        cam->updateUBOForShadow(plight->getPosition());
        
        std::vector<glm::mat4> vecMat;
        glm::vec3 lightViewPos=plight->getPosition();
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(1,0,0), glm::vec3(0,-1,0)));
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(-1,0,0), glm::vec3(0,-1,0)));
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,1,0), glm::vec3(0,0,1)));
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,-1,0), glm::vec3(0,0,-1)));
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,1), glm::vec3(0,-1,0)));
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,-1), glm::vec3(0,-1,0)));
        char szName[128]={0};
        for(int i=0;i<6;i++){
            snprintf(szName, sizeof(szName), "matShadowCube[%d]",i);
            shDepth->setMat4(szName, glm::value_ptr(vecMat[i]));
        }
    });

 
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
    
    float r=1.6;
    plight->setPositionX(plight->getPositionX()*r);
    plight->setPositionZ(plight->getPositionZ()*r);
    timerUtil::getInstance()->exec(0.1, [](camera* cam, pointLight* plight,cubeTex* cubeObj){
        float v=0.08;
        float x0=0;
        float z0=0;
        float x1=0;
        float z1=0;
        //灯光移动
        x0=plight->getPositionX();
        z0=plight->getPositionZ();
        x1=x0*cos(v)-z0*sin(v);
        z1=z0*cos(v)+x0*sin(v);
        plight->setPositionX(x1);
        plight->setPositionZ(z1);

        //cube移动
//        cubeObj1->moveBy(glm::vec3(0.1,0,0));
    }, cam,plight,cubeObj1);
}
