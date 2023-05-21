//
//  test_pbr.cpp
//  flyEngine
//
//  Created by Joe on 2022/12/14.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_pbr.h"
#include "world.h"
#include "control.h"
#include "logUtil.h"
#include "timerUtil.h"
#include "timeUtil.h"
#include "directionLight.h"
#include "cubeTex.h"
#include "quadSize.h"
#include "semiSphere.h"
#include "material2.h"
#include "texture2.h"
#include "textureHdr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "randUtil.h"
#include "threadUtil.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"
#include "skybox.h"
#include "glslUtil.h"
#include "values.h"
#include "camera.h"
#include "model.h"
#include "fbo.h"
#include "window.h"

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

static void init_light_direction(){
    //设置环境光
    directionLight* dirLight=new directionLight(glm::vec3(0.7f,0.7f,0.7f),glm::vec3(-0.2,-1,-0.3),0.1,0.1,0.1,2);
    world::getInstance()->setDirectiontLight(dirLight);
}


void test_PBR_cubeColor(){
    init_light_direction();
    shader* shPBR=new shader("res/shader/3d_color.vs","res/shader/3d_color_pbr.fs");
    
    shPBR->use();
    shPBR->setVec3("albedo", (float*)glm::value_ptr(glm::vec3(0.5,0.0,0.0)));
    shPBR->setFloat("metallic", 0.0);
    shPBR->setFloat("roughness", 0.0);
    shPBR->setFloat("ao", 1.0);
    
    //普通cube
    for(int i=0;i<7;i++){
        shPBR->setFloat("metallic", (float)i / (float)7.0);
        for(int j=0;j<7;j++){
            semiSphere* cubeObj=new semiSphere(glm::vec3(300));
            if(!cubeObj->init()){
                flylog("node init failed!");
                return;
            }
            cubeObj->setPosition(glm::vec3(j*0.3-0.8,i*0.3-0.8,-2));
            cubeObj->setScale(glm::vec3(0.1,0.15,0.2));
            cubeObj->setShader(shPBR);
            world::getInstance()->addChild(cubeObj);
            world::getInstance()->getControl()->bindNode(cubeObj);
            shPBR->setFloat("roughness", glm::clamp((float)j / (float)7.0, 0.05f, 1.0f));
                        
        }
    }
  
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(300),mtLight,0);
    pointLight* lightObj2=new pointLight(glm::vec3(300),mtLight,0);
    pointLight* lightObj3=new pointLight(glm::vec3(300),mtLight,0);
    pointLight* lightObj4=new pointLight(glm::vec3(300),mtLight,0);
    lightObj1->init();
    lightObj2->init();
    lightObj3->init();
    lightObj4->init();
    
    lightObj1->setPosition(glm::vec3(-0.5,0.5,-1));
    lightObj2->setPosition(glm::vec3(0.5,0.5,-1));
    lightObj3->setPosition(glm::vec3(-1,-1,-1));
    lightObj4->setPosition(glm::vec3(1,-1,-1));
    
    lightObj1->setScale(glm::vec3(0.2));
    lightObj2->setScale(glm::vec3(0.2));
    lightObj3->setScale(glm::vec3(0.2));
    lightObj4->setScale(glm::vec3(0.2));
 
    world::getInstance()->addPointLight(lightObj1);
    world::getInstance()->addPointLight(lightObj2);
    world::getInstance()->addPointLight(lightObj3);
    world::getInstance()->addPointLight(lightObj4);
   
    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
//    timerMgrObj->exec(0.1,[](node* lightObj1){
//        float radius=timeUtil::getTimeFloatSinceRun();
//        lightObj1->setPositionX(1.5*cos(radius));
//        lightObj1->setPositionY(1.5*sin(radius));
//    },lightObj1);
    controlObj->bindNode(lightObj1);
    controlObj->bindNode(lightObj2);
    controlObj->bindNode(lightObj3);
    controlObj->bindNode(lightObj4);
 
    camera* cam=world::getInstance()->getCamera();
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam,shPBR](){
        shPBR->use();
        float metallic=shPBR->getFloat("metallic");
        metallic+=0.1;
        if(metallic>=1)
            metallic=1.0;
        shPBR->setFloat("metallic", metallic);
        flylog("set metallic %f",metallic);
    });
    world::getInstance()->getControl()->regOnKeyPress('h', [cam,shPBR](){
        shPBR->use();
        float metallic=shPBR->getFloat("metallic");
        metallic-=0.1;
        if(metallic<=0)
            metallic=0.0;
        shPBR->setFloat("metallic", metallic);
        flylog("set metallic %f",metallic);
    });
    world::getInstance()->getControl()->regOnKeyPress('j', [cam,shPBR](){
        shPBR->use();
        float roughness=shPBR->getFloat("roughness");
        roughness+=0.1;
        if(roughness>=1)
            roughness=1.0;
        shPBR->setFloat("roughness", roughness);
        flylog("set roughness %f",roughness);
    });
    world::getInstance()->getControl()->regOnKeyPress('k', [cam,shPBR](){
        shPBR->use();
        float roughness=shPBR->getFloat("roughness");
        roughness-=0.1;
        if(roughness<=0)
            roughness=0.0;
        shPBR->setFloat("roughness", roughness);
        flylog("set roughness %f",roughness);
    });
    
}

