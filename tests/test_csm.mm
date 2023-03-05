//
//  test_csm.cpp
//  flyEngine
//
//  Created by Joe on 2023/2/12.
//  Copyright © 2023 joe. All rights reserved.
//

#include "test_csm.h"
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

std::vector<glm::vec4> getFrustrumCornersWorldSpace(glm::mat4 proj,glm::mat4 view){
    glm::mat4 trans=glm::inverse(proj*view);
    std::vector<glm::vec4> vectorPos;
    for(int x=0;x<2;x++){
        for(int y=0;y<2;y++){
            for(int z=0;z<2;z++){
                glm::vec4 pos=trans*glm::vec4(2*x-1,2*y-1,2*z-1,1);
                vectorPos.push_back(pos/pos.w);
            }
        }
    }
    return vectorPos;
}

glm::vec3 getFrustrumCenter(std::vector<glm::vec4> corners){
    glm::vec3 pos=glm::vec3(0,0,0);
    for(auto& v:corners){
        pos+=glm::vec3(v);
    }
    pos/=corners.size();
    return pos;
}

glm::mat4 getLightSpaceMatrix(glm::vec3 lightPos,float nearPlane,float farPlane){
    camera* cam=world::getInstance()->getCamera();
    glm::mat4 matProjPerspective=glm::perspective(glm::radians(double(cam->getFov())), (double)cam->getScreenRatio(), (double)nearPlane, (double)farPlane);
    std::vector<glm::vec4> corners=getFrustrumCornersWorldSpace(matProjPerspective, cam->getLookAtMatrix());
    glm::vec3 center=getFrustrumCenter(corners);
    glm::mat4 lightView=glm::lookAt(center+lightPos, center, glm::vec3(0,1,0));
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = minX;
    float maxY = maxX;
    float minZ = minX;
    float maxZ = maxX;
    for (auto& v:corners){
        auto trf=lightView*v;
        minX=std::min(minX,trf.x);
        maxX=std::max(maxX,trf.x);
        minY=std::min(minY,trf.y);
        maxY=std::max(maxY,trf.y);
        minZ=std::min(minZ,trf.z);
        maxZ=std::max(maxZ,trf.z);
    }
    float zMult=10.0;
    if(minZ<0)
        minZ*=zMult;
    else
        minZ/=zMult;
    if(maxZ<0)
        maxZ/=zMult;
    else
        maxZ*=zMult;
    glm::mat4 lightProj=glm::ortho(minX,maxX,minY,maxY,minZ,maxZ);
    return lightProj*lightView;
}


void test_csm_old(){
    camera* cam=world::getInstance()->getCamera();
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
    
    fboStruct stDepth=fbo::createFBOForDepthWithCubemap();

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
    cam->updateUBOForShadow(lightViewPos);
  
    shader* shNormal=huouseObj->getShader();
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
    shDepth->setFloat("far_plane", 25.0f,true);
    shDepth->setVec3("posLight", glm::value_ptr(lightViewPos),true);
   
    shNormal->use();
    shNormal->setBool("bCheckShadowByCubemap",true,true);
    shNormal->setFloat("far_plane", 25.0f,true);
    glActiveTexture(GL_TEXTURE0+4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, stDepth.texID);
    shNormal->setInt("texture_depth_cube",texture_depth_cube,true);
//    shNormal->setBool("g_debug", true,true);
    shNormal->setBool("bUseBlinnPhong", true,true);
    world::getInstance()->getControl()->bindNode(huouseObj);

    world::getInstance()->addPass(stDepth.fbo, shDepth, [](){
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
    });
    world::getInstance()->addPass(0, shNormal, [](){
        glDisable(GL_CULL_FACE);
        glViewport(0,0,g_winWidth,g_winHigh);
    });
    
//    huouseObj->setCBBeforeDrawCall([](int progrmaID){
//        glDisable(GL_CULL_FACE);
//    });
    
//    cubeObj1->setCBBeforeDrawCall([](int progrmaID){
//        glEnable(GL_CULL_FACE);
//    });
    
    world::getInstance()->setCBBeforeAnyGLCall([stDepth,shDepth,plight,cam,matProj](){
        std::vector<glm::mat4> vecMat;
        glm::vec3 lightViewPos=plight->getPosition();
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(1,0,0), glm::vec3(0,-1,0)));
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(-1,0,0), glm::vec3(0,-1,0)));
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,1,0), glm::vec3(0,0,1)));
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,-1,0), glm::vec3(0,0,-1)));
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,1), glm::vec3(0,-1,0)));
        vecMat.push_back(matProj*glm::lookAt(lightViewPos, lightViewPos+glm::vec3(0,0,-1), glm::vec3(0,-1,0)));
        char szName[128]={0};
        for(int i=0;i<6;i++){
            snprintf(szName, sizeof(szName), "matShadowCube[%d]",i);
            shDepth->setMat4(szName, glm::value_ptr(vecMat[i]));
        }
    });

 
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
    
}

