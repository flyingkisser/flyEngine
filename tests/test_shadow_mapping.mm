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
#include "directionLight.h"
#include "pointLight.h"
#include "timerUtil.h"
#include "timeUtil.h"
#include "control.h"
#include "forever.h"
#include "rotateBy.h"
#include "fbo.h"
#include "sprite.h"
#include "shaderMgr.h"
#include "colors.h"
#include "skybox.h"

#ifdef BUILD_IOS
#import "ViewController.h"
#endif

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}
static void init_light_direction(float lightDensity){
    //设置环境光
    directionLight* dirLight=new directionLight(Color::GhostWhite,glm::vec3(-0.2,-1,-0.3),lightDensity,lightDensity,lightDensity,1);
    world::getInstance()->setDirectiontLight(dirLight);
}

//把深度信息绘制到一张2D纹理中
void test_shadow_mapping_to_tex(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setRotation(glm::vec3(30,30,0));
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    cubeTex* cubeObj2=new cubeTex("./res/wood.png");
    cubeObj2->init();
    cubeObj2->setPosition(glm::vec3(0,0.5,-2));
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
        glBindFramebuffer(GL_FRAMEBUFFER,stDepth.fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    shader* shDepth=new shader("./res/shader/depth_shadow.vs","./res/shader/draw_null.fs");
    shader* shDepth2d=new shader("./res/shader/2d_1tex.vs","./res/shader/2d_depth.fs");
    sprite* sp=new sprite(stDepth.texID);
    sp->flipY(true);
    sp->setShader(shDepth2d);

    world::getInstance()->setCBBeforeDrawCall([stDepth,shDepth,cam,sp,cubeObj,cubeObj2,floorObj,plight,plight2,view](){
        cam->updateUBOForShadow(plight->getPosition());
        
        cubeObj->setShader(shDepth);
        cubeObj2->setShader(shDepth);
        floorObj->setShader(shDepth);
        plight->setShader(shDepth);
        plight2->setShader(shDepth);
        
        cubeObj->draw();
        cubeObj2->draw();
        floorObj->draw();
        plight->draw();
        plight2->draw();

        glViewport(0,0,g_winWidth,g_winHigh);
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sp->draw();
    });
    
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

//working
void test_shadow_mapping_normal(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0.5,-1));
    cubeObj->setRotation(glm::vec3(60,60,0));
   
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
    plight->setPosition(glm::vec3(0,2,-6));
    plight->setScale(glm::vec3(0.2,0.2,0.2));
    plight->setVisible(false);
    world::getInstance()->addPointLight(plight);
    
    world::getInstance()->getControl()->bindNode(cubeObj);
//    world::getInstance()->getControl()->bindNode(floorObj);
   
    
    glm::vec3 lightViewPos=plight->getPosition();
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    glm::vec3 camPos=glm::vec3(0,4,12.7);
    cam->setPosition(camPos);
    cam->setYaw(-89);
    cam->setPitch(-16);
    cam->updateUBOForShadow(lightViewPos);
    
    shader* shDepth=new shader("./res/shader/depth_shadow.vs","./res/shader/draw_null.fs");
    shader* shDepth2d=new shader("./res/shader/2d_1tex.vs","./res/shader/2d_depth.fs");
    shader* shNormal=new shader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex_phong_shadow.fs");
    

    shNormal->use();
    shNormal->setBool("bCheckShadow",true,true);
    shNormal->setInt("texture0",0,true);
    shNormal->setInt("texture_shadow",1,true);
    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_2D, stDepth.texID);
    
    
    float near_plane=0.1f;
    float far_plane=100.0f;
    glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
    shDepth->use();
    shDepth->setMat4("proj", matProj);
    
