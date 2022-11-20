//
//  test_framebuffer.cpp
//  flyEngine
//
//  Created by joe on 15/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_framebuffer.h"
#include "world.h"
#include "window.h"
#include "cubeTex.h"
#include "logUtil.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
#include "timerUtil.h"
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

void test_framebuffer(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    fboStruct st=fbo::createFBO();
    sprite* sp=new sprite(st.texID);
    sp->flipY(true);
    
    cam->setPosition(glm::vec3(0,0,3));
    
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    
    world::getInstance()->setCBBeforeDrawCall([st,cam,sp,cubeObj,view](){
        cubeObj->draw();
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif

        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sp->draw();
    });
    
    timerUtil::getInstance()->execOnceDelay(3, [](sprite *sp){
        sp->setGray(true);
    }, sp);
    timerUtil::getInstance()->execOnceDelay(6, [](sprite *sp){
        sp->setReveseColor(true);
    }, sp);
}


void test_framebuffer_mirror(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    fboStruct st=fbo::createFBO();
    sprite* sp=new sprite(st.texID);
    
    fboStruct st2=fbo::createFBO();
    sprite* sp2=new sprite(st2.texID);

    sp->flipY(true);
    sp->setScale(0.8);
    
    cam->setPosition(glm::vec3(0,0,3));
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    world::getInstance()->setCBAfterAnyGLCall([st,cam,sp,cubeObj,view](){
        cubeObj->draw();
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
        cubeObj->draw();
        glDisable(GL_DEPTH_TEST);
        sp->draw();
    });
}

void test_framebuffer_kernel(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    fboStruct st=fbo::createFBO();
    sprite* sp=new sprite(st.texID);

    sp->flipY(true);
    sp->setShader(shaderMgr::getShader("./res/shader/2d_1tex.vs","./res/shader/2d_1tex_kernel.fs"));
    
    cam->setPosition(glm::vec3(0,0,3));
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    world::getInstance()->setCBAfterAnyGLCall([st,cam,sp,cubeObj,view](){
        cubeObj->draw();
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        sp->draw();
    });
}

