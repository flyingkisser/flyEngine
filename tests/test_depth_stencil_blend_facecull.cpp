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
#include "timeUtil.h"
#include "control.h"
#include "forever.h"
#include "rotateBy.h"

using namespace flyEngine;
void test_depths(){
    cubeTex* cubeObj=new cubeTex("res/metal.png");
    if(!cubeObj->init()){
        flylog("init cubeObj failed!");
        return;
    }
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
    cubeObj2->setPosition(glm::vec3(0.5,-0.2,0.5));
    cubeObj2->rotateBy(glm::vec3(60,0,0));
    cubeObj2->setScale(0.3);
    
    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    int descArr[]={3,2};
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane),descArr,2)){
        flylog("init plainObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));

    world::getInstance()->addChild(cubeObj);
    world::getInstance()->addChild(cubeObj2);
    world::getInstance()->addChild(plainObj);
    
    world::getInstance()->getControl()->bindNode(cubeObj);
    world::getInstance()->getControl()->bindNode(cubeObj2);
    world::getInstance()->getControl()->bindNode(plainObj);
        
    world::getInstance()->setCBBeforeAnyGLCall([](){
       // glDepthFunc(GL_ALWAYS);//深度测试无条件通过，这时最后绘制的，会覆盖前面绘制的，GL_LESS是默认值
        glDepthFunc(GL_LESS);
    });
    
}

void test_depths_2(){
    shader* depthShader=new shader("res/shader/3d_1tex_phong.vs","res/shader/3d_depth_test2.fs");
    cubeTex* cubeObj=new cubeTex("res/metal.png");
    if(!cubeObj->init()){
        flylog("init cubeObj failed!");
        return;
    }
    cubeObj->setPosition(glm::vec3(0,-0.3,0.5));
    cubeObj->rotateBy(glm::vec3(60,0,0));
    cubeObj->setScale(0.3);
    
    cubeTex* cubeObj2=new cubeTex("res/metal.png");
    if(!cubeObj2->init()){
        flylog("init cubeObj2 failed!");
        return;
    }
    cubeObj2->setPosition(glm::vec3(0.5,-0.2,0.5));
    cubeObj2->rotateBy(glm::vec3(60,0,0));
    cubeObj2->setScale(0.3);

    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    int descArr[]={3,2};
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane),descArr,2)){
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
    
    world::getInstance()->getControl()->bindNode(plainObj);

    world::getInstance()->setCBBeforeAnyGLCall([](){
        glDepthFunc(GL_LESS);//GL_LESS是默认值
    });
}

//只绘制两个物体的边
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
    int descArr[]={3,2};
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane),descArr,2)){
        flylog("init marbleObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));
    
    world::getInstance()->getControl()->bindNode(cubeObj);
    world::getInstance()->getControl()->bindNode(cubeObj2);
    world::getInstance()->getControl()->bindNode(plainObj);
  
    shader* defShader=plainObj->getShader();
    shader* borderShader=new shader("res/shader/3d_1tex.vs","res/shader/color_border.fs");
    camera* cam=world::getInstance()->getCamera();
    
    world::getInstance()->setCBBeforeAnyGLCall([cubeObj,cubeObj2,plainObj,cam,defShader,borderShader](){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        //定义stencil测试通过的条件
        //定义了ref值为1，stencil and 0xff以后与1进行比较
        //如果GL_ALWAYS了，则通过测试，即永远通过
        glStencilFunc(GL_ALWAYS,1,0xff);
        
        //测试通过后,ref值与0xff相与
        glStencilMask(0xff);
        
        //stencil缓存不更新
        //第一个GL_KEEP:深度和stencil测试都没有通过，如何修改stencil缓存(不更新)
        //第二个GL_KEEP:深度通过，stencil没有通过，如何修改stencil缓存(不更新)
        //第三个GL_KEEP:深度和stencil测试都通过，如何修改stencil缓存(不更新)
        //指定了GL_KEEP,即不改变缓存的值
        glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
        plainObj->draw();
        
        //第一个GL_KEEP:深度和stencil测试都没有通过，如何修改stencil缓存(不更新)
        //第二个GL_KEEP:深度通过，stencil没有通过，如何修改stencil缓存(不更新)
        //第三个GL_REPLACE:深度和stencil测试都通过，如何修改stencil缓存(ref值&0xff写入)
        //通过stencil和深度测试的片元，即需要绘制的片元，在stencil缓存中写入1
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
        cubeObj->setScale(0.3);
        cubeObj2->setScale(0.3);
        cubeObj->setShader(defShader);
        cubeObj2->setShader(defShader);
        cubeObj->draw();
        cubeObj2->draw();
    });
    //在setCBBeforeAnyGLCall之后，会调用glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //即清除颜色和深度缓存，所以上面这些操作只是记录下来了stencil缓存
    
    //让多出来的片元通过stencil测试，绘制出来描边的效果
    world::getInstance()->setCBAfterAnyGLCall([cubeObj,cubeObj2,cam,borderShader](){
        cubeObj->setScale(0.31);
        cubeObj2->setScale(0.31);
        glDisable(GL_DEPTH_TEST);
      
        //上面对两个cubeObj进行绘制的片元相应缓存位置写入了1
        //这次物体放大了一点，缓存不为1的部分，即是和上面绘制相比多出来的部分，才能通过stencil测试
        
        //定义stencil测试通过的条件
        //定义了ref值为1，stencil and 0xff以后与1进行比较
        //GL_NOTEQUAL，与当前缓存中的值不相等，通过测试，即多出来的边，才能通过测试
        glStencilFunc(GL_NOTEQUAL,1,0xff);
        
        cubeObj->setShader(borderShader);
        cubeObj2->setShader(borderShader);
        cubeObj->draw();
        cubeObj2->draw();
    });
}

//正常绘制，两个物体描边
void test_stencil_2(){
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
    int descArr[]={3,2};
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane),descArr,2)){
        flylog("init marbleObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));
    
    world::getInstance()->getControl()->bindNode(cubeObj);
    world::getInstance()->getControl()->bindNode(cubeObj2);
    world::getInstance()->getControl()->bindNode(plainObj);
  
    shader* defShader=plainObj->getShader();
    shader* borderShader=new shader("res/shader/3d_1tex.vs","res/shader/color_border.fs");
    camera* cam=world::getInstance()->getCamera();
    
    //让多出来的片元通过stencil测试，绘制出来描边的效果
    world::getInstance()->setCBAfterAnyGLCall([plainObj,cubeObj,cubeObj2,cam,defShader,borderShader](){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS,1,0xff);
        glStencilMask(0xff);
        glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
        plainObj->draw();
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
        cubeObj->setScale(0.3);
        cubeObj2->setScale(0.3);
        cubeObj->setShader(defShader);
        cubeObj2->setShader(defShader);
        cubeObj->draw();
        cubeObj2->draw();
        
        cubeObj->setScale(0.31);
        cubeObj2->setScale(0.31);
        glDisable(GL_DEPTH_TEST);
        glStencilFunc(GL_NOTEQUAL,1,0xff);
        cubeObj->setShader(borderShader);
        cubeObj2->setShader(borderShader);
        cubeObj->draw();
        cubeObj2->draw();
    });
}