//    sp->setShader(shDepth2d);
    
    world::getInstance()->setCBBeforeAnyGLCall([stDepth](){
        glBindFramebuffer(GL_FRAMEBUFFER,stDepth.fbo);
    });
    
    world::getInstance()->setCBBeforeDrawCall([matProj,sp,stDepth,shDepth,shNormal,cam,cubeObj,floorObj,plight](){
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::vec3 lightViewPos=plight->getPosition();
        glm::mat4 matCamera=glm::lookAt(lightViewPos, glm::vec3(0,0,0.0f), glm::vec3(0.0, 1.0, 0.0));   //look at center of scene
        shDepth->use();
        shDepth->setMat4("view", matCamera);
       
        //first pass
        cubeObj->setShader(shDepth);
        floorObj->setShader(shDepth);
        cubeObj->draw();
        floorObj->draw();

        //second pass
        glViewport(0,0,g_winWidth,g_winHigh);
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        sp->draw();
//        return;
        
        shNormal->use();
        shNormal->setMat4("matLightSpace",matProj*matCamera);
        cubeObj->setShader(shNormal);
        floorObj->setShader(shNormal);
        cubeObj->draw();
        floorObj->draw();
        plight->draw();
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
    
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


//working
void test_shadow_mapping_normal_multi_shadow_by_fbo(){
    std::vector<pointLight*> ligthtArr;
    std::vector<fboStruct> stArr;
    std::vector<glm::vec3> posArr;
    
    init_light_direction(0.1);
    material2* mtLight=createMaterial(0.1, 1, 1, 2);

    posArr.push_back(glm::vec3(0,2,-6));
//    posArr.push_back(glm::vec3(-1,2,0));
//    posArr.push_back(glm::vec3(-1,3,-0.5));
//    posArr.push_back(glm::vec3(-1,3,0.5));
    posArr.push_back(glm::vec3(0,2,3));
  
    int lightNum=posArr.size();
    for(int i=0;i<lightNum;i++){
        pointLight* plight=new pointLight(Color::IndianRed*0.1f,mtLight,0);
        if(!plight->init()){
           flylog("plight light init failed!");
           return;
        }
        plight->setPosition(posArr[i]);
        plight->setScale(glm::vec3(0.2,0.2,0.2));
        plight->setVisible(false);
        world::getInstance()->addPointLight(plight);
        stArr.push_back(fbo::createFBOForDepth());
        ligthtArr.push_back(plight);
    }
  
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/floor.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0.5,-1));
    cubeObj->setRotation(glm::vec3(60,60,0));
   
    cubeTex* floorObj=new cubeTex("./res/floor.png");
    int descArr[]={3,2,3};
    if(!floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane,sizeof(g_verticeArrWithTexCoordAndNormal_plane),descArr,3)){
        flylog("init plainObj failed!");
        return;
    }
    floorObj->setPosition(glm::vec3(0,-1,0));
    
    
    world::getInstance()->getControl()->bindNode(cubeObj);
//    world::getInstance()->getControl()->bindNode(floorObj);
   
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    glm::vec3 camPos=glm::vec3(0,4,12.7);
    cam->setPosition(camPos);
    cam->setYaw(-89);
    cam->setPitch(-16);
