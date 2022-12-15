//
//  test_ssao.cpp
//  flyEngine
//
//  Created by Joe on 2022/12/9.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_ssao.h"
#include <random>
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
#include "shaderMgr.h"


#ifdef BUILD_IOS
#import "ViewController.h"
#endif

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

void test_ssao_0(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cube=new cubeTex("./res/floor.png");
    cube->init();
    cube->setPosition(glm::vec3(0,0,0));
    cube->setRotation(glm::vec3(26.35,-30.14,0));
    cube->setScale(15);
    world::getInstance()->addChild(cube);
    world::getInstance()->getControl()->bindNode(cube);
    
    model* modelObj1=new model("./res/model/backpack/backpack.obj");
    modelObj1->init();
    
    modelObj1->setPosition(glm::vec3(0,0.5,-0.5));
    modelObj1->setRotation(glm::vec3(56.35,-0.14,0));
    modelObj1->setScale(0.5);
    world::getInstance()->addChild(modelObj1);
    world::getInstance()->getControl()->bindNode(modelObj1);
    
    material2* mt=createMaterial(1, 1, 1, 0);
    pointLight* light1=new pointLight(glm::vec3(0.7,0.7,0.7),mt);
    pointLight* light2=new pointLight(glm::vec3(1,0,0),mt);
    pointLight* light3=new pointLight(glm::vec3(0,0,0.5),mt);
    pointLight* light4=new pointLight(glm::vec3(0,0.5,0),mt);
    
    light1->setPosition(glm::vec3(0,0.5,-2.5));
    light2->setPosition(glm::vec3(-4,0.5,-4));
    light3->setPosition(glm::vec3(3,0.5,2));
    light4->setPosition(glm::vec3(-0.8,2.4,4));
    
    float lightScale=0.5;
    light1->setScale(lightScale);
    light2->setScale(lightScale);
    light3->setScale(lightScale);
    light4->setScale(lightScale);
    light1->setVisible(false);
    light2->setVisible(false);
    light3->setVisible(false);
    light4->setVisible(false);
    world::getInstance()->addPointLight(light1);
    world::getInstance()->addPointLight(light2);
    world::getInstance()->addPointLight(light3);
    world::getInstance()->addPointLight(light4);
    world::getInstance()->getControl()->bindNode(light1);
    world::getInstance()->getControl()->bindNode(light2);
    world::getInstance()->getControl()->bindNode(light3);
    world::getInstance()->getControl()->bindNode(light4);
    
    cam->setPosition(glm::vec3(0,0,6.6));
    cam->setYaw(-92.64);
    cam->setPitch(-0.1);
    
    shader* shTest=new shader("./res/shader/3d_model_test.vs","./res/shader/3d_model_test.fs");
    modelObj1->setShader(shTest);
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam,cube,modelObj1](){
        glm::vec3 pos=cam->getPosition();
        flylog("yaw %f pitch %f x %f y %f z %f",cam->getYaw(),cam->getPitch(),pos.x,pos.y,pos.z);
        glm::vec3 s1=cube->getRotation();
        glm::vec3 s2=modelObj1->getRotation();
        flylog("cube rotation %f %f %f",s1.x,s1.y,s1.z);
        flylog("modelObj1 rotation %f %f %f",s2.x,s2.y,s2.z);
    });
}


