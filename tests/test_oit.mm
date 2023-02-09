//
//  test_oit.cpp
//  flyEngine
//
//  Created by Joe on 2023/1/8.
//  Copyright © 2023 joe. All rights reserved.
//

#include "test_oit.h"
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


#ifdef BUILD_IOS
#import "ViewController.h"
#endif

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

void test_oit_quad_color(){
    camera* cam=world::getInstance()->getCamera();

    
    quadColor* sp1=new quadColor(glm::vec4(1,0,0,0.5),10,20);
    quadColor* sp2=new quadColor(glm::vec4(0,1,0,0.5),10,20);
    quadColor* sp3=new quadColor(glm::vec4(0,0,1,0.5),10,20);
    
    sp1->setPosition(glm::vec3(0,0,-9));
    sp1->setRotation(glm::vec3(0,30,0));
    
    sp2->setPosition(glm::vec3(-0.5,0,-10));
    sp2->setRotation(glm::vec3(0,30,0));
    
    sp3->setPosition(glm::vec3(0.5,0,-11));
    sp3->setRotation(glm::vec3(0,30,0));
    
    cam->getControl()->bindNode(sp1);
    cam->getControl()->bindNode(sp2);
    cam->getControl()->bindNode(sp3);
    

    fboOitStruct st=fbo::createFBOOit();
    
    quad* quadComposition=new quad(st.texAccum,g_screenWidth,g_screenHigh);
    quad* quadScreen=new quad(st.texOpaque,g_screenWidth,g_screenHigh);
    
    //当点光源的三个距离参数都不为0时，会计算光球的半径
    //shader中只会点亮处于此半径内的顶点
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
    
//    cam->setPosition(glm::vec3(0,0,20));
    
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fboOpaque);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    
    glm::vec4 zeroVec4=glm::vec4(0.0);
    glm::vec4 oneVec4=glm::vec4(1.0);
    shader* shTransparent=new shader("./res/shader/3d_quad.vs","./res/shader/3d_quad_color_transparent.fs");
    shader* shComposition=new shader("./res/shader/3d_quad.vs","./res/shader/3d_quad_composition.fs");
    shader* shDef=sp1->getShader();
    
    quadComposition->setShader(shComposition);
    world::getInstance()->setCBBeforeDrawCall([st,cam,view,shDef,shTransparent,
                                               sp1,sp2,sp3,quadComposition,quadScreen,
                                               light1,light2,light3,light4,zeroVec4,oneVec4](){
        //第一步：场景渲染到st.fboOpaque
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        sp1->setShader(shDef);
        sp2->setShader(shDef);
        sp3->setShader(shDef);
        sp1->draw();
        sp2->draw();
        sp3->draw();
        
       
        //第二步：场景渲染到st.fboTransparent，形成texAccum和texReveal两张纹理
        glBindFramebuffer(GL_FRAMEBUFFER, st.fboTransparent);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        
        glDepthMask(GL_FALSE);  //深度缓存只读
        glEnable(GL_BLEND);
        glBlendFunci(0,GL_ONE,GL_ONE);  //accumulation blend target
        glBlendFunci(1,GL_ZERO,GL_ONE_MINUS_SRC_COLOR);//revealge blend target
        glBlendEquation(GL_FUNC_ADD);

        glClearBufferfv(GL_COLOR,0,&zeroVec4[0]);
        glClearBufferfv(GL_COLOR,1,&oneVec4[0]);
        
        sp1->setShader(shTransparent);
        sp2->setShader(shTransparent);
        sp3->setShader(shTransparent);
        
        sp1->draw();
        sp2->draw();
        sp3->draw();
        
       //第三步：场景渲染到st.fboOpaque的texOpaque，把上面的texAccum和texReveal两张纹理，合成一张纹理
        glBindFramebuffer(GL_FRAMEBUFFER, st.fboOpaque);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        shader* shComp=quadComposition->getShader();
        shComp->use();
        shComp->setInt("accum", 0);
        shComp->setInt("reveal", 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,st.texAccum);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,st.texReveal);
        quadComposition->draw();

        //最后一步：显示最后合成的这张纹理
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glDisable(GL_DEPTH);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        quadScreen->getShader()->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,st.texOpaque);
        quadScreen->draw();
        