void test_csm_old2(){
    float farPlane=500.0f;
    float nearPlane=0.1f;
    camera* cam=world::getInstance()->getCamera();
    cam->setFarPlane(farPlane);
    material2* mtLight=createMaterial(1, 1, 1, 1);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
        flylog("point light init failed!");
        return;
    }
    plight->setPosition(glm::vec3(-2.1,0,0.2));
    plight->setScale(0.02);
    world::getInstance()->addPointLight(plight);
    glm::vec3 lightPos=plight->getPosition();

#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif

    //house
    cubeTex* floorObj=new cubeTex("./res/floor.png");
    int descArr[]={3,2,3};
    floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormalReverse, sizeof(g_verticeArrWithTexCoordAndNormalReverse), descArr, 3);
    floorObj->setScale(5.0f);
//    world::getInstance()->addChild(floorObj);
     world::getInstance()->getControl()->bindNode(floorObj);
    
    cubeTex* cubeObj1=new cubeTex("./res/wood.png");
    cubeObj1->init();
    cubeObj1->setPosition(glm::vec3(0.1,0.1,0));
    cubeObj1->setScale(0.4);
//    world::getInstance()->addChild(cubeObj1);
    
    cubeTex* cubeObj2=new cubeTex("./res/wood.png");
    cubeObj2->init();
    cubeObj2->setPosition(glm::vec3(0.2,0.2,0.1));
    cubeObj2->setScale(0.2);
//    world::getInstance()->addChild(cubeObj2);
    
    cubeTex* cubeObj3=new cubeTex("./res/wood.png");
    cubeObj3->init();
    cubeObj3->setPosition(glm::vec3(-0.5,-0.5,0));
    cubeObj3->setScale(0.22);
//    world::getInstance()->addChild(cubeObj3);
    
    cubeTex* cubeObj4=new cubeTex("./res/wood.png");
    cubeObj4->init();
    cubeObj4->setPosition(glm::vec3(-0.7,0.5,0.2));
    cubeObj4->setScale(0.3);
//    world::getInstance()->addChild(cubeObj4);
    
    cubeTex* cubeObj5=new cubeTex("./res/wood.png");
    cubeObj5->init();
    cubeObj5->setPosition(glm::vec3(-1,1,0.3));
    cubeObj5->rotateBy(glm::vec3(60,0,60));
    cubeObj5->setScale(0.15);
//    world::getInstance()->addChild(cubeObj5);
    
    glm::vec3 camPos=glm::vec3(-2.2,0.1,0.3);
    cam->setPosition(camPos);
    cam->setYaw(-9.03);
    cam->setPitch(4.78);
//    cam->updateUBOForShadow(lightViewPos);
    
    
//    shader* shDepth=new shader("./res/shader/depth_cubetex_shadow.vs","./res/shader/draw_null.fs","./res/shader/depth_cubetex_shadow_csm.gs");
//    shader* shCSM=cubeObj5->getShader();
    
    fboStruct st=fbo::createFBOForDepthWithCubemap();
    shader* shCSM=floorObj->getShader();
    shader* shDepth=new shader("./res/shader/depth_cubetex_shadow.vs","./res/shader/depth_cubetex_shadow.fs","./res/shader/depth_cubetex_shadow.gs");
    glm::mat4 matProj=glm::perspective(glm::radians(90.0f),(float)g_shadowWidth/(float)g_shadowHigh,1.0f,25.0f);
    std::vector<glm::mat4> vecMat;
    vecMat.push_back(matProj*glm::lookAt(lightPos, lightPos+glm::vec3(1,0,0), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightPos, lightPos+glm::vec3(-1,0,0), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightPos, lightPos+glm::vec3(0,1,0), glm::vec3(0,0,1)));
    vecMat.push_back(matProj*glm::lookAt(lightPos, lightPos+glm::vec3(0,-1,0), glm::vec3(0,0,-1)));
    vecMat.push_back(matProj*glm::lookAt(lightPos, lightPos+glm::vec3(0,0,1), glm::vec3(0,-1,0)));
    vecMat.push_back(matProj*glm::lookAt(lightPos, lightPos+glm::vec3(0,0,-1), glm::vec3(0,-1,0)));
    shDepth->use();
    char szName[128]={0};
    for(int i=0;i<6;i++){
        snprintf(szName, sizeof(szName), "matShadowCube[%d]",i);
        shDepth->setMat4(szName, glm::value_ptr(vecMat[i]),true);
    }
    shDepth->setFloat("far_plane", farPlane,true);
    shDepth->setVec3("posLight", glm::value_ptr(lightPos),true);
   
    shCSM->use();
    shCSM->setBool("bCheckShadowByCubemap",true,true);
    shCSM->setFloat("far_plane", farPlane,true);
    glActiveTexture(GL_TEXTURE0+4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, st.texID);
    shCSM->setInt("texture_depth_cube",texture_depth_cube,true);
