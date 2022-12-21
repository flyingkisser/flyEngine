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
#include "semiSphere.h"
#include "material2.h"
#include "texture2.h"
#include "shader.h"
#include "shaderMgr.h"
#include "randUtil.h"
#include "threadUtil.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"
#include "glslUtil.h"
#include "values.h"
#include "camera.h"
#include "model.h"
#include "fbo.h"

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


void test_PBR_cubeTex_2(){
    init_light_direction();
    shader* shPBR=new shader("res/shader/3d_1tex_pbr_2.vs","res/shader/3d_1tex_pbr_2.fs");
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
}

//IBL: image based lighting
void test_PBR_cubeTex_ibl_1(){
    init_light_direction();
    shader* shPBR=new shader("res/shader/3d_1tex_pbr_ibl_1.vs","res/shader/3d_1tex_pbr_ibl_1.fs");
    shPBR->use();
    shPBR->setInt("tex_albedo", 0);
    shPBR->setInt("tex_normal", 1);
    shPBR->setInt("tex_metallic", 2);
    shPBR->setInt("tex_roughness", 3);
    shPBR->setInt("tex_ao", 4);
    
    texture2* tex0=new texture2("./res/pbr_ibl/newport_loft.hdr");
//    texture2* tex1=new texture2("./res/pbr_ibl/normal.png");
//    texture2* tex2=new texture2("./res/pbr_ibl/metallic.png");
//    texture2* tex3=new texture2("./res/pbr_ibl/roughness.png");
//    texture2* tex4=new texture2("./res/pbr_ibl/ao.png");
    
    tex0->init();
//    tex1->init();
//    tex2->init();
//    tex3->init();
//    tex4->init();
    
    tex0->glInit(GL_TEXTURE0);
//    tex1->glInit(GL_TEXTURE1);
//    tex2->glInit(GL_TEXTURE2);
//    tex3->glInit(GL_TEXTURE3);
//    tex4->glInit(GL_TEXTURE4);
    
    cubeTex* cubeObj=new cubeTex("");
    shader* shCubeMap=new shader("res/shader/3d_1tex_ibl_cubemap.vs","res/shader/3d_1tex_ibl_cubemap.fs");
    shCubeMap->use();
    shCubeMap->setInt("envMap", 0);
    fboStruct st=fbo::createFBOForIBLWithCubemap();
    
    world::getInstance()->setCBAfterAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });
    
    glm::mat4 proj=glm::perspective(glm::radians(90.0), 1.0, 0.1, 10.0);
    glm::mat4 viewArr[]={
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,-1,0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(-1,0,0), glm::vec3(0,-1,0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,-1,0), glm::vec3(0,0,-1)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec3(0,-1,0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,-1,0))
    }
    
    world::getInstance()->setCBBeforeDrawCall([st](){
        
    });
    
   

    //普通cube
//    for(int i=0;i<7;i++){
//        for(int j=0;j<7;j++){
//            semiSphere* cubeObj=new semiSphere(glm::vec3(300));
//            if(!cubeObj->init()){
//                flylog("semiSphere init failed!");
//                return;
//            }
//            cubeObj->setPosition(glm::vec3(j*0.3-0.8,i*0.3-0.8,-2));
//            cubeObj->setScale(glm::vec3(0.1,0.15,0.2));
//            cubeObj->setShader(shPBR);
//            world::getInstance()->addChild(cubeObj);
//            world::getInstance()->getControl()->bindNode(cubeObj);
//        }
//    }
  
    
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
}