//        light1->draw();
//        light2->draw();
//        light3->draw();
//        light4->draw();
        //        glBindFramebuffer(GL_FRAMEBUFFER,0);
        //        glDisable(GL_DEPTH_TEST);
        //        glDepthMask(GL_TRUE);
        //        glDisable(GL_BLEND);
        //        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        //        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //        glActiveTexture(GL_TEXTURE0);
        //        glBindTexture(GL_TEXTURE_2D,st.texReveal);
        //        quadScreen->setTexID(st.texReveal);
        //        quadScreen->draw();
        //        return;
    });
}

void test_oit2(){
    camera* cam=world::getInstance()->getCamera();
    model* modelObj1=new model("./res/model/backpack/backpack.obj");
    model* modelObj2=new model("./res/model/backpack/backpack.obj");
    model* modelObj3=new model("./res/model/backpack/backpack.obj");
    model* modelObj4=new model("./res/model/backpack/backpack.obj");
    
    modelObj1->setPosition(glm::vec3(1,0,0));
    modelObj2->setPosition(glm::vec3(-1,0,0));
    modelObj3->setPosition(glm::vec3(1,1.5,0));
    modelObj4->setPosition(glm::vec3(-1,1.5,0));
    
    modelObj1->setScale(0.5);
    modelObj2->setScale(0.5);
    modelObj3->setScale(0.5);
    modelObj4->setScale(0.5);
    
    modelObj1->setRotation(glm::vec3(30,30,0));
    modelObj2->setRotation(glm::vec3(30,30,0));
    modelObj3->setRotation(glm::vec3(30,30,0));
    modelObj4->setRotation(glm::vec3(30,30,0));
    
    world::getInstance()->getControl()->bindNode(modelObj1);
    world::getInstance()->getControl()->bindNode(modelObj2);
    world::getInstance()->getControl()->bindNode(modelObj3);
    world::getInstance()->getControl()->bindNode(modelObj4);
    
    fboDeferredShadingStruct st=fbo::createFBODeferredShading();
    
    //当点光源的三个距离参数都不为0时，会计算光球的半径
    //shader中只会点亮处于此半径内的顶点
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
    
    cam->setPosition(glm::vec3(0,0,20));
    
    quad* quad2D=new quad(st.texPos,g_winWidth,g_winHigh);
    quad2D->flipY(true);
    
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    
    shader* shDeferredShading=new shader("./res/shader/3d_model.vs","./res/shader/3d_model_deferred_shading.fs");
    shader* shLightDeferredShading=new shader("./res/shader/3d_color.vs","./res/shader/3d_color_deferred_shading.fs");
    shader* shQuadDeferredShading=new shader("./res/shader/3d_quad.vs","./res/shader/2d_quad_deferred_shading.fs");
    quad2D->setShader(shQuadDeferredShading);
    shQuadDeferredShading->use();
    shQuadDeferredShading->setInt("texPosition",0);
    shQuadDeferredShading->setInt("texNormal",1);
    shQuadDeferredShading->setInt("texAlbedoSpec",2);
    //    shQuadDeferredShading->setBool("bEnableLightVolumn", true);
    
    //    glslUtil::printAllUniformAndBlock(shQuadDeferredShading->getProgramID());
    
    modelObj1->setShader(shDeferredShading);
    modelObj2->setShader(shDeferredShading);
    modelObj3->setShader(shDeferredShading);
    modelObj4->setShader(shDeferredShading);
    
    light1->setShader(shLightDeferredShading);
    light2->setShader(shLightDeferredShading);
    light3->setShader(shLightDeferredShading);
    light4->setShader(shLightDeferredShading);
    
    world::getInstance()->setCBBeforeDrawCall([st,cam,view,shDeferredShading,shQuadDeferredShading,quad2D,modelObj1,modelObj2,modelObj3,modelObj4,light1,light2,light3,light4](){
        //第一步：场景渲染到st.fbo的三张纹理中(片元坐标，法向量，贴图颜色)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        modelObj1->draw();
        modelObj2->draw();
        modelObj3->draw();
        modelObj4->draw();
        
        light1->draw();
        light2->draw();
        light3->draw();
        light4->draw();
        
        //第二步
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad2D->setShader(shQuadDeferredShading);
        quad2D->setTexID(st.texPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, st.texPos);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, st.texNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, st.texAlbedoSpec);
        quad2D->draw();
    });
    
}