void test_ssao(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cube=new cubeTex("./res/floor.png");
    cube->init();
    cube->setPosition(glm::vec3(0,0,0));
    cube->setRotation(glm::vec3(26.35,-30.14,0));
    cube->setScale(15);
    world::getInstance()->getControl()->bindNode(cube);
    
    model* modelObj1=new model("./res/model/backpack/backpack.obj");
//    model* modelObj2=new model("./res/model/backpack/backpack.obj");
//    model* modelObj3=new model("./res/model/backpack/backpack.obj");
//    model* modelObj4=new model("./res/model/backpack/backpack.obj");
    
    modelObj1->init();
    modelObj1->setPosition(glm::vec3(0,0.5,-0.5));
    
//    modelObj1->setPosition(glm::vec3(1,0.2,-0.1));
//    modelObj2->setPosition(glm::vec3(-1,0,0));
//    modelObj3->setPosition(glm::vec3(1,1.5,0));
//    modelObj4->setPosition(glm::vec3(-1,1.5,0));
    
    modelObj1->setScale(0.5);
//    modelObj2->setScale(0.5);
//    modelObj3->setScale(0.5);
//    modelObj4->setScale(0.5);
    
    modelObj1->setRotation(glm::vec3(56.35,-0.14,0));
//    modelObj2->setRotation(glm::vec3(30,30,0));
//    modelObj3->setRotation(glm::vec3(30,30,0));
//    modelObj4->setRotation(glm::vec3(30,30,0));
    
    world::getInstance()->getControl()->bindNode(modelObj1);
//    world::getInstance()->getControl()->bindNode(modelObj2);
//    world::getInstance()->getControl()->bindNode(modelObj3);
//    world::getInstance()->getControl()->bindNode(modelObj4);
    
    fboSSAOStruct st=fbo::createFBOSSAO();
    
    //当点光源的三个距离参数都不为0时，会计算光球的半径
    //shader中只会点亮处于此半径内的顶点
    material2* mt=createMaterial(1, 1, 1, 0);
    pointLight* light1=new pointLight(glm::vec3(0.5,0.5,0.5),mt);
    pointLight* light2=new pointLight(glm::vec3(0.1,0,0),mt);
    pointLight* light3=new pointLight(glm::vec3(0,0,0.1),mt);
    pointLight* light4=new pointLight(glm::vec3(0,0.1,0),mt);
    
    light1->setPosition(glm::vec3(0,0.5,-2.5));
    light2->setPosition(glm::vec3(-4,0.5,-4));
    light3->setPosition(glm::vec3(3,0.5,2));
    light4->setPosition(glm::vec3(-0.8,2.4,4));
    
    float lightScale=0.5;
    light1->setScale(lightScale);
    light2->setScale(lightScale);
    light3->setScale(lightScale);
    light4->setScale(lightScale);
    light1->setVisible(false);
    light2->setVisible(false);
    light3->setVisible(false);
    light4->setVisible(false);
    world::getInstance()->addPointLight(light1);
    world::getInstance()->addPointLight(light2);
    world::getInstance()->addPointLight(light3);
    world::getInstance()->addPointLight(light4);
    world::getInstance()->getControl()->bindNode(light1);
    world::getInstance()->getControl()->bindNode(light2);
    world::getInstance()->getControl()->bindNode(light3);
    world::getInstance()->getControl()->bindNode(light4);
    
    cam->setPosition(glm::vec3(0,0,6.6));
    cam->setYaw(-92.64);
    cam->setPitch(-0.1);
    
    quad* quad2D=new quad(st.texPos,g_winWidth,g_winHigh);
    quad2D->flipY(true);
    
    std::uniform_real_distribution<float> randomFloats(0.0,1.0);
    std::default_random_engine gen;
    std::vector<glm::vec3> ssaoKernel;
    for(int i=0;i<64;i++){
        float x=randomFloats(gen)*2.0-1.0;
        float y=randomFloats(gen)*2.0-1.0;
        float z=randomFloats(gen);
        glm::vec3 samplePos=glm::vec3(x,y,z);
        samplePos=glm::normalize(samplePos);
        samplePos*=randomFloats(gen);
        float s = (float)i / 64.0;
        float scale=mathUtil::lerp(0.1, 1, s*s);
        samplePos*=scale;
//        flylog("kernel %f %f %f",samplePos.x,samplePos.y,samplePos.z);
        ssaoKernel.push_back(samplePos);
    }
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    
    shader* shGBufferModel=new shader("./res/shader/3d_model.vs","./res/shader/3d_model_ssao.fs");
    shader* shGBufferTex=new shader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex_ssao.fs");
//     shader* shDS=new shader("./res/shader/3d_model.vs","./res/shader/3d_model_deferred_shading.fs");
//    shader* shLightDeferredShading=new shader("./res/shader/3d_color.vs","./res/shader/3d_color_ssao.fs");
    shader* shQuadSSAO=new shader("./res/shader/2d_quad.vs","./res/shader/2d_quad_ssao.fs");
    shader* shQuadSSAOBlur=new shader("./res/shader/2d_quad.vs","./res/shader/2d_quad_ssao_blur.fs");
    shader* shQuadLight=new shader("./res/shader/2d_quad.vs","./res/shader/2d_quad_light_after_ssao.fs");
  
    cube->setShader(shGBufferTex);
    shGBufferTex->use();
    shGBufferTex->setBool("bReverseNormal", 1);
    
    quad2D->setShader(shQuadSSAO);
    shQuadSSAO->use();
    shQuadSSAO->setInt("texPosition",0);
    shQuadSSAO->setInt("texNormal",1);
    shQuadSSAO->setInt("texAlbedoSpec",2);
    shQuadSSAO->setInt("texNoise",3);
    shQuadSSAO->setMat4("mat_proj", (float*)glm::value_ptr(cam->getPerspectiveMatrix()));
    shQuadSSAO->setMat4("mat_view", (float*)glm::value_ptr(cam->getLookAtMatrix()));
    
    for(int i=0;i<64;i++){
        char szBuf[32]={0};
        sprintf(szBuf,"kernelArr[%d]",i);
        shQuadSSAO->setVec3(szBuf,(float*)&ssaoKernel[i],true);
    }
    
    shQuadLight->use();
    shQuadLight->setInt("texPosition",0);
    shQuadLight->setInt("texNormal",1);
    shQuadLight->setInt("texAlbedoSpec",2);
    shQuadLight->setInt("texOcclusion",3);
    shQuadLight->setBool("bEnableOcclusion", 1);
    shQuadLight->setBool("bZeroOcclusion", 0);
    
    modelObj1->setShader(shGBufferModel);
//    modelObj2->setShader(shFirstPass);
//    modelObj3->setShader(shFirstPass);
//    modelObj4->setShader(shFirstPass);
    
//    light1->setShader(shLightDeferredShading);
//    light2->setShader(shLightDeferredShading);
//    light3->setShader(shLightDeferredShading);
//    light4->setShader(shLightDeferredShading);
    
    world::getInstance()->getControl()->regOnKeyPress('1', [modelObj1](){
        glm::vec3 pos=modelObj1->getPosition();
        flylog("modelObj1 inc z %f->%f",pos.z,pos.z+0.1);
        modelObj1->setPositionZ(pos.z+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('2', [modelObj1](){
        glm::vec3 pos=modelObj1->getPosition();
        flylog("modelObj1 desc z %f->%f",pos.z,pos.z-0.1);
        modelObj1->setPositionZ(pos.z-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('u', [shQuadLight](){
        shQuadLight->use();
        bool bEnableOcclusion=shQuadLight->getBool("bEnableOcclusion");
        shQuadLight->setBool("bEnableOcclusion",!bEnableOcclusion);
        flylog("bEnableOcclusion set to %d",!bEnableOcclusion);
    });
    world::getInstance()->getControl()->regOnKeyPress('y', [shQuadLight](){
        shQuadLight->use();
        bool bZeroOcclusion=shQuadLight->getBool("bZeroOcclusion");
        shQuadLight->setBool("bZeroOcclusion",!bZeroOcclusion);
        flylog("bZeroOcclusion set to %d",!bZeroOcclusion);
    });
    
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fboGBuffer);
    });
    world::getInstance()->setCBBeforeDrawCall([st,cam,view,shGBufferModel,shQuadSSAO,shQuadSSAOBlur,shQuadLight,quad2D,modelObj1,cube,light1,light2,light3,light4](){
        //第一步：场景渲染到st.fbo的三张纹理中(片元坐标，法向量，贴图颜色)
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube->draw();
        modelObj1->setShader(shGBufferModel);
        modelObj1->draw();
        
        
//        modelObj2->draw();
//        modelObj3->draw();
//        modelObj4->draw();
        
//        light1->draw();
//        light2->draw();
//        light3->draw();
//        light4->draw();
        
        
        //第二个pass，计算出环境光遮蔽值
        glBindFramebuffer(GL_FRAMEBUFFER,st.fboSSAO);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad2D->setShader(shQuadSSAO);
        quad2D->setTexID(st.texPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, st.texPos);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, st.texNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, st.texAlbedoSpec);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, st.texNoise);
        quad2D->draw();
        
        //第三个pass，对环境光遮蔽进行模糊
        glBindFramebuffer(GL_FRAMEBUFFER,st.fboSSAOBlur);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad2D->setShader(shQuadSSAOBlur);
        quad2D->setTexID(st.texSSAOColor);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, st.texSSAOColor);
        quad2D->draw();

        //第四个pass，带入模糊后的遮蔽值进行光照计算
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad2D->setShader(shQuadLight);
        quad2D->setTexID(st.texPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, st.texPos);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, st.texNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, st.texAlbedoSpec);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, st.texSSAOColorBlur);
        quad2D->draw();
    });
}