void test_PBR_cubeTex(){
    init_light_direction();
    shader* shPBR=new shader("res/shader/3d_1tex_pbr.vs","res/shader/3d_1tex_pbr.fs");
    shPBR->use();
    shPBR->setInt("tex_albedo", 0);
    shPBR->setInt("tex_normal", 1);
    shPBR->setInt("tex_metallic", 2);
    shPBR->setInt("tex_roughness", 3);
    shPBR->setInt("tex_ao", 4);
    
    texture2* tex0=new texture2("./res/pbr_ball/albedo.png");
    texture2* tex1=new texture2("./res/pbr_ball/normal.png");
    texture2* tex2=new texture2("./res/pbr_ball/metallic.png");
    texture2* tex3=new texture2("./res/pbr_ball/roughness.png");
    texture2* tex4=new texture2("./res/pbr_ball/ao.png");
    
    tex0->init();
    tex1->init();
    tex2->init();
    tex3->init();
    tex4->init();
    
    tex0->glInit(GL_TEXTURE0);
    tex1->glInit(GL_TEXTURE1);
    tex2->glInit(GL_TEXTURE2);
    tex3->glInit(GL_TEXTURE3);
    tex4->glInit(GL_TEXTURE4);
    
    world::getInstance()->setCBBeforeDrawCall([tex0,tex1,tex2,tex3,tex4](){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex0->getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex1->getTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,tex2->getTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,tex3->getTextureID());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,tex4->getTextureID());
    });

    //普通cube
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            semiSphere* cubeObj=new semiSphere(glm::vec3(300));
            if(!cubeObj->init()){
                flylog("semiSphere init failed!");
                return;
            }
            cubeObj->setPosition(glm::vec3(j*0.3-0.8,i*0.3-0.8,-2));
            cubeObj->setScale(glm::vec3(0.1,0.15,0.2));
            cubeObj->setShader(shPBR);
            world::getInstance()->addChild(cubeObj);
            world::getInstance()->getControl()->bindNode(cubeObj);
        }
    }
  
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(2),mtLight,0);
    pointLight* lightObj2=new pointLight(glm::vec3(0.2),mtLight,0);
    pointLight* lightObj3=new pointLight(glm::vec3(0.3),mtLight,0);
    pointLight* lightObj4=new pointLight(glm::vec3(0.1),mtLight,0);
    lightObj1->init();
    lightObj2->init();
    lightObj3->init();
    lightObj4->init();
    
    lightObj1->setPosition(glm::vec3(-0.5,0.5,-1));
    lightObj2->setPosition(glm::vec3(0.5,0.5,-1));
    lightObj3->setPosition(glm::vec3(-1,-1,-1));
    lightObj4->setPosition(glm::vec3(1,-1,-1));
    
    lightObj1->setScale(glm::vec3(0.2));
    lightObj2->setScale(glm::vec3(0.2));
    lightObj3->setScale(glm::vec3(0.2));
    lightObj4->setScale(glm::vec3(0.2));
 
    world::getInstance()->addPointLight(lightObj1);
    world::getInstance()->addPointLight(lightObj2);
    world::getInstance()->addPointLight(lightObj3);
    world::getInstance()->addPointLight(lightObj4);
   
    controlObj->bindNode(lightObj1);
    controlObj->bindNode(lightObj2);
    controlObj->bindNode(lightObj3);
    controlObj->bindNode(lightObj4);
    
    pointLight* plight0=lightObj1;
    float move=0.2;
    world::getInstance()->getControl()->regOnKeyPress('1', [plight0,move](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+move);
    });
    world::getInstance()->getControl()->regOnKeyPress('2', [plight0,move](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-move);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('3', [plight0,move](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionY(pos.y+move);
    });
    world::getInstance()->getControl()->regOnKeyPress('4', [plight0,move](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionY(pos.y-move);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('5', [plight0,move](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionZ(pos.z+move);
    });
    world::getInstance()->getControl()->regOnKeyPress('6', [plight0,move](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionZ(pos.z-move);
    });
    
}

//IBL: image based lighting
//render lighting to a cubemap
void test_PBR_ibl_1(){
    init_light_direction();
    //***********
    //加载等矩贴图
    //***********
    textureHdr* texHDR=new textureHdr("./res/pbr_ibl/newport_loft.hdr");
    texHDR->init();
    texHDR->glInit(GL_TEXTURE0);

    //**********************************************
    //第一个pass
    //分六次采样等矩贴图的六个区域，输出到cubemap的6个贴图里
    //即把一个平面图转换成cubemap的过程
    //**********************************************
    fboStruct st=fbo::createFBOForIBLWithCubemap();
    
    glm::mat4 proj=glm::perspective(glm::radians(90.0), 1.0, 0.1, 10.0);
    glm::mat4 viewArr[]={
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,-1,0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(-1,0,0), glm::vec3(0,-1,0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,-1,0), glm::vec3(0,0,-1)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec3(0,-1,0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,-1,0))
    };
    shader* shRenderSixFace=new shader("res/shader/cubemap.vs","res/shader/equirectangular2cubemap.fs");
    shRenderSixFace->use();
    shRenderSixFace->setInt("texHDR", 0);
    shRenderSixFace->setMat4("proj", glm::value_ptr(proj));
  
    cubeTex* cubeRenderSixFace=new cubeTex(texHDR->getTextureID());
    cubeRenderSixFace->init();
    cubeRenderSixFace->setShader(shRenderSixFace);
    
    glViewport(0,0,512,512);
    glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texHDR->getTextureID());
    for(int i=0;i<6;i++){
        shRenderSixFace->setMat4("view", glm::value_ptr(viewArr[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,st.texID,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        cubeRenderSixFace->draw();
    }

    //**********************
    //第二个pass
    //以这个cubemap绘制skybox
    //**********************
#ifdef BUILD_MAC
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
    [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
    shader* shBackground=new shader("res/shader/pbr_ibl_background.vs","res/shader/pbr_ibl_background.fs");
    shBackground->use();
    shBackground->setInt("texCubeMap", 0);
//    skybox* skyBackground=new skybox(texHDR->getTextureID(),shBackground);
    skybox* skyBackground=new skybox(st.texID,shBackground);
    world::getInstance()->addChild(skyBackground);


    //很多个球使用pbr贴图渲染
    shader* shPBR=new shader("res/shader/3d_1tex_pbr.vs","res/shader/3d_1tex_pbr.fs");
    shPBR->use();
    shPBR->setInt("tex_albedo", 0);
    shPBR->setInt("tex_normal", 1);
    shPBR->setInt("tex_metallic", 2);
    shPBR->setInt("tex_roughness", 3);
    shPBR->setInt("tex_ao", 4);
    
    texture2* tex0=new texture2("./res/pbr_ball/albedo.png");
    texture2* tex1=new texture2("./res/pbr_ball/normal.png");
    texture2* tex2=new texture2("./res/pbr_ball/metallic.png");
    texture2* tex3=new texture2("./res/pbr_ball/roughness.png");
    texture2* tex4=new texture2("./res/pbr_ball/ao.png");
    
    tex0->init();
    tex1->init();
    tex2->init();
    tex3->init();
    tex4->init();
    
    tex0->glInit(GL_TEXTURE0);
    tex1->glInit(GL_TEXTURE1);
    tex2->glInit(GL_TEXTURE2);
    tex3->glInit(GL_TEXTURE3);
    tex4->glInit(GL_TEXTURE4);
    
    world::getInstance()->setCBBeforeDrawCall([tex0,tex1,tex2,tex3,tex4](){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex0->getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex1->getTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,tex2->getTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,tex3->getTextureID());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,tex4->getTextureID());
    });
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            semiSphere* cubeObj=new semiSphere(glm::vec3(300));
            if(!cubeObj->init()){
                flylog("semiSphere init failed!");
                return;
            }
            cubeObj->setPosition(glm::vec3(j*0.3-0.8,i*0.3-0.8,-2));
            cubeObj->setScale(glm::vec3(0.1,0.15,0.2));
            cubeObj->setShader(shPBR);
            world::getInstance()->addChild(cubeObj);
            world::getInstance()->getControl()->bindNode(cubeObj);
        }
    }
  
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(100,0,0),mtLight,0);
    pointLight* lightObj2=new pointLight(glm::vec3(0,100,0),mtLight,0);
    pointLight* lightObj3=new pointLight(glm::vec3(0,0,100),mtLight,0);
    pointLight* lightObj4=new pointLight(glm::vec3(100,100,20),mtLight,0);
    lightObj1->init();
    lightObj2->init();
    lightObj3->init();
    lightObj4->init();
    
    lightObj1->setPosition(glm::vec3(-0.5,0.5,-0.8));
    lightObj2->setPosition(glm::vec3(0.5,0.5,-0.8));
    lightObj3->setPosition(glm::vec3(-1,-1,0.5));
    lightObj4->setPosition(glm::vec3(0.2,0.2,0));
    
    lightObj1->setScale(glm::vec3(0.2));
    lightObj2->setScale(glm::vec3(0.2));
    lightObj3->setScale(glm::vec3(0.2));
    lightObj4->setScale(glm::vec3(0.2));
 
    world::getInstance()->addPointLight(lightObj1);
    world::getInstance()->addPointLight(lightObj2);
    world::getInstance()->addPointLight(lightObj3);
    world::getInstance()->addPointLight(lightObj4);
   
    controlObj->bindNode(lightObj1);
    controlObj->bindNode(lightObj2);
    controlObj->bindNode(lightObj3);
    controlObj->bindNode(lightObj4);
}

