//
//  test_depth_stencil_blend.cpp
//  flyEngine
//
//  Created by joe on 27/06/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_depth_stencil_blend_facecull.h"
#include "world.h"
#include "cubeTex.h"
#include "logUtil.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
#include "timerUtil.h"
#include "control.h"
using namespace flyEngine;
void test_depths(){
    cubeTex* cubeObj=new cubeTex("res/metal.png");
    if(!cubeObj->init()){
        flylog("init cubeObj failed!");
        return;
    }
//    cubeObj->resetPos();
//    cubeObj->rotateBy(glm::vec3(45,0,0));
//    cubeObj->setScale(glm::vec3(0.5,0.5,0.3));
//    cubeObj->setPosition(glm::vec3(0,-0.3,0.5));
    
    cubeObj->resetPos();
    cubeObj->setPosition(glm::vec3(0,-0.3,0.5));
    cubeObj->rotateBy(glm::vec3(60,0,0));
    cubeObj->setScale(0.3);
    
    cubeTex* cubeObj2=new cubeTex("res/metal.png");
    if(!cubeObj2->init()){
        flylog("init cubeObj2 failed!");
        return;
    }
//    cubeObj2->resetPos();
//    cubeObj2->rotateBy(glm::vec3(60,0,0));
//    cubeObj2->setScale(glm::vec3(0.3,0.3,0.3));
//    cubeObj2->setPosition(glm::vec3(0.5,-0.2,0.5));
    cubeObj2->resetPos();
    cubeObj2->setPosition(glm::vec3(0.5,-0.2,0.5));
    cubeObj2->rotateBy(glm::vec3(60,0,0));
    cubeObj2->setScale(0.3);
    
    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane))){
        flylog("init plainObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));

    world::getInstance()->addChild(cubeObj);
    world::getInstance()->addChild(cubeObj2);
    world::getInstance()->addChild(plainObj);
        
    world::getInstance()->setCBBeforeDrawCall([](){
       // glDepthFunc(GL_ALWAYS);//深度测试无条件通过，这时最后绘制的，会覆盖前面绘制的，GL_LESS是默认值
        glDepthFunc(GL_LESS);
    });
    
}

void test_depths_2(){
    shader* depthShader=new shader("res/shader/3d_model.vs","res/shader/3d_depth_test1.fs");
    cubeTex* cubeObj=new cubeTex("res/metal.png");
    if(!cubeObj->init()){
        flylog("init cubeObj failed!");
        return;
    }
//    cubeObj->resetPos();
//    cubeObj->rotateBy(glm::vec3(45,0,0));
//    cubeObj->setScale(glm::vec3(0.5,0.5,0.3));
//    cubeObj->setPosition(glm::vec3(0,-0.3,0.5));

    cubeObj->resetPos();
    cubeObj->setPosition(glm::vec3(0,-0.3,0.5));
    cubeObj->rotateBy(glm::vec3(60,0,0));
    cubeObj->setScale(0.3);
    
    cubeTex* cubeObj2=new cubeTex("res/metal.png");
    if(!cubeObj2->init()){
        flylog("init cubeObj2 failed!");
        return;
    }
    cubeObj2->resetPos();
    cubeObj2->rotateBy(glm::vec3(60,0,0));
    cubeObj2->setScale(0.3);
    cubeObj2->setPosition(glm::vec3(0.5,-0.2,0.5));
        

    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane))){
        flylog("init plainObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));

    cubeObj->setShader(depthShader);
    cubeObj2->setShader(depthShader);
    plainObj->setShader(depthShader);

    world::getInstance()->addChild(cubeObj);
    world::getInstance()->addChild(cubeObj2);
    world::getInstance()->addChild(plainObj);

    world::getInstance()->setCBBeforeDrawCall([](){
        glDepthFunc(GL_LESS);//GL_LESS是默认值
    });
}

void test_stencil(){
    cubeTex* cubeObj=new cubeTex("res/metal.png");
    if(!cubeObj->init()){
        flylog("init cubeTex failed!");
        return;
    }
    cubeObj->resetPos();
    cubeObj->setPosition(glm::vec3(0,-0.3,0.5));
    cubeObj->rotateBy(glm::vec3(60,0,0));
    cubeObj->setScale(0.3);
    
    cubeTex* cubeObj2=new cubeTex("res/metal.png");
    if(!cubeObj2->init()){
        flylog("init cubeTex failed!");
        return;
    }
    cubeObj2->resetPos();
    cubeObj2->setPosition(glm::vec3(0.5,-0.2,0.5));
    cubeObj2->rotateBy(glm::vec3(60,0,0));
    cubeObj2->setScale(0.3);
    
    
    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane))){
        flylog("init marbleObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));
    
    shader* defShader=plainObj->getShader();
    shader* borderShader=new shader("res/shader/3d_1tex.vs","res/shader/color_border.fs");
    camera* cam=world::getInstance()->getCamera();
    
    world::getInstance()->setCBBeforeDrawCall([cubeObj,cubeObj2,plainObj,cam,defShader,borderShader](){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        glStencilFunc(GL_ALWAYS,1,0xff);
        glStencilMask(0xff);
        
        //stencil缓存不更新
        glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
        plainObj->draw(cam);
        
        //通过stencil测试的片元，stencil缓存写入1
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
        cubeObj->setScale(0.3);
        cubeObj2->setScale(0.3);
        cubeObj->setShader(defShader);
        cubeObj2->setShader(defShader);
        cubeObj->draw(cam);
        cubeObj2->draw(cam);
        
    });
    
    
    world::getInstance()->setCBAfterDrawCall([cubeObj,cubeObj2,cam,borderShader](){
        cubeObj->setScale(0.31);
        cubeObj2->setScale(0.31);
        glDisable(GL_DEPTH_TEST);
        glStencilFunc(GL_NOTEQUAL,1,0xff);
        cubeObj->setShader(borderShader);
        cubeObj2->setShader(borderShader);
        cubeObj->draw(cam);
        cubeObj2->draw(cam);
    });

}

//对于加载的对象，绘制出描边效果
void test_stencil_2(){
    model* modelObj=new model("res/model/backpack/backpack.obj");
    if(!modelObj->init()){
        flylog("init model failed!");
        return;
    }
    modelObj->setPosition(glm::vec3(0,0,-0.5));
//    modelObj->rotateBy(glm::vec3(60,0,0));
//    modelObj->setScale(0.3);
    
    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane))){
        flylog("init marbleObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));
    
    shader* defShader=modelObj->getShader();
    shader* borderShader=new shader("res/shader/3d_1tex.vs","res/shader/color_border.fs");
    flyEngine::camera* cam=world::getInstance()->getCamera();

    world::getInstance()->setCBBeforeDrawCall([modelObj,plainObj,cam,defShader,borderShader](){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        glStencilFunc(GL_ALWAYS,1,0xff);
        glStencilMask(0xff);
        
        //stencil缓存不更新
        glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
        plainObj->draw(cam);
        
        //通过stencil测试的片元，stencil缓存写入1
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
        modelObj->setScale(0.3);
        modelObj->setShader(defShader);
        modelObj->draw(cam);
    });
    
    
    world::getInstance()->setCBAfterDrawCall([modelObj,cam,borderShader](){
        modelObj->setScale(0.31);
        glDisable(GL_DEPTH_TEST);
        glStencilFunc(GL_NOTEQUAL,1,0xff);
        modelObj->setShader(borderShader);
        modelObj->draw(cam);
    });
}

void test_blend_1(){
    float transparentVertices_flip[] = {
           // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
           0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
           0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
           1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

           0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
           1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
           1.0f,  0.5f,  0.0f,  1.0f,  0.0f
       };
    float transparentVertices[] = {
           // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
           0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
           0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
           1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

           0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
           1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
           1.0f,  0.5f,  0.0f,  1.0f,  1.0f
       };

    cubeTex* cubeObj=new cubeTex("res/grass.png");
    if(!cubeObj->initByVerticeArr(transparentVertices,sizeof(transparentVertices))){
        flylog("init cubeObj failed!");
        return;
    }
    shader* shaderObj=new shader("res/shader/3d_1tex.vs","res/shader/3d_blend_1.fs");
    cubeObj->setShader(shaderObj);
    std::vector<glm::vec3> posVector={
        glm::vec3(-1.5f,  0.0f, -0.48f),
        glm::vec3(1.5f,  0.0f,  0.51f),
        glm::vec3(0.0f,  0.0f,  0.7f),
        glm::vec3(-0.3f,  0.0f, -2.3f),
        glm::vec3(0.5f,  0.0f, -0.6f)
    };
    for(auto it:posVector){
        cubeTex* cube=cubeObj->clone();
        cube->setPosition(it);
        world::getInstance()->addChild(cube);
    }
    
    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane))){
        flylog("init cubeTex failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));
    world::getInstance()->addChild(plainObj);
    
}


void test_blend_2(){
    cubeTex* cubeObj=new cubeTex("res/metal.png");
    if(!cubeObj->init()){
        flylog("init cubeTex failed!");
        return;
    }
    cubeObj->resetPos();
    cubeObj->setPosition(glm::vec3(0,-0.3,0.5));
    cubeObj->rotateBy(glm::vec3(60,0,0));
    cubeObj->setScale(0.3);
    
    cubeTex* cubeObj2=new cubeTex("res/metal.png");
    if(!cubeObj2->init()){
        flylog("init cubeTex failed!");
        return;
    }
    cubeObj2->resetPos();
    cubeObj2->setPosition(glm::vec3(0.5,-0.2,0.5));
    cubeObj2->rotateBy(glm::vec3(60,0,0));
    cubeObj2->setScale(0.3);
    
    
    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane))){
        flylog("init marbleObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));
    
    world::getInstance()->addChild(cubeObj);
    world::getInstance()->addChild(cubeObj2);
    world::getInstance()->addChild(plainObj);
    
    float windowArr[]={
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    
    cubeTex* windowObj=new cubeTex("res/window.png");
    if(!windowObj->initByVerticeArr(windowArr,sizeof(windowArr))){
        flylog("init windowObj failed!");
        return;
    }
    std::map<float,glm::vec3> mapPos;
    std::vector<glm::vec3> vectorPos={
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3( 1.5f, 0.0f, 0.51f),
        glm::vec3( 0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, -0.3f, -2.3f),
        glm::vec3( 0.5f, -0.2f, -0.6f)
    };
    camera* cam=world::getInstance()->getCamera();
    shader* windowShader=new shader("res/shader/3d_1tex.vs","res/shader/3d_blend_2.fs");
 
    glm::vec3 cameroPos=cam->getPosition();
    std::vector<cubeTex*> vectorWindow;
    for(auto pos:vectorPos){
        mapPos[glm::length(cameroPos-pos)]=pos;
        cubeTex* windowCube=windowObj->clone();
        windowCube->setShader(windowShader);
        world::getInstance()->addChild(windowCube);
        windowCube->setPosition(pos);
        windowCube->setScale(0.6);
        windowCube->rotateBy(glm::vec3(0,30,0));
        vectorWindow.push_back(windowCube);
    }
    //同时启用深度测试和GL_BLEND以后，后绘制的物体会找先绘制的颜色
    //如果先添加了近平面的物体，再添加远平面的东西
    //近平面添加以后，因为这时背影没有东西，所以只显示前面物体的颜色
    //绘制后面的物品时，因为深度测试的原因，部分处于遮挡的片元会被抛弃掉
    //所以整体的效果就是放在前面有透明的物体如玻璃，显示不出来后面的物体
    //所以需要按先后顺序进行绘制（即DrawCall)
    //先绘制后面的，再绘制前面的带透明效果的
    world::getInstance()->setCBBeforeDrawCall([vectorPos,vectorWindow](){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        
        glClear(GL_DEPTH_BUFFER_BIT);
        
        std::map<float,glm::vec3> mapPos;
        camera* cam=world::getInstance()->getCamera();
        glm::vec3 camPos=cam->getPosition();
        for(auto pos:vectorPos){
            mapPos[glm::length(camPos-pos)]=pos;
        }
        int i=0;
        for(auto it:mapPos){
            vectorWindow[i++]->setPosition(it.second);
        }
        
    });

}



//对于加载的对象，绘制出描边效果
void test_facecull(){
    model* modelObj=new model("res/model/backpack/backpack.obj");
    if(!modelObj->init()){
        flylog("init model failed!");
        return;
    }
    modelObj->setPosition(glm::vec3(0,0,-0.5));
    modelObj->setScale(0.4);
    
    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane))){
        flylog("init marbleObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));
    world::getInstance()->getControl()->bindNode(modelObj);
    camera* cam=world::getInstance()->getCamera();
    world::getInstance()->addChild(modelObj);
    world::getInstance()->addChild(plainObj);
    world::getInstance()->setCBBeforeDrawCall([cam,plainObj,modelObj](){
//        glDisable(GL_CULL_FACE);
//        glEnable(GL_DEPTH_TEST);
//        glClear(GL_DEPTH_BUFFER_BIT);
//        plainObj->draw(cam);
//
//        glEnable(GL_CULL_FACE);
//        glCullFace(GL_BACK);
//        modelObj->draw(cam);
    });
    
    timerUtil::getInstance()->exec(0.1,[](camera* cam,model* modelObj,cubeTex* plainObj){
        float x=2*sin(glfwGetTime());
        float z=8*cos(glfwGetTime());
        modelObj->setPositionX(x);
        modelObj->setPositionZ(z-8);
        modelObj->rotateBy(glm::vec3(0,0.5,0));
    },cam,modelObj,plainObj);
}