//    shNormal->setBool("g_debug", true,true);
    shCSM->setBool("bUseBlinnPhong", true,true);
    
    
    
    shCSM->use();
    shCSM->setInt("texture_shadow_array",3,true);
    shCSM->setBool("gamma_correct", false,true);
    shCSM->setBool("bUseBlinnPhong", true,true);
    
//    fboStruct st=fbo::createFBOForDepthWithCubemap();

//    world::getInstance()->addPass(stDepth.fbo, shDepth, [](){
//        glViewport(0,0,g_shadowWidth,g_shadowHigh);
//    });
//    world::getInstance()->addPass(0, shNormal, [](){
//        glDisable(GL_CULL_FACE);
//        glViewport(0,0,g_winWidth,g_winHigh);
//    });
    
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });

    world::getInstance()->setCBBeforeDrawCall([st,shDepth,shCSM,floorObj,cubeObj1,cubeObj2,cubeObj3,cubeObj4,cubeObj5](){
//        glViewport(0,0,g_winWidth,g_winHigh);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        floorObj->setShader(shDepth);
        cubeObj1->setShader(shDepth);
        cubeObj2->setShader(shDepth);
        cubeObj3->setShader(shDepth);
        cubeObj4->setShader(shDepth);
        cubeObj5->setShader(shDepth);
        floorObj->draw();
        cubeObj1->draw();
        cubeObj2->draw();
        cubeObj3->draw();
        cubeObj4->draw();
        cubeObj5->draw();
        
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE3);
//        glBindTexture(GL_TEXTURE_2D_ARRAY, st.texDepthArr);
        
        glActiveTexture(GL_TEXTURE0+4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, st.texID);
        
        floorObj->setShader(shCSM);
        cubeObj1->setShader(shCSM);
        cubeObj2->setShader(shCSM);
        cubeObj3->setShader(shCSM);
        cubeObj4->setShader(shCSM);
        cubeObj5->setShader(shCSM);
        floorObj->draw();
        cubeObj1->draw();
        cubeObj2->draw();
        cubeObj3->draw();
        cubeObj4->draw();
        cubeObj5->draw();
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
}


void test_csm_1(){
    float farPlane=500.0f;
    float nearPlane=0.1f;
    camera* cam=world::getInstance()->getCamera();
    cam->setFarPlane(farPlane);
    material2* mtLight=createMaterial(1, 1, 1, 1);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
        flylog("point light init failed!");
        return;
    }
    plight->setPosition(glm::vec3(-2.1,0,0.2));
    plight->setScale(0.02);
    world::getInstance()->addPointLight(plight);
    glm::vec3 lightPos=plight->getPosition();

#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif

    //house
    cubeTex* floorObj=new cubeTex("./res/floor.png");
    int descArr[]={3,2,3};
    floorObj->initByVerticeArr(g_verticeArrWithTexCoordAndNormalReverse, sizeof(g_verticeArrWithTexCoordAndNormalReverse), descArr, 3);
    floorObj->setScale(5.0f);
//    world::getInstance()->addChild(floorObj);
     world::getInstance()->getControl()->bindNode(floorObj);
    
    cubeTex* cubeObj1=new cubeTex("./res/wood.png");
    cubeObj1->init();
    cubeObj1->setPosition(glm::vec3(0.1,0.1,0));
    cubeObj1->setScale(0.4);
//    world::getInstance()->addChild(cubeObj1);
    
    cubeTex* cubeObj2=new cubeTex("./res/wood.png");
    cubeObj2->init();
    cubeObj2->setPosition(glm::vec3(0.2,0.2,0.1));
    cubeObj2->setScale(0.2);