void test_PBR_ibl_irradiance(){
    init_light_direction();
    camera* cam=world::getInstance()->getCamera();
    glm::mat4 proj = cam->getPerspectiveMatrix();
    glm::mat4 projForCap = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 viewArr[] ={
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    
    unsigned int hdrTexture;
    textureHdr* texHDR=new textureHdr("res/pbr_ibl/newport_loft.hdr");
    texHDR->init();
    texHDR->glInit();
    hdrTexture=texHDR->getTextureID();

//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.

    shader* shPBR=new shader("res/shader/3d_1tex_pbr.vs","res/shader/3d_1tex_pbr_irradiance.fs");
    shader* shBackground=new shader("res/shader/pbr_ibl_background.vs", "res/shader/pbr_ibl_background.fs");
    shader* shRenderSixFace=new shader("res/shader/cubemap.vs", "res/shader/equirectangular2cubemap.fs");
    shader* shIrradiance=new shader("res/shader/cubemap.vs", "res/shader/irradiance.fs");

    shRenderSixFace->use();
    shRenderSixFace->setInt("texHDR", 0);
    shRenderSixFace->setMat4("proj", projForCap);

    shIrradiance->use();
    shIrradiance->setInt("texEnvMap", 0);
    shIrradiance->setMat4("proj", projForCap);
    
    shPBR->use();
    shPBR->setInt("tex_albedo", 0);
    shPBR->setInt("tex_normal", 1);
    shPBR->setInt("tex_metallic", 2);
    shPBR->setInt("tex_roughness", 3);
    shPBR->setInt("tex_ao", 4);
    shPBR->setInt("tex_irradiance", 5);
  
    shBackground->use();
    shBackground->setInt("texCubeMap", 0);
    shBackground->setMat4("proj", proj);
    shBackground->setMat4("view", cam->getLookAtMatrix());
    
    //把等矩贴图渲染到一张cubemap中
    fboStruct st=fbo::createFBOForIBLWithCubemap();
    cubeTex* cubeRenderSixFace=new cubeTex(texHDR->getTextureID());
    cubeRenderSixFace->init();
    cubeRenderSixFace->setShader(shRenderSixFace);
    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    for (unsigned int i = 0; i < 6; ++i){
        shRenderSixFace->setMat4("view", viewArr[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, st.texID, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeRenderSixFace->draw();
    }
    
    //把环境光渲染到一张cubemap的过程
    //从上面的cubemap中采样，经过计算体积分光照
    unsigned int irradianceMap;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i){
       glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, st.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    //注意这里一定要用skybox进行渲染，因为st.texID是一个cubemap，不能用cubeTex对象!在这里卡了3天
    skybox* cubeIrradiance=new skybox(st.texID,shIrradiance);
    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    for (unsigned int i = 0; i < 6; ++i){
        shIrradiance->setMat4("view", viewArr[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeIrradiance->drawSimple();   //这里一定要调用没有设置depth function的版本，否则无法渲染进irradianceMap
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, g_winWidth, g_winHigh);
  
    skybox* skyBackground=new skybox(st.texID,shBackground);
    world::getInstance()->addChild(skyBackground);
    
//    int scrWidth, scrHeight;
//    glfwGetFramebufferSize(g_window, &scrWidth, &scrHeight);
    
    //很多个球使用pbr贴图渲染
    texture2* tex0=new texture2("./res/pbr_ball/albedo.png");
    texture2* tex1=new texture2("./res/pbr_ball/normal.png");
    texture2* tex2=new texture2("./res/pbr_ball/metallic.png");
    texture2* tex3=new texture2("./res/pbr_ball/roughness.png");
    texture2* tex4=new texture2("./res/pbr_ball/ao.png");

    tex0->init();
    tex1->init();
    tex2->init();
    tex3->init();
    tex4->init();

    tex0->glInit(GL_TEXTURE0);
    tex1->glInit(GL_TEXTURE1);
    tex2->glInit(GL_TEXTURE2);
    tex3->glInit(GL_TEXTURE3);
    tex4->glInit(GL_TEXTURE4);

    world::getInstance()->setCBBeforeDrawCall([tex0,tex1,tex2,tex3,tex4,irradianceMap](){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex0->getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex1->getTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,tex2->getTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,tex3->getTextureID());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,tex4->getTextureID());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
    });
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            semiSphere* cubeObj=new semiSphere(glm::vec3(300));
            if(!cubeObj->init()){
                flylog("semiSphere init failed!");
                return;
            }
            cubeObj->setPosition(glm::vec3(j*0.3-0.8,i*0.3-0.8,-2));
            cubeObj->setScale(glm::vec3(0.1,0.15,0.2));
            cubeObj->setShader(shPBR);
            world::getInstance()->addChild(cubeObj);
            world::getInstance()->getControl()->bindNode(cubeObj);
        }
    }
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(1,0,0),mtLight,0);
    pointLight* lightObj2=new pointLight(glm::vec3(0,1,0),mtLight,0);
    pointLight* lightObj3=new pointLight(glm::vec3(0,0,1),mtLight,0);
    pointLight* lightObj4=new pointLight(glm::vec3(1,1,0.2),mtLight,0);
    lightObj1->init();
    lightObj2->init();
    lightObj3->init();
    lightObj4->init();
    
    lightObj1->setPosition(glm::vec3(-0.5,0.5,-0.8));
    lightObj2->setPosition(glm::vec3(0.5,0.5,-0.8));
    lightObj3->setPosition(glm::vec3(-0.6,-0.6,-0.8));
    lightObj4->setPosition(glm::vec3(0.2,0.2,-0.8));
    
    lightObj1->setScale(glm::vec3(0.2));
    lightObj2->setScale(glm::vec3(0.2));
    lightObj3->setScale(glm::vec3(0.2));
    lightObj4->setScale(glm::vec3(0.2));
 
    world::getInstance()->addPointLight(lightObj1);
    world::getInstance()->addPointLight(lightObj2);
    world::getInstance()->addPointLight(lightObj3);
    world::getInstance()->addPointLight(lightObj4);
   
    controlObj->bindNode(lightObj1);
    controlObj->bindNode(lightObj2);
    controlObj->bindNode(lightObj3);
    controlObj->bindNode(lightObj4);
}