//正常绘制，三个物体描边
void test_stencil_3(){
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
    int descArr[]={3,2};
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane),descArr,2)){
        flylog("init marbleObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));
    
    world::getInstance()->getControl()->bindNode(cubeObj);
    world::getInstance()->getControl()->bindNode(cubeObj2);
    world::getInstance()->getControl()->bindNode(plainObj);
  
    shader* defShader=plainObj->getShader();
    shader* borderShader=new shader("res/shader/3d_1tex.vs","res/shader/color_border.fs");
    camera* cam=world::getInstance()->getCamera();
    
    //让多出来的片元通过stencil测试，绘制出来描边的效果
    world::getInstance()->setCBAfterAnyGLCall([plainObj,cubeObj,cubeObj2,cam,defShader,borderShader](){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS,1,0xff);
        glStencilMask(0xff);
//        glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
//        plainObj->draw();
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
        cubeObj->setScale(0.3);
        cubeObj2->setScale(0.3);
        plainObj->setScale(1);
        cubeObj->setShader(defShader);
        cubeObj2->setShader(defShader);
        plainObj->setShader(defShader);
        cubeObj->draw();
        cubeObj2->draw();
        plainObj->draw();
        
        cubeObj->setScale(0.31);
        cubeObj2->setScale(0.31);
        plainObj->setScale(1.01);
        glDisable(GL_DEPTH_TEST);
        glStencilFunc(GL_NOTEQUAL,1,0xff);
        cubeObj->setShader(borderShader);
        cubeObj2->setShader(borderShader);
        plainObj->setShader(borderShader);
        cubeObj->draw();
        cubeObj2->draw();
        plainObj->draw();
    });
}


