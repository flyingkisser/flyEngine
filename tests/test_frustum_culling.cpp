//
//  test_frustum_culling.cpp
//  flyEngine
//
//  Created by joe on 03/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#include "test_frustum_culling.h"
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




void test_frustum_culling_sphere(){
    camera* cam=world::getInstance()->getCamera();
    glm::vec3 camPos=glm::vec3(-1.8,1.8,9.5);
    cam->setPosition(camPos);
    cam->setYaw(-83.03);
    cam->setPitch(-9);
    
    float far=cam->getFarPlane();
    float near=cam->getNearPlane();
    
    cubeTex* floorObj=new cubeTex("./res/floor.png");
   
    int descArr[]={3,2,3};
    floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane, sizeof(g_verticeArrWithTexCoordAndNormal_plane), descArr, 3);
    floorObj->setScale(5.0f);
    world::getInstance()->addChild(floorObj);
    world::getInstance()->getControl()->bindNode(floorObj);
    
    std::vector<cubeTex*> objArr;
    int countX=10; int countY=countX;
    int c=0;
    for(int i=0;i<countX;i++){
        for(int j=0;j<countY;j++){
            cubeTex* cubeObj=new cubeTex("./res/model/rock/rock.png");
            cubeObj->init();
            float x=-1.0f+(i+1)*2/(float)countX;
            float y=0.5;
            float z=-1.0f+(j+1)*2/(float)countY;
            cubeObj->setPosition(glm::vec3(x,0.5,z));
            cubeObj->setScale(0.1);
            cubeObj->setCutOffBySphere(true);
            world::getInstance()->addChild(cubeObj);
            world::getInstance()->getControl()->bindNode(cubeObj);
            flylog("add %d rock %f %f %f",c++,x,y,z);
            objArr.push_back(cubeObj);
        }
    }
    
    fboStruct stDepth=fbo::createFBOForDepthWithCubemap();

    material2* mtLight=createMaterial(1, 1, 1, 1);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0.2,0.8,-0.2));
   
    plight->setScale(0.02);
    world::getInstance()->addPointLight(plight);
    
    glm::vec3 lightViewPos=plight->getPosition();
    cam->updateUBOForShadow(lightViewPos);
  
    shader* shNormal=floorObj->getShader();
    shader* shDepth=new shader("./res/shader/depth_cubetex_shadow.vs","./res/shader/depth_cubetex_shadow.fs","./res/shader/depth_cubetex_shadow.gs");
    glm::mat4 matProj=glm::perspective(glm::radians(90.0f),(float)g_shadowWidth/(float)g_shadowHigh,1.0f,25.0f);
    std::vector<glm::mat4> vecMat;
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(1,0,0), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(-1,0,0), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,1,0), glm::vec3(0,0,1)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,-1,0), glm::vec3(0,0,-1)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,1), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,-1), glm::vec3(0,-1,0)));
 
    
    shDepth->use();
    char szName[128]={0};
    for(int i=0;i<6;i++){
        snprintf(szName, sizeof(szName), "matShadowCube[%d]",i);
        shDepth->setMat4(szName, glm::value_ptr(vecMat[i]),true);
    }
    shDepth->setFloat("far_plane", far,true);
    shDepth->setVec3("posLight", glm::value_ptr(lightViewPos),true);
   
    shNormal->use();
    shNormal->setBool("bCheckShadowByCubemap",true,true);
    shNormal->setFloat("far_plane", far,true);
    glActiveTexture(GL_TEXTURE0+4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, stDepth.texID);
    shNormal->setInt("texture_depth_cube",texture_depth_cube,true);
    shNormal->setBool("bUseBlinnPhong", true,true);
   

    world::getInstance()->addPass(stDepth.fbo, shDepth,[](){
//        glViewport(0,0,g_shadowWidth,g_shadowHigh);
    });
    
    world::getInstance()->addPass(0, shNormal, [](){
//        glDisable(GL_CULL_FACE);
//        glViewport(0,0,g_winWidth,g_winHigh);
    });

    world::getInstance()->getControl()->regOnKeyPress('p', [cam,objArr](){
        objArr[0]->print();
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
}



void test_frustum_culling_aabb(){
    camera* cam=world::getInstance()->getCamera();
    glm::vec3 camPos=glm::vec3(-1.8,1.8,9.5);
    cam->setPosition(camPos);
    cam->setYaw(-83.03);
    cam->setPitch(-9);

    float far=cam->getFarPlane();
    float near=cam->getNearPlane();

    cubeTex* floorObj=new cubeTex("./res/floor.png");
   
    int descArr[]={3,2,3};
    floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormal_plane, sizeof(g_verticeArrWithTexCoordAndNormal_plane), descArr, 3);
    floorObj->setScale(5.0f);
    world::getInstance()->addChild(floorObj);
    world::getInstance()->getControl()->bindNode(floorObj);
    
    std::vector<cubeTex*> objArr;
    int countX=15; int countY=countX;
    float len=5.0f;
    int c=0;
    for(int i=0;i<countX;i++){
        for(int j=0;j<countY;j++){
            cubeTex* cubeObj=new cubeTex("./res/model/rock/rock.png");
            cubeObj->init();
            float x=-len*0.5f+(i+1)*len/(float)countX;
            float y=0.3;
            float z=-len*0.5f+(j+1)*len/(float)countY;
            cubeObj->setPosition(glm::vec3(x,y,z));
            cubeObj->setScale(0.1);
            cubeObj->setCutOffByAABB(true);
            world::getInstance()->addChild(cubeObj);
            world::getInstance()->getControl()->bindNode(cubeObj);
            flylog("add %d rock %f %f %f",c++,x,y,z);
            objArr.push_back(cubeObj);
        }
    }
    
    fboStruct stDepth=fbo::createFBOForDepthWithCubemap();

    material2* mtLight=createMaterial(1, 1, 1, 1);
    pointLight* plight=new pointLight(glm::vec3(1,0,0),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0.2,0.5,-20));