//    cam->updateUBOForShadow(plight0->getPosition());
    
    shader* shDepth=new shader("./res/shader/depth_shadow.vs","./res/shader/draw_null.fs");
    shader* shDepth2d=new shader("./res/shader/2d_1tex.vs","./res/shader/2d_depth.fs");
    shader* shNormal=new shader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex_phong_shadow_multi.fs");
    
    shNormal->use();
    shNormal->setBool("bCheckShadow",true,true);
    shNormal->setInt("shadowNum",lightNum,true);
    shNormal->setInt("texture0",0,true);
    
    for(int i=0;i<lightNum;i++){
        char szBuf[128]={0};
        snprintf(szBuf, sizeof(szBuf), "texture_shadow%d",i);
        shNormal->setInt(szBuf,i+1,true);
        glActiveTexture(GL_TEXTURE0+1+i);
        glBindTexture(GL_TEXTURE_2D, stArr[i].texID);
    }
    
    float near_plane=0.1f;
    float far_plane=100.0f;
    glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
    shDepth->use();
    shDepth->setMat4("proj", matProj);
    
    sprite* sp=new sprite(stArr[0].texID);
    sp->flipY(true);
    sp->setShader(shDepth2d);
    
    world::getInstance()->setCBBeforeDrawCall([lightNum,stArr,ligthtArr,shDepth,shNormal,cam,cubeObj,floorObj,matProj,sp](){
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
        cubeObj->setShader(shDepth);
        floorObj->setShader(shDepth);
        for(int i=0;i<lightNum;i++){
            glm::mat4 matView=glm::lookAt(ligthtArr[i]->getPosition(), glm::vec3(0,0,0.0f), glm::vec3(0.0, 1.0, 0.0));
            glBindFramebuffer(GL_FRAMEBUFFER,stArr[i].fbo);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shDepth->use();
            shDepth->setMat4("view", matView);
            cubeObj->draw();
            floorObj->draw();
        }
        
        //second pass
        glViewport(0,0,g_winWidth,g_winHigh);
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        sp->draw();
//        return;
        shNormal->use();
        cubeObj->setShader(shNormal);
        floorObj->setShader(shNormal);
        for(int i=0;i<lightNum;i++){
            char szBuf[128]={0};
            snprintf(szBuf, sizeof(szBuf), "matLightSpace[%d]",i);
            glm::mat4 matView=glm::lookAt(ligthtArr[i]->getPosition(), glm::vec3(0,0,0.0f), glm::vec3(0.0, 1.0, 0.0));
            shNormal->setMat4(szBuf,matProj*matView);
        }
        cubeObj->draw();
        floorObj->draw();
        for(int i=0;i<lightNum;i++)
            ligthtArr[i]->draw();
    });
    
    int lightIndex=0;
    world::getInstance()->getControl()->regOnKeyPress('g', [&lightIndex](){
        lightIndex++;
        flylog("lightIndex %d",lightIndex);
    });
    
    pointLight* plight0=ligthtArr[0];
    world::getInstance()->getControl()->regOnKeyPress('1', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('2', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('3', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionY(pos.y+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('4', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionY(pos.y-0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('5', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionZ(pos.z+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('6', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionZ(pos.z-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('7', [cubeObj](){
        glm::vec3 pos=cubeObj->getPosition();
        cubeObj->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('8', [cubeObj](){
        glm::vec3 pos=cubeObj->getPosition();
        cubeObj->setPositionX(pos.x-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('9', [cubeObj](){
        glm::vec3 pos=cubeObj->getPosition();
        cubeObj->setPositionY(pos.y+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('0', [cubeObj](){
        glm::vec3 pos=cubeObj->getPosition();
        cubeObj->setPositionY(pos.y-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('-', [cubeObj](){
        glm::vec3 pos=cubeObj->getPosition();
        cubeObj->setPositionZ(pos.z+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('=', [cubeObj](){
        glm::vec3 pos=cubeObj->getPosition();
        cubeObj->setPositionZ(pos.z-0.1);
    });
}

//working
//用sampler2DArray来实现多光源时，把多个光照形成的shadow map绘制进一个纹理数组里
//最终在物体的渲染管线中，从这个纹理数组中进行采样
void test_shadow_mapping_normal_multi_shadow_sampler2DArray(){
    std::vector<pointLight*> ligthtArr;
    std::vector<glm::vec3> posArr;
    init_light_direction(0.1);
    material2* mtLight=createMaterial(0.1, 1, 1, 2);
    posArr.push_back(glm::vec3(0,2,-6));
    posArr.push_back(glm::vec3(-1,2,0));
    posArr.push_back(glm::vec3(-1,3,-0.5));
    posArr.push_back(glm::vec3(-1,3,0.5));
    posArr.push_back(glm::vec3(0,2,3));
    int lightNum=posArr.size();
    //创建fbo绑定指定光源数量的texture_2d_array
    fboStruct st=fbo::createFBOForDepthSampler2DArray(lightNum);
    for(int i=0;i<lightNum;i++){
        pointLight* plight=new pointLight(Color::IndianRed*0.1f,mtLight,0);
        if(!plight->init()){
           flylog("plight light init failed!");
           return;
        }
        plight->setPosition(posArr[i]);
        plight->setScale(glm::vec3(0.2,0.2,0.2));
        plight->setVisible(false);
        world::getInstance()->addPointLight(plight);
        ligthtArr.push_back(plight);
    }
    
    cubeTex* cubeObj=new cubeTex("./res/floor.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0.5,-1));
    cubeObj->setRotation(glm::vec3(60,60,0));
    cubeTex* floorObj=new cubeTex("./res/floor.png");
    int descArr[]={3,2,3};
    if(!floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane,sizeof(g_verticeArrWithTexCoordAndNormal_plane),descArr,3)){
        flylog("init plainObj failed!");
        return;
    }
    floorObj->setPosition(glm::vec3(0,-1,0));
    world::getInstance()->getControl()->bindNode(cubeObj);
//    world::getInstance()->getControl()->bindNode(floorObj);
   
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    camera* cam=world::getInstance()->getCamera();
    glm::vec3 camPos=glm::vec3(0,4,12.7);
    cam->setPosition(camPos);
    cam->setYaw(-89);
    cam->setPitch(-16);

    shader* shDepth=new shader("./res/shader/depth_cubetex_shadow.vs","./res/shader/draw_null.fs","./res/shader/depth_cubetex_shadow_csm.gs");
    shader* shNormal=new shader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex_phong_shadow_sampler2DArray.fs");
    shNormal->use();
    shNormal->setBool("bCheckShadow",true,true);
    shNormal->setInt("shadowNum",lightNum,true);
    shNormal->setInt("texture0",0,true);
    shNormal->setInt("texture_shadow0",1,true);
    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, st.texID);
    
    float near_plane=0.1f;
    float far_plane=100.0f;
    glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
    shDepth->use();
    shDepth->setMat4("proj", matProj);
    shDepth->setMat4("matModel", glm::mat4(1.0));

    sprite* sp=new sprite(st.texID);
//    shader* shDepth2dDebug=new shader("./res/shader/2d_1tex.vs","./res/shader/2d_depth_array.fs");
//    sp->flipY(true);
//    sp->setShader(shDepth2dDebug);
//    shDepth2dDebug->use();
//    shDepth2dDebug->setInt("texture0",0,true);
//    shDepth2dDebug->setInt("lightIndex",15);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D_ARRAY,st.texID);
    
    world::getInstance()->setCBBeforeDrawCall([lightNum,st,ligthtArr,shDepth,shNormal,cam,cubeObj,floorObj,matProj,sp](){
        //第一个pass，把每个灯的view矩阵写入shader
        //因为在glTexImage3D设置texture_2d_array纹理时使用了5个维度，以及在depth_cubetex_shadow_csm.gs里硬编码了5个invocations
        //所以这里需要写入5个灯的view数据
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
//        glFramebufferTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D_ARRAY, st.texID, 0);
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shDepth->use();
        for(int i=0;i<lightNum;i++){
            glm::mat4 matView=glm::lookAt(ligthtArr[i]->getPosition(), glm::vec3(0,0,0.0f), glm::vec3(0.0, 1.0, 0.0));
            shDepth->setMat4("lightSpaceMatrices["+std::to_string(i)+"]",matView);
        }
        cubeObj->setShader(shDepth);
        cubeObj->draw();
        floorObj->setShader(shDepth);
        floorObj->draw();

        //second pass
        glViewport(0,0,g_winWidth,g_winHigh);
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
//        shDepth2d->use();
//        sp->updateModel();
//        glBindVertexArray(sp->getVAO());
//        glDrawArrays(GL_TRIANGLES,0,6);
//        return;
       
        shNormal->use();
        cubeObj->setShader(shNormal);
        floorObj->setShader(shNormal);
        for(int i=0;i<lightNum;i++){
            glm::mat4 matView=glm::lookAt(ligthtArr[i]->getPosition(), glm::vec3(0,0,0.0f), glm::vec3(0.0, 1.0, 0.0));
            shNormal->setMat4("matLightSpace["+std::to_string(i)+"]",matProj*matView);
        }
        cubeObj->draw();
        floorObj->draw();
        for(int i=0;i<lightNum;i++)
            ligthtArr[i]->draw();
    });
    
    int lightIndex=1;
    world::getInstance()->getControl()->regOnKeyPress('g', [&lightIndex](){
        lightIndex++;
        flylog("lightIndex %d",lightIndex);
    });
    
    pointLight* plight0=ligthtArr[lightIndex];
    world::getInstance()->getControl()->regOnKeyPress('1', [ligthtArr](){
        pointLight* plight0=ligthtArr[0];
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('2', [ligthtArr](){
        pointLight* plight0=ligthtArr[0];
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('3', [ligthtArr](){
        pointLight* plight0=ligthtArr[1];
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('4', [ligthtArr](){
        pointLight* plight0=ligthtArr[1];
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('5', [ligthtArr](){
        pointLight* plight0=ligthtArr[2];
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('6', [ligthtArr](){
        pointLight* plight0=ligthtArr[2];
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('7', [ligthtArr](){
        pointLight* plight0=ligthtArr[3];
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('8', [ligthtArr](){
        pointLight* plight0=ligthtArr[3];
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('9', [ligthtArr](){
        pointLight* plight0=ligthtArr[4];
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('0', [ligthtArr](){
        pointLight* plight0=ligthtArr[4];
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('-', [cubeObj](){
        glm::vec3 pos=cubeObj->getPosition();
        cubeObj->setPositionZ(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('=', [cubeObj](){
        glm::vec3 pos=cubeObj->getPosition();
        cubeObj->setPositionZ(pos.x-0.1);
    });
}



//使用cubemap来存储阴影数据
void test_shadow_by_cubemap(){
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
    
    world::getInstance()->getControl()->bindNode(huouseObj);
    
    fboStruct st=fbo::createFBOForDepthWithCubemap();

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
    cam->setYaw(-9.03);
    cam->setPitch(4.78);
  
//    shader* shNormal=huouseObj->getShader();
    shader* shDepth=new shader("./res/shader/depth_cubetex_shadow.vs","./res/shader/depth_cubetex_shadow.fs","./res/shader/depth_cubetex_shadow.gs");
    glm::mat4 matProj=glm::perspective(glm::radians(90.0f),(float)g_shadowWidth/(float)g_shadowHigh,1.0f,100.0f);
//    shader* shNormal=new shader("./res/shader/3d_1tex.vs","./res/shader/debug_shadow_cubetex.fs");
    shader* shNormal=new shader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex_phong_shadow_cubetex.fs");
 
    shDepth->use();
    shDepth->setFloat("far_plane", 100.0,true);
    shDepth->setVec3("posLight",lightViewPos,true);
    shDepth->setMat4("proj", matProj);
    shDepth->setMat4("matModel", glm::mat4(1.0f));
  
    shNormal->use();
    shNormal->setVec3("lightPos", lightViewPos,true);
    shNormal->setFloat("far_plane", 100.0,true);
    shNormal->setInt("texture_depth_cube",1,true);
   
    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, st.texID);
    
    skybox* skyObj=new skybox(st.texID);
    
//    world::getInstance()->addPass(stDepth.fbo, shDepth, [](){
//        glViewport(0,0,g_shadowWidth,g_shadowHigh);
//    });
//    world::getInstance()->addPass(0, shNormal, [](){
//        glDisable(GL_CULL_FACE);
//        glViewport(0,0,g_winWidth,g_winHigh);
//    });
    
    world::getInstance()->setCBBeforeDrawCall([skyObj,st,shDepth,shNormal,plight,cam,matProj,huouseObj,cubeObj1,cubeObj2,cubeObj3,cubeObj4,cubeObj5](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
        std::vector<glm::mat4> vecMat;
        glm::vec3 lightViewPos=plight->getPosition();
        vecMat.push_back(glm::lookAt(lightViewPos, lightViewPos+glm::vec3(1,0,0), glm::vec3(0,-1,0)));
        vecMat.push_back(glm::lookAt(lightViewPos, lightViewPos+glm::vec3(-1,0,0), glm::vec3(0,-1,0)));
        vecMat.push_back(glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,1,0), glm::vec3(0,0,1)));
        vecMat.push_back(glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,-1,0), glm::vec3(0,0,-1)));
        vecMat.push_back(glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,1), glm::vec3(0,-1,0)));
        vecMat.push_back(glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,-1), glm::vec3(0,-1,0)));
        shDepth->use();
        for(int i=0;i<lightViewPos.length();i++)
            shDepth->setMat4("matViewArr["+std::to_string(i)+"]",vecMat[i]);
        shDepth->setVec3("posLight", glm::value_ptr(lightViewPos),true);
        huouseObj->setShader(shDepth);
        cubeObj1->setShader(shDepth);
        cubeObj2->setShader(shDepth);
        cubeObj3->setShader(shDepth);
        cubeObj4->setShader(shDepth);
        cubeObj5->setShader(shDepth);
        huouseObj->draw();
        cubeObj1->draw();
        cubeObj2->draw();
        cubeObj3->draw();
        cubeObj4->draw();
        cubeObj5->draw();
    
        //second pass
        glViewport(0,0,g_winWidth,g_winHigh);
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
//        skyObj->draw();
//        return;
        huouseObj->setShader(shNormal);
        cubeObj1->setShader(shNormal);
        cubeObj2->setShader(shNormal);
        cubeObj3->setShader(shNormal);
        cubeObj4->setShader(shNormal);
        cubeObj5->setShader(shNormal);
        huouseObj->draw();
        cubeObj1->draw();
        cubeObj2->draw();
        cubeObj3->draw();
        cubeObj4->draw();
        cubeObj5->draw();
        plight->draw();
        
    });

    pointLight* plight0=plight;
    world::getInstance()->getControl()->regOnKeyPress('1', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('2', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('3', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionY(pos.y+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('4', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionY(pos.y-0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('5', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionZ(pos.z+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('6', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionZ(pos.z-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
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
//    sprite* sp=new sprite(stDepth.texID);
//    sp->flipY(true);

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
