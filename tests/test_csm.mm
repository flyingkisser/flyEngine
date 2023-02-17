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
    std::vector<glm::vec4> corners=getFrustrumCornersWorldSpace(cam->getPerspectiveMatrix(), cam->getLookAtMatrix());
    glm::vec3 center=getFrustrumCenter(corners);
    glm::mat4 lightView=glm::lookAt(center+lightPos, center, glm::vec3(0,1,0));
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();
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
        glViewport(0,0,g_winWidth,g_winHigh);
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
    
//    float r=1.6;
//    plight->setPositionX(plight->getPositionX()*r);
//    plight->setPositionZ(plight->getPositionZ()*r);
//    timerUtil::getInstance()->exec(0.1, [](camera* cam, pointLight* plight,cubeTex* cubeObj){
//        float v=0.08;
//        float x0=0;
//        float z0=0;
//        float x1=0;
//        float z1=0;
//        //灯光移动
//        x0=plight->getPositionX();
//        z0=plight->getPositionZ();
//        x1=x0*cos(v)-z0*sin(v);
//        z1=z0*cos(v)+x0*sin(v);
//        plight->setPositionX(x1);
//        plight->setPositionZ(z1);
//        //cube移动
//        //        cubeObj1->moveBy(glm::vec3(0.1,0,0));
//    }, cam,plight,cubeObj1);
}