//    plight->setScale(0.1);
    world::getInstance()->addPointLight(plight);
    
    pointLight* plight2=new pointLight(glm::vec3(0,1,0),mtLight,0);
    if(!plight2->init()){
       flylog("point plight2 init failed!");
       return;
    }
    plight2->setPosition(glm::vec3(0.2,0.3,-10));
//    plight2->setScale(1);
    world::getInstance()->addPointLight(plight2);
    
    glm::vec3 lightViewPos=plight->getPosition();
    cam->updateUBOForShadow(lightViewPos);
  
    shader* shNormal=floorObj->getShader();
    shader* shDepth=new shader("./res/shader/depth_cubetex_shadow.vs","./res/shader/depth_cubetex_shadow.fs","./res/shader/depth_cubetex_shadow.gs");
    glm::mat4 matProj=glm::perspective(glm::radians(90.0f),(float)g_shadowWidth/(float)g_shadowHigh,1.0f,25.0f);
    std::vector<glm::mat4> vecMat;
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(1,0,0), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(-1,0,0), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,1,0), glm::vec3(0,0,1)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,-1,0), glm::vec3(0,0,-1)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,1), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,-1), glm::vec3(0,-1,0)));
 
    
    shDepth->use();
    char szName[128]={0};
    for(int i=0;i<6;i++){
        snprintf(szName, sizeof(szName), "matShadowCube[%d]",i);
        shDepth->setMat4(szName, glm::value_ptr(vecMat[i]),true);
    }
    shDepth->setFloat("far_plane", far,true);
    shDepth->setVec3("posLight", glm::value_ptr(lightViewPos),true);
   
    shNormal->use();
    shNormal->setBool("bCheckShadowByCubemap",true,true);
    shNormal->setFloat("far_plane", far,true);
    glActiveTexture(GL_TEXTURE0+4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, stDepth.texID);
    shNormal->setInt("texture_depth_cube",texture_depth_cube,true);
    shNormal->setBool("bUseBlinnPhong", true,true);
   

    world::getInstance()->addPass(stDepth.fbo, shDepth,[](){
//        glViewport(0,0,g_shadowWidth,g_shadowHigh);
    });
    
    world::getInstance()->addPass(0, shNormal, [](){
        glDisable(GL_CULL_FACE);
//        glViewport(0,0,g_winWidth,g_winHigh);
    });

    world::getInstance()->getControl()->regOnKeyPress('p', [objArr](){
        objArr[0]->print();
    });
 
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
}