//    world::getInstance()->addChild(cubeObj2);
    
    cubeTex* cubeObj3=new cubeTex("./res/wood.png");
    cubeObj3->init();
    cubeObj3->setPosition(glm::vec3(-0.5,-0.5,0));
    cubeObj3->setScale(0.22);
//    world::getInstance()->addChild(cubeObj3);
    
    cubeTex* cubeObj4=new cubeTex("./res/wood.png");
    cubeObj4->init();
    cubeObj4->setPosition(glm::vec3(-0.7,0.5,0.2));
    cubeObj4->setScale(0.3);
//    world::getInstance()->addChild(cubeObj4);
    
    cubeTex* cubeObj5=new cubeTex("./res/wood.png");
    cubeObj5->init();
    cubeObj5->setPosition(glm::vec3(-1,1,0.3));
    cubeObj5->rotateBy(glm::vec3(60,0,60));
    cubeObj5->setScale(0.15);
//    world::getInstance()->addChild(cubeObj5);
    
    glm::vec3 camPos=glm::vec3(-2.2,0.1,0.3);
    cam->setPosition(camPos);
    cam->setYaw(-9.03);
    cam->setPitch(4.78);
//    cam->updateUBOForShadow(lightViewPos);
    
    
    shader* shDepth=new shader("./res/shader/depth_cubetex_shadow.vs","./res/shader/draw_null.fs","./res/shader/depth_cubetex_shadow_csm.gs");
    shader* shCSM=new shader("./res/shader/3d_1tex_csm.vs","./res/shader/3d_1tex_csm.fs");
    
    shCSM->use();
    std::vector<float> depthLevels={farPlane/50.0f,farPlane/25.0f,farPlane/10.0f,farPlane/2.0f};
    std::vector<glm::mat4> lightSpaceMatrices;
    fboCsmStruct st=fbo::createFBOCsm(depthLevels.size()+1);
    for(int i=0;i<depthLevels.size()+1;i++){
        if(i==0)
            lightSpaceMatrices.push_back(getLightSpaceMatrix(lightPos,nearPlane,depthLevels[0]));
        else if(i<depthLevels.size())
            lightSpaceMatrices.push_back(getLightSpaceMatrix(lightPos,depthLevels[i-1],depthLevels[i]));
        else
            lightSpaceMatrices.push_back(getLightSpaceMatrix(lightPos,depthLevels[i-1],farPlane));
    }
    shCSM->setInt("shadowLevelCount",depthLevels.size(),true);
    for(int i=0;i<depthLevels.size();i++){
        shCSM->setMat4("lightSpaceMatrices["+std::to_string(i)+"]",lightSpaceMatrices[i],true);
        shCSM->setFloat("shadowLevelDistance["+std::to_string(i)+"]",depthLevels[i],true);
    }
    
    shCSM->setInt("texture_shadow_array",3,true);
    shCSM->setFloat("far_plane", 25.0f,true);
    shCSM->setBool("gamma_correct", false,true);
    shCSM->setBool("bUseBlinnPhong", true,true);
    

//    world::getInstance()->addPass(stDepth.fbo, shDepth, [](){
//        glViewport(0,0,g_shadowWidth,g_shadowHigh);
//    });
//    world::getInstance()->addPass(0, shNormal, [](){
//        glDisable(GL_CULL_FACE);
//        glViewport(0,0,g_winWidth,g_winHigh);
//    });
    
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fboLight);
    });

    world::getInstance()->setCBBeforeDrawCall([st,shDepth,shCSM,floorObj,cubeObj1,cubeObj2,cubeObj3,cubeObj4,cubeObj5](){
//        glViewport(0,0,g_winWidth,g_winHigh);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        floorObj->setShader(shDepth);
        cubeObj1->setShader(shDepth);
        cubeObj2->setShader(shDepth);
        cubeObj3->setShader(shDepth);
        cubeObj4->setShader(shDepth);
        cubeObj5->setShader(shDepth);
        floorObj->draw();
        cubeObj1->draw();
        cubeObj2->draw();
        cubeObj3->draw();
        cubeObj4->draw();
        cubeObj5->draw();
        
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D_ARRAY, st.texDepthArr);
        floorObj->setShader(shCSM);
        cubeObj1->setShader(shCSM);
        cubeObj2->setShader(shCSM);
        cubeObj3->setShader(shCSM);
        cubeObj4->setShader(shCSM);
        cubeObj5->setShader(shCSM);
        floorObj->draw();
        cubeObj1->draw();
        cubeObj2->draw();
        cubeObj3->draw();
        cubeObj4->draw();
        cubeObj5->draw();
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
}
