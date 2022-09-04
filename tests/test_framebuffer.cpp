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

USE_NS_FLYENGINE

void test_framebuffer(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
    
    fboStruct st=fbo::createFBO();
    sprite* sp=new sprite(st.texID);
    sp->flipY(true);
    
    cam->setPosition(glm::vec3(0,0,3));
    world::getInstance()->setCBBeforeDrawCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });
    world::getInstance()->setCBAfterDrawCall([st,cam,sp,cubeObj](){
        cubeObj->draw();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    
    fboStruct st=fbo::createFBO();
    sprite* sp=new sprite(st.texID);

    sp->flipY(true);
    sp->setScale(0.8);
    
    cam->setPosition(glm::vec3(0,0,3));
    world::getInstance()->setCBBeforeDrawCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });
    world::getInstance()->setCBAfterDrawCall([st,cam,sp,cubeObj](){
        cubeObj->draw();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    
    fboStruct st=fbo::createFBO();
    sprite* sp=new sprite(st.texID);

    sp->flipY(true);
    sp->setShader(shaderMgr::getShader("./res/shader/2d_1tex.vs","./res/shader/2d_1tex_kernel.fs"));
    
    cam->setPosition(glm::vec3(0,0,3));
    world::getInstance()->setCBBeforeDrawCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });
    world::getInstance()->setCBAfterDrawCall([st,cam,sp,cubeObj](){
        cubeObj->draw();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        sp->draw();
    });
}