//IBL: image based lighting
//render lighting to a cubemap
//把周围环境渲染到第二个cubemap中，后面的pbr shader从这个cubemap中采样出光照需要的参数
//和上面那个测试函数无本质区别，就是按两种方式写出来的
void test_PBR_ibl_irradiance2(){
//    init_light_direction();
    camera* cam=world::getInstance()->getCamera();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.

    //***********
    //加载等矩贴图
    //***********
    textureHdr* texHDR=new textureHdr("./res/pbr_ibl/newport_loft.hdr");
    texHDR->init();
    texHDR->glInit();

    //**********************************************
    //第一个pass
    //分六次采样等矩贴图的六个区域，输出到cubemap的6个贴图里
    //即把一个平面图转换成cubemap的过程
    //**********************************************
    fboStruct st=fbo::createFBOForIBLWithCubemap();
    
    glm::mat4 proj=glm::perspective(glm::radians(90.0), 1.0, 0.1, 10.0);
    glm::mat4 viewArr[]={
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,-1,0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(-1,0,0), glm::vec3(0,-1,0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,-1,0), glm::vec3(0,0,-1)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec3(0,-1,0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,-1,0))
    };
    shader* shRenderSixFace=new shader("res/shader/cubemap.vs","res/shader/equirectangular2cubemap.fs");
    shRenderSixFace->use();
    shRenderSixFace->setInt("texHDR", 0);
    shRenderSixFace->setMat4("proj", glm::value_ptr(proj));
  
    cubeTex* cubeRenderSixFace=new cubeTex(texHDR->getTextureID());
    cubeRenderSixFace->init();
    cubeRenderSixFace->setShader(shRenderSixFace);
    //第一个cubemap
    glViewport(0,0,512,512);
    glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D,texHDR->getTextureID());
    for(int i=0;i<6;i++){
        shRenderSixFace->setMat4("view", glm::value_ptr(viewArr[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,st.texID,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        cubeRenderSixFace->draw();
    }
    
    //第二个cubemap
    //irradiance map
    shader* shIrradiance=new shader("res/shader/cubemap.vs","res/shader/irradiance.fs");
    shIrradiance->use();
    shIrradiance->setInt("texEnvMap", 0);
    shIrradiance->setMat4("proj", glm::value_ptr(proj));
  
//    shader* shIrradiance=new shader("res/shader/cubemap.vs","res/shader/equirectangular2cubemap.fs");
//    shIrradiance->use();
//    shIrradiance->setInt("texHDR", 0);
//    shIrradiance->setMat4("proj", glm::value_ptr(proj));

    skybox* cubeIrradiance=new skybox(st.texID,shIrradiance);

    unsigned int irradianceMap;
    unsigned int irrLen=32;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (int i=0;i<6;i++){
       glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB16F,irrLen,irrLen,0,GL_RGB,GL_FLOAT,NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    glBindRenderbuffer(GL_RENDERBUFFER,st.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,irrLen,irrLen);

    glViewport(0,0,irrLen,irrLen);
    for(int i=0;i<6;i++){
        shIrradiance->setMat4("view", glm::value_ptr(viewArr[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,irradianceMap,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        cubeIrradiance->drawSimple();
    }

    //**********************
    //第二个pass
    //以这个cubemap绘制skybox
    //**********************
#ifdef BUILD_MAC
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
    [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
    shader* shBackground=new shader("res/shader/pbr_ibl_background.vs","res/shader/pbr_ibl_background.fs");
    shBackground->use();
    shBackground->setInt("texCubeMap", 0);
    shBackground->setMat4("view", cam->getLookAtMatrix());
    shBackground->setMat4("proj", cam->getPerspectiveMatrix());
    skybox* skyBackground=new skybox(st.texID,shBackground);
    world::getInstance()->addChild(skyBackground);
   
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(g_window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    //很多个球使用pbr贴图渲染
    shader* shPBR=new shader("res/shader/3d_1tex_pbr.vs","res/shader/3d_1tex_pbr_irradiance.fs");
    shPBR->use();
    shPBR->setInt("tex_albedo", 0);
    shPBR->setInt("tex_normal", 1);
    shPBR->setInt("tex_metallic", 2);
    shPBR->setInt("tex_roughness", 3);
    shPBR->setInt("tex_ao", 4);
    shPBR->setInt("tex_irradiance", 5);
    
    texture2* tex0=new texture2("./res/pbr_ball/albedo.png");
    texture2* tex1=new texture2("./res/pbr_ball/normal.png");
    texture2* tex2=new texture2("./res/pbr_ball/metallic.png");
    texture2* tex3=new texture2("./res/pbr_ball/roughness.png");
    texture2* tex4=new texture2("./res/pbr_ball/ao.png");

    tex0->init();
    tex1->init();
    tex2->init();
    tex3->init();
    tex4->init();

    tex0->glInit(GL_TEXTURE0);
    tex1->glInit(GL_TEXTURE1);
    tex2->glInit(GL_TEXTURE2);
    tex3->glInit(GL_TEXTURE3);
    tex4->glInit(GL_TEXTURE4);

    world::getInstance()->setCBBeforeDrawCall([tex0,tex1,tex2,tex3,tex4,irradianceMap](){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex0->getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex1->getTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,tex2->getTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,tex3->getTextureID());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,tex4->getTextureID());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
    });
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            semiSphere* cubeObj=new semiSphere(glm::vec3(300));
            if(!cubeObj->init()){
                flylog("semiSphere init failed!");
                return;
            }
            cubeObj->setPosition(glm::vec3(j*0.3-0.8,i*0.3-0.8,-2));
            cubeObj->setScale(glm::vec3(0.1,0.15,0.2));
            cubeObj->setShader(shPBR);
            world::getInstance()->addChild(cubeObj);
            world::getInstance()->getControl()->bindNode(cubeObj);
        }
    }
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(100,0,0),mtLight,0);
    pointLight* lightObj2=new pointLight(glm::vec3(0,100,0),mtLight,0);
    pointLight* lightObj3=new pointLight(glm::vec3(0,0,100),mtLight,0);
    pointLight* lightObj4=new pointLight(glm::vec3(100,100,20),mtLight,0);
    lightObj1->init();
    lightObj2->init();
    lightObj3->init();
    lightObj4->init();
    
    lightObj1->setPosition(glm::vec3(-0.5,0.5,-0.8));
    lightObj2->setPosition(glm::vec3(0.5,0.5,-0.8));
    lightObj3->setPosition(glm::vec3(-1,-1,0.5));
    lightObj4->setPosition(glm::vec3(0.2,0.2,-0.8));
    
    lightObj1->setScale(glm::vec3(0.2));
    lightObj2->setScale(glm::vec3(0.2));
    lightObj3->setScale(glm::vec3(0.2));
    lightObj4->setScale(glm::vec3(0.2));
 
    world::getInstance()->addPointLight(lightObj1);
    world::getInstance()->addPointLight(lightObj2);
    world::getInstance()->addPointLight(lightObj3);
    world::getInstance()->addPointLight(lightObj4);
   
    controlObj->bindNode(lightObj1);
    controlObj->bindNode(lightObj2);
    controlObj->bindNode(lightObj3);
    controlObj->bindNode(lightObj4);
}


void test_PBR_ibl_diffuse_specular(){
    init_light_direction();
    camera* cam=world::getInstance()->getCamera();
    glm::mat4 proj = cam->getPerspectiveMatrix();
    glm::mat4 projForCap = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 viewArr[] ={
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    
    unsigned int hdrTexture;
    textureHdr* texHDR=new textureHdr("res/pbr_ibl/newport_loft.hdr");
    texHDR->init();
    texHDR->glInit();
    hdrTexture=texHDR->getTextureID();

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.

    shader* shPBR=new shader("res/shader/3d_1tex_pbr.vs","res/shader/3d_1tex_pbr_irradiance_prefilter.fs");
    shader* shBackground=new shader("res/shader/pbr_ibl_background.vs", "res/shader/pbr_ibl_background.fs");
    shader* shRenderSixFace=new shader("res/shader/cubemap.vs", "res/shader/equirectangular2cubemap.fs");
    shader* shIrradiance=new shader("res/shader/cubemap.vs", "res/shader/irradiance.fs");
    shader* shPrefilter=new shader("res/shader/cubemap.vs", "res/shader/pbr_prefilter.fs");
    shader* shBRDF=new shader("res/shader/2d_quad.vs", "res/shader/pbr_brdf.fs");

    shRenderSixFace->use();
    shRenderSixFace->setInt("texHDR", 0);
    shRenderSixFace->setMat4("proj", projForCap);

    shIrradiance->use();
    shIrradiance->setInt("texEnvMap", 0);
    shIrradiance->setMat4("proj", projForCap);

    shPrefilter->use();
    shPrefilter->setInt("texEnvMap", 0);
    shPrefilter->setMat4("proj", proj);
//
    shPBR->use();
    shPBR->setInt("tex_albedo", 0);
    shPBR->setInt("tex_normal", 1);
    shPBR->setInt("tex_metallic", 2);
    shPBR->setInt("tex_roughness", 3);
    shPBR->setInt("tex_ao", 4);
    shPBR->setInt("tex_irradiance", 5);
    shPBR->setInt("tex_prefilter", 6);
 
    shBackground->use();
    shBackground->setInt("texCubeMap", 0);
    shBackground->setMat4("proj", proj);
    shBackground->setMat4("view", cam->getLookAtMatrix());
    
    //把等矩贴图渲染到一张cubemap中
    fboStruct st=fbo::createFBOForIBLWithCubemap();
    cubeTex* cubeRenderSixFace=new cubeTex(texHDR->getTextureID());
    cubeRenderSixFace->init();
    cubeRenderSixFace->setShader(shRenderSixFace);
    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    for (unsigned int i = 0; i < 6; ++i){
        shRenderSixFace->setMat4("view", viewArr[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, st.texID, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeRenderSixFace->draw();
    }
    
    //环境光采样到irradianceMap
     unsigned int irradianceMap;
     glGenTextures(1,&irradianceMap);
     glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
     for (unsigned int i = 0; i < 6; ++i){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
     }
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
     glBindRenderbuffer(GL_RENDERBUFFER, st.rbo);
     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

     skybox* cubeIrradiance=new skybox(st.texID,shIrradiance);
     glViewport(0, 0, 32, 32);
     for (unsigned int i = 0; i < 6; ++i){
         shIrradiance->setMat4("view", viewArr[i]);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         cubeIrradiance->drawSimple();
     }
    
    //环境光采样到prefilterMap
    //从上面的cubemap中采样，经过蒙特卡罗积分
    unsigned int prefilterMap=0;
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i){
       glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    
    //计算这个积分结果 ∫ΩLi(p,ωi)dωi
    skybox* cubePrefilter=new skybox(st.texID,shPrefilter);
    int maxMipLevel=5;
    for(int i=0;i<maxMipLevel;i++){
        int width=(int)128*std::pow(0.5,i);
        int height=(int)128*std::pow(0.5,i);
        glBindRenderbuffer(GL_RENDERBUFFER, st.rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glViewport(0, 0, width, height); // don't forget to configure the viewport to the capture dimensions.
        float roughness = (float)i / (float)(maxMipLevel - 1);
        shPrefilter->setFloat("roughness", roughness);
        //roughness越大，则从mipmap level越低的贴图采样
        for(int j = 0; j < 6; j++){
            shPrefilter->setMat4("view", viewArr[j]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, prefilterMap, i);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cubePrefilter->drawSimple();
        }
    }
    
    //BRDF计算，镜面反射结果进入texBRDFLut纹理中
    unsigned int texBRDFLut;
    glGenTextures(1, &texBRDFLut);
    glBindTexture(GL_TEXTURE_2D, texBRDFLut);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, st.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texBRDFLut, 0);
    glViewport(0, 0, 512, 512);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    quadSize* quadBRDF=new quadSize(1,1,1);
    quadBRDF->setShader(shBRDF);
    quadBRDF->draw();
  
    //display brdf texture
//    quad* quadObj=new quad(texBRDFLut,g_winHigh,g_winHigh);
//    world::getInstance()->addChild(quadObj);
  
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    skybox* skyBackground=new skybox(st.texID,shBackground);
    world::getInstance()->addChild(skyBackground);
    
    glViewport(0, 0, g_winWidth, g_winHigh);

    //很多个球使用pbr贴图渲染
    texture2* tex0=new texture2("./res/pbr_ball/albedo.png");
    texture2* tex1=new texture2("./res/pbr_ball/normal.png");
    texture2* tex2=new texture2("./res/pbr_ball/metallic.png");
    texture2* tex3=new texture2("./res/pbr_ball/roughness.png");
    texture2* tex4=new texture2("./res/pbr_ball/ao.png");

    tex0->init();
    tex1->init();
    tex2->init();
    tex3->init();
    tex4->init();

    tex0->glInit(GL_TEXTURE0);
    tex1->glInit(GL_TEXTURE1);
    tex2->glInit(GL_TEXTURE2);
    tex3->glInit(GL_TEXTURE3);
    tex4->glInit(GL_TEXTURE4);

    world::getInstance()->setCBBeforeDrawCall([tex0,tex1,tex2,tex3,tex4,irradianceMap,prefilterMap](){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex0->getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex1->getTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,tex2->getTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,tex3->getTextureID());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,tex4->getTextureID());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP,prefilterMap);
    });
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            semiSphere* cubeObj=new semiSphere(glm::vec3(300));
            if(!cubeObj->init()){
                flylog("semiSphere init failed!");
                return;
            }
            cubeObj->setPosition(glm::vec3(j*0.3-0.8,i*0.3-0.8,-2));
            cubeObj->setScale(glm::vec3(0.1,0.15,0.2));
            cubeObj->setShader(shPBR);
            world::getInstance()->addChild(cubeObj);
            world::getInstance()->getControl()->bindNode(cubeObj);
        }
    }


    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();

    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(1,0,0),mtLight,0);
    pointLight* lightObj2=new pointLight(glm::vec3(0,1,0),mtLight,0);
    pointLight* lightObj3=new pointLight(glm::vec3(0,0,1),mtLight,0);
    pointLight* lightObj4=new pointLight(glm::vec3(1,1,0.2),mtLight,0);
    lightObj1->init();
    lightObj2->init();
    lightObj3->init();
    lightObj4->init();

    lightObj1->setPosition(glm::vec3(-0.5,0.5,-0.8));
    lightObj2->setPosition(glm::vec3(0.5,0.5,-0.8));
    lightObj3->setPosition(glm::vec3(-1,-1,0.5));
    lightObj4->setPosition(glm::vec3(0.2,0.2,-0.8));

    lightObj1->setScale(glm::vec3(0.2));
    lightObj2->setScale(glm::vec3(0.2));
    lightObj3->setScale(glm::vec3(0.2));
    lightObj4->setScale(glm::vec3(0.2));

    world::getInstance()->addPointLight(lightObj1);
    world::getInstance()->addPointLight(lightObj2);
    world::getInstance()->addPointLight(lightObj3);
    world::getInstance()->addPointLight(lightObj4);

    controlObj->bindNode(lightObj1);
    controlObj->bindNode(lightObj2);
    controlObj->bindNode(lightObj3);
    controlObj->bindNode(lightObj4);
}

