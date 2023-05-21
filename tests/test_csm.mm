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
#include "colors.h"


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
//    return lightView;
}

void test_csm_0(){
    std::vector<pointLight*> ligthtArr;
    std::vector<glm::vec3> posArr;
    float nearPlane=0.1f;
    float farPlane=100.0f;
    std::vector<float> depthLevels={farPlane/50.0f,farPlane/25.0f,farPlane/10.0f,farPlane/2.0f};
    
//    init_light_direction(0.1);
    material2* mtLight=createMaterial(1, 1, 1, 1);
//    material2* mtLight=createMaterial(0.1, 1, 1, 2);
    posArr.push_back(glm::vec3(0,3,2));
//    posArr.push_back(glm::vec3(-1,2,0));
//    posArr.push_back(glm::vec3(-1,3,-0.5));
//    posArr.push_back(glm::vec3(-1,3,0.5));
//    posArr.push_back(glm::vec3(0,2,3));
   
    //创建fbo绑定指定光源数量的texture_2d_array
    int lightNum=posArr.size();
//    fboStruct st=fbo::createFBOForDepthSampler2DArray(1);
//    int lightNum=depthLevels.size()+1;
    
//    fboCsmStruct st=fbo::createFBOCsm(depthLevels.size()+1);
    fboStruct st=fbo::createFBOCsm(3);

    
//    for(int i=0;i<lightNum;i++){
//        pointLight* plight=new pointLight(Color::IndianRed*0.1f,mtLight,0);
//        if(!plight->init()){
//           flylog("plight light init failed!");
//           return;
//        }
//        plight->setPosition(posArr[i]);
//        plight->setScale(glm::vec3(0.2,0.2,0.2));
//        plight->setVisible(false);
//        world::getInstance()->addPointLight(plight);
//        ligthtArr.push_back(plight);
//    }

       pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
       if(!plight->init()){
           flylog("point light init failed!");
           return;
       }
//       plight->setPosition(glm::vec3(-2.1,2,0.2));
    plight->setPosition(posArr[0]);
       plight->setScale(0.2);
       world::getInstance()->addPointLight(plight);
       glm::vec3 lightPos=plight->getPosition();
    ligthtArr.push_back(plight);
    
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
//    shader* shNormal=new shader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex_phong_shadow_sampler2DArray.fs");
    shader* shCSM=new shader("./res/shader/3d_1tex_csm.vs","./res/shader/3d_1tex_csm2.fs");
    shader* shNormal=shCSM;
    for(int i=0;i<depthLevels.size();i++){
           shCSM->setFloat("shadowLevelDistance["+std::to_string(i)+"]",depthLevels[i],true);
    }
    shCSM->setFloat("far_plane", farPlane,true);
    shCSM->setInt("shadowLevelCount",depthLevels.size(),true);
    shCSM->setInt("texture0",0,true);
    shCSM->setInt("texture_shadow_array",1,true);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, st.texID);
   
//    shNormal->use();
//    shNormal->setBool("bCheckShadow",true,true);
//    shNormal->setInt("shadowNum",lightNum,true);
//    shNormal->setInt("texture0",0,true);
//    shNormal->setInt("texture_shadow0",1,true);
//    glActiveTexture(GL_TEXTURE0+1);
//    glBindTexture(GL_TEXTURE_2D_ARRAY, st.texID);
    
  
    glm::mat4 matProj=glm::ortho(-10.0f,10.0f,-10.0f,10.0f,nearPlane,farPlane);
    shDepth->use();
    shDepth->setMat4("proj",  glm::mat4(1.0));
//    shDepth->setMat4("proj",  matProj);
    shDepth->setMat4("matModel", glm::mat4(1.0));

    sprite* sp=new sprite(st.texID);
    shader* shDepth2dDebug=new shader("./res/shader/2d_1tex.vs","./res/shader/2d_depth_array.fs");
    sp->flipY(true);
    sp->setShader(shDepth2dDebug);
    shDepth2dDebug->use();
    shDepth2dDebug->setInt("texture0",0,true);
    shDepth2dDebug->setInt("lightIndex",0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY,st.texID);
    
    world::getInstance()->setCBBeforeDrawCall([plight,depthLevels,nearPlane,farPlane,lightNum,st,ligthtArr,shDepth,shNormal,shDepth2dDebug,sp,cam,cubeObj,floorObj,matProj](){
        //第一个pass，把每个灯的view矩阵写入shader
        //因为在glTexImage3D设置texture_2d_array纹理时使用了5个维度，以及在depth_cubetex_shadow_csm.gs里硬编码了5个invocations
        //所以这里需要写入5个灯的view数据
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
//        glFramebufferTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D_ARRAY, st.texID, 0);
        glViewport(0,0,g_shadowWidth,g_shadowHigh);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shDepth->use();
        glm::vec3 lightPos=plight->getPosition();
        std::vector<glm::mat4> lightSpaceMatrices;
        for(int i=0;i<depthLevels.size()+1;i++){
           if(i==0)
               lightSpaceMatrices.push_back(getLightSpaceMatrix(lightPos,nearPlane,depthLevels[0]));
           else if(i<depthLevels.size())
               lightSpaceMatrices.push_back(getLightSpaceMatrix(lightPos,depthLevels[i-1],depthLevels[i]));
           else
               lightSpaceMatrices.push_back(getLightSpaceMatrix(lightPos,depthLevels[i-1],farPlane));
        }
        for(int i=0;i<lightSpaceMatrices.size();i++){
            shDepth->setMat4("lightSpaceMatrices["+std::to_string(i)+"]",lightSpaceMatrices[i]);
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
        
//        shDepth2dDebug->use();
//        sp->updateModel();
//        glBindVertexArray(sp->getVAO());
//        glDrawArrays(GL_TRIANGLES,0,6);
//        return;
       
        shNormal->use();
        for(int i=0;i<lightSpaceMatrices.size();i++){
            shNormal->setMat4("lightSpaceMatrices["+std::to_string(i)+"]",lightSpaceMatrices[i]);
        }
        cubeObj->setShader(shNormal);
        floorObj->setShader(shNormal);
        cubeObj->draw();
        floorObj->draw();
        for(int i=0;i<lightNum;i++)
            ligthtArr[i]->draw();
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
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('4', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('5', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('6', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('7', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('8', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('9', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('0', [plight0](){
        glm::vec3 pos=plight0->getPosition();
        plight0->setPositionX(pos.x-0.1);
    });
}
