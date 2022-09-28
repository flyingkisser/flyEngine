//
//  test_gs.cpp
//  flyEngine
//
//  Created by joe on 12/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_gs.h"
#include "shader.h"
#include "world.h"
#include "control.h"
#include "state.h"
#include "model.h"
#include "timeUtil.h"
#include "timerUtil.h"
#include "sprite.h"
#include "rotateBy.h"
#include "moveBy.h"
#include "forever.h"
#include "window.h"

USE_NS_FLYENGINE

void test_gs_1(){
//    shader* sh=new shader("./res/shader/gs1.vs","./res/shader/gs1.fs","./res/shader/gs1.gs");
    shader* sh=new shader("./res/shader/gs1.vs","./res/shader/gs1.fs");
    sh->bindGeometry("./res/shader/gs1.gs");
    unsigned int vao,vbo;
    float posArr[]={
        -0.5,0.5,
        0.5,0.5,
        0.5,-0.5,
        -0.5,-0.5
    };
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(posArr),&posArr,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    world::getInstance()->setCBBeforeRender([sh,vao](){
        sh->use();
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS,0,4);
        state::log(1);
    });
}

void test_gs_2(){
    shader* sh=new shader("./res/shader/gs1.vs","./res/shader/gs1.fs","./res/shader/gs2.gs");
    unsigned int vao,vbo;
    float posArr[]={
        -0.5,0.5,
        0.5,0.5,
        0.5,-0.5,
        -0.5,-0.5
    };
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(posArr),&posArr,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    world::getInstance()->setCBBeforeRender([sh,vao](){
        sh->use();
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS,0,4);
        state::log(1);
    });
}

void test_gs_model_explode(){
    model* modelObj=new model("./res/model/backpack/backpack.obj");
    if(!modelObj->init())
        return;
    //如果采用bindGeometry()则不起作用
    shader *sh=new shader("./res/shader/3d_model.vs","./res/shader/3d_1tex_phong.fs","./res/shader/3d_model_explode.gs");
    modelObj->setShader(sh);
    
    world::getInstance()->addChild(modelObj);
    world::getInstance()->getControl()->bindNode(modelObj);
    world::getInstance()->setCBBeforeRender([sh](){
        float t=timeUtil::getTimeFloatSinceRun();
        sh->use();
        sh->setFloat("time",t,true);
    });
}

void test_gs_model_show_normals(){
    model* modelObj=new model("./res/model/backpack/backpack.obj");
    if(!modelObj->init())
        return;
    //如果采用bindGeometry()则不起作用
    shader* newShader=new shader("./res/shader/3d_model_normal.vs","./res/shader/3d_model_normal.fs","./res/shader/3d_model_normal.gs");
    shader* oldShader=modelObj->getShader();
    world::getInstance()->getControl()->bindNode(modelObj);
    world::getInstance()->setCBBeforeRender([newShader,oldShader,modelObj](){
        modelObj->setShader(oldShader);
        modelObj->draw();
        modelObj->setShader(newShader);
        modelObj->draw();
    });
}

void test_gs_model_gen2(){
    model* modelObj=new model("./res/model/backpack/backpack.obj");
    if(!modelObj->init())
        return;
    //如果采用bindGeometry()则不起作用
    shader *sh=new shader("./res/shader/3d_model.vs","./res/shader/3d_1tex_phong.fs","./res/shader/3d_model_gen2.gs");
    modelObj->setShader(sh);
    sh->use();
    sh->setInt("gen_max",10,true);
    
    world::getInstance()->addChild(modelObj);
    world::getInstance()->getControl()->bindNode(modelObj);
    
//    timerUtil::getInstance()->exec(0.0167,[](model* m){
//        m->rotateBy(glm::vec3(0.1,0.1,0.0));
//    },modelObj);
}

void test_sprite_gs(){
    sprite* sp=new sprite("res/wood.png");
    shader* sh=new shader("./res/shader/2d_1tex.vs", "./res/shader/2d_1tex.fs","./res/shader/2d_1tex.gs");
    sp->setShader(sh);
    world::getInstance()->addChild(sp);
    timerUtil::getInstance()->execOnceDelay(1.5, [](sprite* sp){
        flyEngine::size s=sp->getContentSize();
        sp->setPosition(glm::vec3((g_winWidth-s.width)/2,(g_winHigh-s.height)/2,0));
        rotateBy* r=new rotateBy(1,glm::vec3(0,0,60));
        moveBy* m1=new moveBy(1,glm::vec3(100,50,0));
        moveBy* m2=new moveBy(1,glm::vec3(-100,-50,0));
        sp->runAction(new forever(3,r,m1,m2));
    }, sp);
}

void test_billboard_gs(){
    sprite* sp=new sprite("res/wood.png");
    shader* sh=new shader("./res/shader/2d_1tex.vs", "./res/shader/2d_1tex.fs","./res/shader/2d_1tex.gs");
    sp->setShader(sh);
    world::getInstance()->addChild(sp);
    timerUtil::getInstance()->execOnceDelay(1.5, [](sprite* sp){
        flyEngine::size s=sp->getContentSize();
        sp->setPosition(glm::vec3((g_winWidth-s.width)/2,(g_winHigh-s.height)/2,0));
        rotateBy* r=new rotateBy(1,glm::vec3(0,0,60));
        moveBy* m1=new moveBy(1,glm::vec3(100,50,0));
        moveBy* m2=new moveBy(1,glm::vec3(-100,-50,0));
        sp->runAction(new forever(3,r,m1,m2));
    }, sp);
}