void test_PBR_ibl_diffuse_specular_ball2(){
    init_light_direction();
    camera* cam=world::getInstance()->getCamera();
    glm::mat4 proj = cam->getPerspectiveMatrix();
    glm::mat4 projForCap = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 viewArr[] ={
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    
    unsigned int hdrTexture;
    textureHdr* texHDR=new textureHdr("res/pbr_ibl/newport_loft.hdr");
    texHDR->init();
    texHDR->glInit();
    hdrTexture=texHDR->getTextureID();

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.

    shader* shPBR=new shader("res/shader/3d_1tex_pbr.vs","res/shader/3d_1tex_pbr_irradiance_prefilter.fs");
    shader* shBackground=new shader("res/shader/pbr_ibl_background.vs", "res/shader/pbr_ibl_background.fs");
    shader* shRenderSixFace=new shader("res/shader/cubemap.vs", "res/shader/pbr_equirectangular2cubemap.fs");
    shader* shIrradiance=new shader("res/shader/cubemap.vs", "res/shader/pbr_irradiance.fs");
    shader* shPrefilter=new shader("res/shader/cubemap.vs", "res/shader/pbr_prefilter.fs");
    shader* shBRDF=new shader("res/shader/2d_quad.vs", "res/shader/pbr_brdf.fs");

    shRenderSixFace->use();
    shRenderSixFace->setInt("texHDR", 0);
    shRenderSixFace->setMat4("proj", projForCap);

    shIrradiance->use();
    shIrradiance->setInt("texEnvMap", 0);
    shIrradiance->setMat4("proj", projForCap);

    shPrefilter->use();
    shPrefilter->setInt("texEnvMap", 0);
    shPrefilter->setMat4("proj", proj);

    shPBR->use();
    shPBR->setInt("tex_albedo", 0);
    shPBR->setInt("tex_normal", 1);
    shPBR->setInt("tex_metallic", 2);
    shPBR->setInt("tex_roughness", 3);
    shPBR->setInt("tex_ao", 4);
    shPBR->setInt("tex_irradiance", 5);
    shPBR->setInt("tex_prefilter", 6);
    shPBR->setInt("tex_brdf", 7);
 
    shBackground->use();
    shBackground->setInt("texCubeMap", 0);
    shBackground->setMat4("proj", proj);
    shBackground->setMat4("view", cam->getLookAtMatrix());
    
    //把等矩贴图渲染到一张cubemap中
    fboStruct st=fbo::createFBOForIBLWithCubemap();
    cubeTex* cubeRenderSixFace=new cubeTex(texHDR->getTextureID());
    cubeRenderSixFace->init();
    cubeRenderSixFace->setShader(shRenderSixFace);
    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    for(unsigned int i=0;i<6;++i){
        shRenderSixFace->setMat4("view",viewArr[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,st.texID,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        cubeRenderSixFace->draw();
    }
    
    //环境光采样到另一个cubemap: irradianceMap
    unsigned int irradianceMap;
    glGenTextures(1,&irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
    for(unsigned int i=0;i<6;++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB16F,32,32,0,GL_RGB,GL_FLOAT,nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    glBindRenderbuffer(GL_RENDERBUFFER,st.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,32,32);
    skybox* skyIrradiance=new skybox(st.texID,shIrradiance);
    glViewport(0,0,32,32);
    for(unsigned int i=0;i<6;++i){
        shIrradiance->setMat4("view",viewArr[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,irradianceMap,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        skyIrradiance->drawSimple();
    }
    
    //环境光采样到prefilterMap
    //从上面的cubemap中采样，经过蒙特卡罗积分，输出到cubemap: prefilterMap
    unsigned int prefilterMap=0;
    glGenTextures(1,&prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP,prefilterMap);
    for(unsigned int i=0;i<6;++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB16F,128,128,0,GL_RGB,GL_FLOAT,nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    skybox* skyPrefilter=new skybox(st.texID,shPrefilter);//计算这个积分结果 ∫ΩLi(p,ωi)dωi
    int maxMipLevel=5;
    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    for(int i=0;i<maxMipLevel;i++){
        int width=(int)128*std::pow(0.5,i);
        int height=(int)128*std::pow(0.5,i);
        glBindRenderbuffer(GL_RENDERBUFFER,st.rbo);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,width,height);
        glViewport(0,0,width,height); //don't forget to configure the viewport to the capture dimensions.
        float roughness=(float)i/(float)(maxMipLevel-1);
        shPrefilter->setFloat("roughness",roughness); //roughness越大，则从mipmap level越低的贴图采样
        for(int j=0;j<6;j++){
            shPrefilter->setMat4("view",viewArr[j]);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+j,prefilterMap,i);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            skyPrefilter->drawSimple();
        }
    }
    
    //BRDF计算，结果进入texBRDFLut纹理中
    unsigned int texBRDFLut;
    glGenTextures(1,&texBRDFLut);
    glBindTexture(GL_TEXTURE_2D,texBRDFLut);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RG16F,512,512,0,GL_RG,GL_FLOAT,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    glBindRenderbuffer(GL_RENDERBUFFER,st.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,512,512);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texBRDFLut,0);
    glViewport(0,0,512,512);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    quadSize* quadBRDF=new quadSize(1,1,1);
    quadBRDF->setShader(shBRDF);
    quadBRDF->draw();
  
    //display brdf texture
//    quad* quadObj=new quad(texBRDFLut,g_winHigh,g_winHigh);
//    world::getInstance()->addChild(quadObj);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    skybox* skyBackground=new skybox(st.texID,shBackground);
    world::getInstance()->addChild(skyBackground);
    glViewport(0,0,g_winWidth,g_winHigh);

    //很多个球使用pbr贴图渲染
    texture2* tex0=new texture2("./res/pbr_ball2/Crusted_snow2_albedo.png");
    texture2* tex1=new texture2("./res/pbr_ball2/Crusted_snow2_normal-ogl.png");
    texture2* tex2=new texture2("./res/pbr_ball2/Crusted_snow2_metallic.png");
    texture2* tex3=new texture2("./res/pbr_ball2/Crusted_snow2_roughness.png");
    texture2* tex4=new texture2("./res/pbr_ball2/Crusted_snow2_ao.png");
    tex0->init();
    tex1->init();
    tex2->init();
    tex3->init();
    tex4->init();
    tex0->glInit(GL_TEXTURE0);
    tex1->glInit(GL_TEXTURE1);
    tex2->glInit(GL_TEXTURE2);
    tex3->glInit(GL_TEXTURE3);
    tex4->glInit(GL_TEXTURE4);

    world::getInstance()->setCBBeforeDrawCall([tex0,tex1,tex2,tex3,tex4,irradianceMap,prefilterMap,texBRDFLut](){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex0->getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex1->getTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,tex2->getTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,tex3->getTextureID());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,tex4->getTextureID());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP,prefilterMap);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_CUBE_MAP,texBRDFLut);
    });
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            semiSphere* cubeObj=new semiSphere(glm::vec3(300));
            if(!cubeObj->init()){
                flylog("semiSphere init failed!");
                return;
            }
            cubeObj->setPosition(glm::vec3(j*0.3-0.8,i*0.3-0.8,-2));
            cubeObj->setScale(glm::vec3(0.1,0.15,0.2));
            cubeObj->setShader(shPBR);
            world::getInstance()->addChild(cubeObj);
            world::getInstance()->getControl()->bindNode(cubeObj);
        }
    }

    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();

    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(100,100,100),mtLight,0);
    pointLight* lightObj2=new pointLight(glm::vec3(1,1,0),mtLight,0);
    pointLight* lightObj3=new pointLight(glm::vec3(1,1,10),mtLight,0);
    pointLight* lightObj4=new pointLight(glm::vec3(1,1,0.2),mtLight,0);
    lightObj1->init();
    lightObj2->init();
    lightObj3->init();
    lightObj4->init();

    lightObj1->setPosition(glm::vec3(-0.5,0.5,-0.8));
    lightObj2->setPosition(glm::vec3(0.5,0.5,-0.8));
    lightObj3->setPosition(glm::vec3(-1,-1,0.5));
    lightObj4->setPosition(glm::vec3(0.2,0.2,-0.8));

    lightObj1->setScale(glm::vec3(0.2));
    lightObj2->setScale(glm::vec3(0.2));
    lightObj3->setScale(glm::vec3(0.2));
    lightObj4->setScale(glm::vec3(0.2));

    world::getInstance()->addPointLight(lightObj1);
    world::getInstance()->addPointLight(lightObj2);
    world::getInstance()->addPointLight(lightObj3);
    world::getInstance()->addPointLight(lightObj4);

    controlObj->bindNode(lightObj1);
    controlObj->bindNode(lightObj2);
    controlObj->bindNode(lightObj3);
    controlObj->bindNode(lightObj4);
}