//绘制模型和底面的描边
void test_stencil_4(){
    model* modelObj=new model("res/model/backpack/backpack.obj");
    if(!modelObj->init()){
        flylog("init model failed!");
        return;
    }
    modelObj->setPosition(glm::vec3(0,0,-0.5));
    
    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    int descArr[]={3,2};
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane),descArr,2)){
        flylog("init marbleObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));
    
    world::getInstance()->getControl()->bindNode(modelObj);
    world::getInstance()->getControl()->bindNode(plainObj);
    
    shader* defShader=modelObj->getShader();
    shader* borderShader=new shader("res/shader/3d_1tex.vs","res/shader/color_border.fs");
    flyEngine::camera* cam=world::getInstance()->getCamera();

    world::getInstance()->setCBBeforeAnyGLCall([modelObj,plainObj,cam,defShader,borderShader](){
       
        
        //stencil缓存不更新
        glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
        plainObj->draw();
        
        //通过stencil测试的片元，stencil缓存写入1
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
        modelObj->setScale(0.3);
        modelObj->setShader(defShader);
        modelObj->draw();
    });
    
    world::getInstance()->setCBAfterAnyGLCall([modelObj,plainObj,defShader,borderShader](){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        glStencilFunc(GL_ALWAYS,1,0xff);
        glStencilMask(0xff);
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
        modelObj->setScale(0.3);
        plainObj->setScale(1);
        modelObj->setShader(defShader);
        plainObj->setShader(defShader);
        modelObj->draw();
        plainObj->draw();
        
        glDisable(GL_DEPTH_TEST);
        glStencilFunc(GL_NOTEQUAL,1,0xff);
        modelObj->setScale(0.31);
        plainObj->setScale(1.01);
        modelObj->setShader(borderShader);
        plainObj->setShader(borderShader);
        modelObj->draw();
        plainObj->draw();
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
    int descArr[]={3,2};
    if(!cubeObj->initByVerticeArr(transparentVertices,sizeof(transparentVertices),descArr,2)){
        flylog("init cubeObj failed!");
        return;
    }
    shader* shaderObj=new shader("res/shader/3d_1tex.vs","res/shader/3d_blend_1.fs");
    cubeObj->setShader(shaderObj);
//    cubeObj->setPosition(glm::vec3(-1.5f,  0.0f, -0.48f));
//    world::getInstance()->addChild(cubeObj);
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
        world::getInstance()->getControl()->bindNode(cube);
    }
    
    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane),descArr,2)){
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
    cubeObj->setPosition(glm::vec3(0,-0.3,0.5));
    cubeObj->rotateBy(glm::vec3(60,0,0));
    cubeObj->setScale(0.3);
    
    cubeTex* cubeObj2=new cubeTex("res/metal.png");
    if(!cubeObj2->init()){
        flylog("init cubeTex failed!");
        return;
    }
    cubeObj2->setPosition(glm::vec3(0.5,-0.2,0.5));
    cubeObj2->rotateBy(glm::vec3(60,0,0));
    cubeObj2->setScale(0.3);
    
    
    cubeTex* plainObj=new cubeTex("res/marble.jpg");
    int descArr[]={3,2};
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane),descArr,2)){
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
    if(!windowObj->initByVerticeArr(windowArr,sizeof(windowArr),descArr,2)){
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
    
    shader* windowShader=new shader("res/shader/3d_1tex.vs","res/shader/3d_blend_2.fs");
    windowObj->setShader(windowShader);

    camera* cam=world::getInstance()->getCamera();
    glm::vec3 cameroPos=cam->getPosition();
    std::vector<cubeTex*> vectorWindow;
    for(auto pos:vectorPos){
        cubeTex* windowCube=windowObj->clone();
        world::getInstance()->addChild(windowCube);
        world::getInstance()->getControl()->bindNode(windowCube);
        windowCube->setPosition(pos);
        windowCube->setScale(0.6);
        windowCube->rotateBy(glm::vec3(0,30,0));
        vectorWindow.push_back(windowCube);
        mapPos[glm::length(cameroPos-pos)]=pos;
    }
    
    //同时启用深度测试和GL_BLEND以后，后绘制的物体会找先绘制的颜色
    //如果先添加了近平面的物体，再添加远平面的东西
    //近平面添加以后，因为这时背影没有东西，所以只显示前面物体的颜色
    //绘制后面的物品时，因为深度测试的原因，部分处于遮挡的片元会被抛弃掉
    //所以整体的效果就是放在前面有透明的物体如玻璃，显示不出来后面的物体
    //所以需要按先后顺序进行绘制（即DrawCall)
    //先绘制后面的，再绘制前面的带透明效果的
    world::getInstance()->setCBBeforeAnyGLCall([vectorPos,vectorWindow](){
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
    int descArr[]={3,2};
    if(!plainObj->initByVerticeArr(g_verticeArrWithTexCoord_plane,sizeof(g_verticeArrWithTexCoord_plane),descArr,2)){
        flylog("init marbleObj failed!");
        return;
    }
    plainObj->setPosition(glm::vec3(0,0,0.5));

//    world::getInstance()->addChild(plainObj);
    
    world::getInstance()->addChild(modelObj);
    world::getInstance()->getControl()->bindNode(modelObj);
    
    world::getInstance()->setCBBeforeDrawCall([plainObj,modelObj](){
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
        //需要disable掉cull_face再绘制平面
        plainObj->draw();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    });
    
    timerUtil::getInstance()->exec(0.1,[](model* modelObj,cubeTex* plainObj){
        float t=(float)timeUtil::getTimeFloatSinceRun();
        float x=2*sin(t);
        float z=8*cos(t);
        modelObj->setPositionX(x);
        modelObj->setPositionZ(z-8);
        modelObj->rotateBy(glm::vec3(0,0.5,0));
    },modelObj,plainObj);
}

void test_facecull_2(){
    cubeTex* cubeObj=new cubeTex("res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0.2,-0.2));
    world::getInstance()->addChild(cubeObj);
    cubeObj->runAction(new forever(1,new rotateBy(1,glm::vec3(0,10,0))));
    world::getInstance()->getControl()->bindNode(cubeObj);
    world::getInstance()->setCBBeforeDrawCall([](){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    });

}
