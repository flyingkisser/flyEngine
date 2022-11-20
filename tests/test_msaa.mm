//
//  test_msaa.cpp
//  flyEngine
//
//  Created by joe on 03/10/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_msaa.h"
#include "texture2.h"
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"

#ifdef BUILD_MAC
#include "keyboardEventMgr.h"
#include "mouseEventMgr.h"
#endif

#include "camera.h"
#include "control.h"
#include "world.h"
#include "material2.h"
#include "cubeTex.h"
#include "cubeColor.h"
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
#include "glslUtil.h"
#include "fbo.h"
#include "sprite.h"
#ifdef BUILD_IOS
#include "ViewController.h"
#endif

USE_NS_FLYENGINE

static void init_light_direction(){
    //设置环境光
    directionLight* dirLight=new directionLight(glm::vec3(1.0f,1.0f,1.0f));
    world::getInstance()->setDirectiontLight(dirLight);
    //因为只有环境光，所以设置的比较亮
    //dirLight->setMaterial(createMaterial(1.0,1.0,1.0,0.2));
}

void test_msaa_1(){
    init_light_direction();
    cubeColor* cubeObj=new cubeColor(glm::vec4(0.8,0.2,0,1));
    if(!cubeObj->init()){
       flylog("cubeObj init failed!");
       return;
    }
    cubeObj->setPosition(glm::vec3(0,0,-2));
//    cubeObj->setMaterial(createMaterial(1.0,1.0,1.0,0.2));
    world::getInstance()->addChild(cubeObj);
    
    world::getInstance()->setCBAfterAnyGLCall([](){
#ifdef BUILD_MAC
        glEnable(GL_MULTISAMPLE);
#endif
    });

    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);

    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* _node){
       _node->rotateBy(glm::vec3(0.5f,0,0));
    },cubeObj);
}


void test_msaa_framebuffer_no_msaa(){
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

void test_msaa_framebuffer_normal(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    fboStruct stMSAA=fbo::createFBOMSAA();
    
    cam->setPosition(glm::vec3(0,0,3));
    world::getInstance()->setCBBeforeAnyGLCall([stMSAA](){
        glBindFramebuffer(GL_FRAMEBUFFER,stMSAA.fbo);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    
    world::getInstance()->setCBBeforeDrawCall([stMSAA,cam,cubeObj,view](){
        cubeObj->draw();
#ifdef BUILD_MAC
        glBindFramebuffer(GL_READ_FRAMEBUFFER,stMSAA.fbo);  //以msaa framebuffer为绘制输入
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);//绘制输出到默认framebuffer
        glBlitFramebuffer(0,0,g_winWidth,g_winHigh,0,0,g_winWidth,g_winHigh,GL_COLOR_BUFFER_BIT,GL_NEAREST);
#elif BUILD_IOS
        glBindFramebuffer(GL_READ_FRAMEBUFFER,stMSAA.fbo);
        [view bindDrawable];    //绑定GLKView创建的framebuffer
        glBlitFramebuffer(0,0,g_winWidth,g_winHigh,0,0,g_winWidth,g_winHigh,GL_COLOR_BUFFER_BIT,GL_NEAREST);
#endif
    });
}


void test_msaa_framebuffer_texture(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    fboStruct st=fbo::createFBO();
    fboStruct stMSAA=fbo::createFBOMSAA();
    sprite* sp=new sprite(st.texID);
    sp->flipY(true);
    
    cam->setPosition(glm::vec3(0,0,3));
    world::getInstance()->setCBBeforeAnyGLCall([stMSAA](){
        glBindFramebuffer(GL_FRAMEBUFFER,stMSAA.fbo);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    
    world::getInstance()->setCBBeforeDrawCall([st,stMSAA,cam,sp,cubeObj,view](){
        cubeObj->draw();
#ifdef BUILD_MAC
        glBindFramebuffer(GL_READ_FRAMEBUFFER,stMSAA.fbo);  //以msaa framebuffer为绘制输入
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,st.fbo);//绘制输出到普通的framebuffer绑定的纹理中，最后渲染这张纹理即可
        glBlitFramebuffer(0,0,g_winWidth,g_winHigh,0,0,g_winWidth,g_winHigh,GL_COLOR_BUFFER_BIT,GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
        
#elif BUILD_IOS
        glBindFramebuffer(GL_READ_FRAMEBUFFER,stMSAA.fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,st.fbo);
        glBlitFramebuffer(0,0,g_winWidth,g_winHigh,0,0,g_winWidth,g_winHigh,GL_COLOR_BUFFER_BIT,GL_NEAREST);
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif

        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sp->draw(); //渲染普通framebuffer绑定的纹理，可通过sp纹理对象的shader进行任何的post-processing
    });
    
    timerUtil::getInstance()->execOnceDelay(3, [](sprite *sp){
        sp->setGray(true);
    }, sp);
    timerUtil::getInstance()->execOnceDelay(6, [](sprite *sp){
        sp->setReveseColor(true);
    }, sp);
}