void test_PBR_ibl_diffuse_model_fbx_gun(){
    init_light_direction();
    camera* cam=world::getInstance()->getCamera();
    glm::mat4 proj = cam->getPerspectiveMatrix();
    glm::mat4 projForCap = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 viewArr[] ={
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    
    unsigned int hdrTexture;
    textureHdr* texHDR=new textureHdr("res/pbr_ibl/newport_loft.hdr");
    texHDR->init();
    texHDR->glInit();
    hdrTexture=texHDR->getTextureID();

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.

    shader* shPBR=new shader("res/shader/3d_1tex_pbr.vs","res/shader/3d_1tex_pbr_irradiance_prefilter.fs");
//    shader* shPBR=new shader("res/shader/3d_1tex_pbr.vs","res/shader/3d_1tex_pbr.fs");
    shader* shBackground=new shader("res/shader/pbr_ibl_background.vs", "res/shader/pbr_ibl_background.fs");
    shader* shRenderSixFace=new shader("res/shader/cubemap.vs", "res/shader/equirectangular2cubemap.fs");
    shader* shIrradiance=new shader("res/shader/cubemap.vs", "res/shader/irradiance.fs");
    shader* shPrefilter=new shader("res/shader/cubemap.vs", "res/shader/pbr_prefilter.fs");
    shader* shBRDF=new shader("res/shader/2d_quad.vs", "res/shader/pbr_brdf.fs");

    shRenderSixFace->use();
    shRenderSixFace->setInt("texHDR", 0);
    shRenderSixFace->setMat4("proj", projForCap);

    shIrradiance->use();
    shIrradiance->setInt("texEnvMap", 0);
    shIrradiance->setMat4("proj", projForCap);

    shPrefilter->use();
    shPrefilter->setInt("texEnvMap", 0);
    shPrefilter->setMat4("proj", proj);
//
    shPBR->use();
    shPBR->setInt("tex_albedo", 0);
    shPBR->setInt("tex_normal", 1);
    shPBR->setInt("tex_metallic", 2);
    shPBR->setInt("tex_roughness", 3);
    shPBR->setInt("tex_ao", 4);
    shPBR->setInt("tex_irradiance", 5);
    shPBR->setInt("tex_prefilter", 6);
 
    shBackground->use();
    shBackground->setInt("texCubeMap", 0);
    shBackground->setMat4("proj", proj);
    shBackground->setMat4("view", cam->getLookAtMatrix());
    
    //把等矩贴图渲染到一张cubemap中
    fboStruct st=fbo::createFBOForIBLWithCubemap();
    cubeTex* cubeRenderSixFace=new cubeTex(texHDR->getTextureID());
    cubeRenderSixFace->init();
    cubeRenderSixFace->setShader(shRenderSixFace);
    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    for (unsigned int i = 0; i < 6; ++i){
        shRenderSixFace->setMat4("view", viewArr[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, st.texID, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeRenderSixFace->draw();
    }
    
    //环境光采样到irradianceMap
     unsigned int irradianceMap;
     glGenTextures(1,&irradianceMap);
     glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
     for (unsigned int i = 0; i < 6; ++i){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
     }
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
     glBindRenderbuffer(GL_RENDERBUFFER, st.rbo);
     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

     skybox* cubeIrradiance=new skybox(st.texID,shIrradiance);
     glViewport(0, 0, 32, 32);
     for (unsigned int i = 0; i < 6; ++i){
         shIrradiance->setMat4("view", viewArr[i]);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         cubeIrradiance->drawSimple();
     }
    
    //环境光采样到prefilterMap
    //从上面的cubemap中采样，经过蒙特卡罗积分
    unsigned int prefilterMap=0;
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i){
       glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    
    //计算这个积分结果 ∫ΩLi(p,ωi)dωi
    skybox* cubePrefilter=new skybox(st.texID,shPrefilter);
    int maxMipLevel=5;
    for(int i=0;i<maxMipLevel;i++){
        int width=(int)128*std::pow(0.5,i);
        int height=(int)128*std::pow(0.5,i);
        glBindRenderbuffer(GL_RENDERBUFFER, st.rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glViewport(0, 0, width, height); // don't forget to configure the viewport to the capture dimensions.
        float roughness = (float)i / (float)(maxMipLevel - 1);
        shPrefilter->setFloat("roughness", roughness);
        //roughness越大，则从mipmap level越低的贴图采样
        for(int j = 0; j < 6; j++){
            shPrefilter->setMat4("view", viewArr[j]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, prefilterMap, i);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cubePrefilter->drawSimple();
        }
    }
    
    //BRDF计算，镜面反射结果进入texBRDFLut纹理中
    unsigned int texBRDFLut;
    glGenTextures(1, &texBRDFLut);
    glBindTexture(GL_TEXTURE_2D, texBRDFLut);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, st.fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, st.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texBRDFLut, 0);
    glViewport(0, 0, 512, 512);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    quadSize* quadBRDF=new quadSize(1,1,1);
    quadBRDF->setShader(shBRDF);
    quadBRDF->draw();
  
    //display brdf texture
//    quad* quadObj=new quad(texBRDFLut,g_winHigh,g_winHigh);
//    world::getInstance()->addChild(quadObj);
  
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    skybox* skyBackground=new skybox(st.texID,shBackground);
    world::getInstance()->addChild(skyBackground);
    
    glViewport(0, 0, g_winWidth, g_winHigh);

    //很多个球使用pbr贴图渲染
    texture2* tex0=new texture2("./res/model/Cerberus/Textures/Cerberus_A.tga");
    texture2* tex1=new texture2("./res/model/Cerberus/Textures/Cerberus_N.tga");
    texture2* tex2=new texture2("./res/model/Cerberus/Textures/Cerberus_M.tga");
    texture2* tex3=new texture2("./res/model/Cerberus/Textures/Cerberus_R.tga");
    texture2* tex4=new texture2("./res/model/Cerberus/Textures/Raw/Cerberus_AO.tga");
 
//    texture2* tex1=new texture2("./res/pbr_ball2/Crusted_snow2_normal-ogl.png");
//    texture2* tex2=new texture2("./res/pbr_ball2/Crusted_snow2_metallic.png");
//    texture2* tex3=new texture2("./res/pbr_ball2/Crusted_snow2_roughness.png");
//    texture2* tex4=new texture2("./res/pbr_ball2/Crusted_snow2_ao.png");

    tex0->init();
    tex1->init();
    tex2->init();
    tex3->init();
    tex4->init();

    tex0->glInit(GL_TEXTURE0);
    tex1->glInit(GL_TEXTURE1);
    tex2->glInit(GL_TEXTURE2);
    tex3->glInit(GL_TEXTURE3);
    tex4->glInit(GL_TEXTURE4);


   
    model* modelObj=new model("res/model/Cerberus/Cerberus_LP.FBX");
    modelObj->setPosition(glm::vec3(0,0,-20));
    modelObj->setScale(0.1);
    modelObj->setRotation(glm::vec3(90,90,270));
    if(!modelObj->init()){
       flylog("modelObj init failed!");
       return;
    }
    modelObj->setShader(shPBR);
//    world::getInstance()->addChild(modelObj);
    world::getInstance()->getControl()->bindNode(modelObj);

    world::getInstance()->setCBBeforeDrawCall([modelObj,tex0,tex1,tex2,tex3,tex4,irradianceMap,prefilterMap](){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex0->getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex1->getTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,tex2->getTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,tex3->getTextureID());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,tex4->getTextureID());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP,prefilterMap);
        modelObj->drawSimple();
    });
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();

    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(100,100,100),mtLight,0);
    pointLight* lightObj2=new pointLight(glm::vec3(1,1,0),mtLight,0);
    pointLight* lightObj3=new pointLight(glm::vec3(1,1,10),mtLight,0);
    pointLight* lightObj4=new pointLight(glm::vec3(1,1,0.2),mtLight,0);
    lightObj1->init();
    lightObj2->init();
    lightObj3->init();
    lightObj4->init();

    lightObj1->setPosition(glm::vec3(-0.5,0.5,-0.8));
    lightObj2->setPosition(glm::vec3(0.5,0.5,-0.8));
    lightObj3->setPosition(glm::vec3(-1,-1,0.5));
    lightObj4->setPosition(glm::vec3(0.2,0.2,-0.8));

    lightObj1->setScale(glm::vec3(0.2));
    lightObj2->setScale(glm::vec3(0.2));
    lightObj3->setScale(glm::vec3(0.2));
    lightObj4->setScale(glm::vec3(0.2));

    world::getInstance()->addPointLight(lightObj1);
    world::getInstance()->addPointLight(lightObj2);
    world::getInstance()->addPointLight(lightObj3);
    world::getInstance()->addPointLight(lightObj4);

    controlObj->bindNode(lightObj1);
    controlObj->bindNode(lightObj2);
    controlObj->bindNode(lightObj3);
    controlObj->bindNode(lightObj4);
}
