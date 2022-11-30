//
//  test_normal_parallax_mapping.cpp
//  flyEngine
//
//  Created by joe on 08/11/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_normal_parallax_mapping.h"
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
#include "control.h"
#include "forever.h"
#include "rotateBy.h"
#include "fbo.h"
#include "sprite.h"
#include "shaderMgr.h"
#include "texture2.h"
#include "textureMgr.h"

#ifdef BUILD_IOS
#import "ViewController.h"
#endif

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

void initPosArr(sprite* spObj){
    // positions
    glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
    glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
    glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
    glm::vec3 pos4( 1.0f,  1.0f, 0.0f);
    // texture coordinates
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    // normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    // triangle 1
    // ----------
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    // triangle 2
    // ----------
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


    float quadVertices[] = {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };
    int descArr[]={3,3,2,3,3};
    spObj->initVAO(quadVertices, sizeof(quadVertices), descArr, 5);
}

//法向量左乘TBN矩阵，把法向量从切线空间变换到世界坐标空间，光源和相机坐标不需要变换，在片元着色器中进行空间变换
void test_normal_mapping_1(){
    camera* cam=world::getInstance()->getCamera();
  
    material2* mtLight=createMaterial(1, 1, 1, 1);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0.1,0.1,0.2));
    plight->setScale(0.03);
    world::getInstance()->addPointLight(plight);
    glm::vec3 lightViewPos=plight->getPosition();
    glm::vec3 camPos=glm::vec3(0,0,3);
    cam->setPosition(camPos);
    
    //sprite
    sprite* spObj=new sprite("./res/brickwall.jpg");
    initPosArr(spObj);
    spObj->setPosition(glm::vec3(0,0,-2.5));
    texture2* texNormal=new texture2("./res/brickwall_normal.jpg");
    texNormal->init();
    texNormal->glInit(GL_TEXTURE0+1);
    shader* sh=new shader("./res/shader/3d_1tex_board_normal.vs","./res/shader/3d_1tex_board_normal.fs");
    spObj->setShader(sh);
    sh->use();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texNormal->getTextureID());
    sh->setInt("texture0",0,true);
    sh->setInt("texture_normal",1,true);
    sh->setVec3("lightPos",glm::value_ptr(lightViewPos),true);
    world::getInstance()->addChild(spObj);
    world::getInstance()->getControl()->bindNode(spObj);
    spObj->setCBBeforeDrawCall([sh,plight](int programID){
        sh->use();
        sh->setVec3("lightPos",glm::value_ptr(plight->getPosition()),true);
    });
  
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });

    float r=3.6;
    plight->setPositionX(plight->getPositionX()*r);
    plight->setPositionY(plight->getPositionY()*r);
    timerUtil::getInstance()->exec(0.1, [](camera* cam, pointLight* plight,sprite* spObj){
        float v=0.08;
        float x0=0;
        float z0=0;
        float x1=0;
        float z1=0;
        //灯光移动
        x0=plight->getPositionX();
        z0=plight->getPositionY();
        x1=x0*cos(v)-z0*sin(v);
        z1=z0*cos(v)+x0*sin(v);
        plight->setPositionX(x1);
        plight->setPositionY(z1);
//        plight->setPositionX(x0+0.001);
        
        //cube移动
//        spObj->moveBy(glm::vec3(0.1,0,0));
    }, cam,plight,spObj);
}


//把光线向量和视角向量从世界空间变换到切线空间，然后在切线空间中计算光照，在片元着色器中进行空间变换
void test_normal_mapping_2(){
    camera* cam=world::getInstance()->getCamera();
  
    material2* mtLight=createMaterial(1, 1, 1, 1);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0.1,0.1,0.2));
    plight->setScale(0.03);
    world::getInstance()->addPointLight(plight);
    glm::vec3 lightViewPos=plight->getPosition();
    glm::vec3 camPos=glm::vec3(0,0,3);
    cam->setPosition(camPos);
    
    //sprite
    sprite* spObj=new sprite("./res/brickwall.jpg");
    initPosArr(spObj);
    spObj->setPosition(glm::vec3(0,0,-2.5));
    texture2* texNormal=new texture2("./res/brickwall_normal.jpg");
    texNormal->init();
    texNormal->glInit(GL_TEXTURE0+1);
    shader* sh=new shader("./res/shader/3d_1tex_board_normal_2.vs","./res/shader/3d_1tex_board_normal_2.fs");
    spObj->setShader(sh);
    sh->use();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texNormal->getTextureID());
    sh->setInt("texture0",0,true);
    sh->setInt("texture_normal",1,true);
    sh->setVec3("lightPos",glm::value_ptr(lightViewPos),true);
    world::getInstance()->addChild(spObj);
    world::getInstance()->getControl()->bindNode(spObj);
    spObj->setCBBeforeDrawCall([sh,plight](int programID){
        sh->use();
        sh->setVec3("lightPos",glm::value_ptr(plight->getPosition()),true);
    });
  
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });

    float r=4.6;
    plight->setPositionX(plight->getPositionX()*r);
    plight->setPositionY(plight->getPositionY()*r);
    timerUtil::getInstance()->exec(0.05, [](camera* cam, pointLight* plight,sprite* spObj){
        float v=0.08;
        float x0=0;
        float z0=0;
        float x1=0;
        float z1=0;
        //灯光移动
        x0=plight->getPositionX();
        z0=plight->getPositionY();
        x1=x0*cos(v)-z0*sin(v);
        z1=z0*cos(v)+x0*sin(v);
        plight->setPositionX(x1);
        plight->setPositionY(z1);
//        plight->setPositionX(x0+0.001);
        
        //cube移动
//        spObj->moveBy(glm::vec3(0.1,0,0));
    }, cam,plight,spObj);
}


//在顶点着色器中即把顶点坐标，相机坐标，光源坐标从世界空间变换到切线空间
//片元着色器因此不用做任何改变
void test_normal_mapping_3(){
    camera* cam=world::getInstance()->getCamera();
  
    material2* mtLight=createMaterial(1, 1, 1, 1);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0.1,0.1,0.2));
    plight->setScale(0.03);
    world::getInstance()->addPointLight(plight);
    glm::vec3 lightViewPos=plight->getPosition();
    glm::vec3 camPos=glm::vec3(0,0,3);
    cam->setPosition(camPos);
    
    //sprite
    sprite* spObj=new sprite("./res/brickwall.jpg");
    initPosArr(spObj);
    spObj->setPosition(glm::vec3(0,0,-2.5));
    texture2* texNormal=new texture2("./res/brickwall_normal.jpg");
    texNormal->init();
    texNormal->glInit(GL_TEXTURE0+1);
    shader* sh=new shader("./res/shader/3d_1tex_board_normal_3.vs","./res/shader/3d_1tex_board_normal_3.fs");
    spObj->setShader(sh);
    sh->use();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texNormal->getTextureID());
    sh->setInt("texture0",0,true);
    sh->setInt("texture_normal",1,true);
    sh->setVec3("lightPos",glm::value_ptr(lightViewPos),true);
    world::getInstance()->addChild(spObj);
    world::getInstance()->getControl()->bindNode(spObj);
    spObj->setCBBeforeDrawCall([sh,plight](int programID){
        sh->use();
        sh->setVec3("lightPos",glm::value_ptr(plight->getPosition()),true);
    });
  
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });

    float r=4.6;
    plight->setPositionX(plight->getPositionX()*r);
    plight->setPositionY(plight->getPositionY()*r);
    timerUtil::getInstance()->exec(0.05, [](camera* cam, pointLight* plight,sprite* spObj){
        float v=0.08;
        float x0=0;
        float z0=0;
        float x1=0;
        float z1=0;
        //灯光移动
        x0=plight->getPositionX();
        z0=plight->getPositionY();
        x1=x0*cos(v)-z0*sin(v);
        z1=z0*cos(v)+x0*sin(v);
        plight->setPositionX(x1);
        plight->setPositionY(z1);
//        plight->setPositionX(x0+0.001);
        
        //cube移动
//        spObj->moveBy(glm::vec3(0.1,0,0));
    }, cam,plight,spObj);
}

//对于模型，使用法向贴图进行光照计算
void test_normal_mapping_model(){
    camera* cam=world::getInstance()->getCamera();
  
    float am=0.1;
    float diff=0.1;
    float sp=1;
    float shine=2;
    material2* mtLight=createMaterial(am,diff,sp,shine);
    pointLight* plight=new pointLight(glm::vec3(1,1,1),mtLight,0);
    if(!plight->init()){
       flylog("point light init failed!");
       return;
    }
    plight->setPosition(glm::vec3(0.1,0.1,0.2));
    plight->setScale(0.03);
    world::getInstance()->addPointLight(plight);
    glm::vec3 lightViewPos=plight->getPosition();
    glm::vec3 camPos=glm::vec3(0,0,3);
    cam->setPosition(camPos);
    
    //sprite
    model* spObj=new model("./res/model/cyborg/cyborg.obj");
    if(!spObj->init()){
       flylog("spObj init failed!");
       return;
    }
    spObj->setPosition(glm::vec3(0,-1.5,-8.5));
    world::getInstance()->addChild(spObj);
    world::getInstance()->getControl()->bindNode(spObj);
    
    shader* sh=new shader("./res/shader/3d_model_normal.vs","./res/shader/3d_1tex_phong_normal.fs");
    spObj->setShader(sh);
    sh->use();
    sh->setBool("bUseBlinnPhong", true);
 
    //动态调整光照系数
    world::getInstance()->getControl()->regOnKeyPress('r', [cam,plight,am,diff,sp,shine](){
        material2* mt=plight->getMaterial();
        mt->setAmbient(am);
        mt->setDiffuse(diff);
        mt->setSpecular(sp);
        mt->setShiness(shine);
        plight->setDirtyUBO(true);
        flylog("reset light materail am %f diff %f sp %f shine %f",mt->getAmbientFloat(),mt->getDiffuseFloat(),mt->getSpecular(),mt->getShiness());
    });
    
    //环境光
    float inner=0.2;
    world::getInstance()->getControl()->regOnKeyPress('g', [cam,plight,inner](){
        material2* mt=plight->getMaterial();
        mt->setAmbient((float)mt->getAmbientFloat()+inner);
        plight->setDirtyUBO(true);
        flylog("ambient %f",mt->getAmbientFloat());
    });
    //漫反射
    world::getInstance()->getControl()->regOnKeyPress('h', [cam,plight,inner](){
        material2* mt=plight->getMaterial();
        mt->setDiffuse((float)mt->getDiffuseFloat()+inner);
        plight->setDirtyUBO(true);
        flylog("diffuse %f",mt->getDiffuseFloat());
    });
    //镜面反射
    world::getInstance()->getControl()->regOnKeyPress('j', [cam,plight,inner](){
        material2* mt=plight->getMaterial();
        mt->setSpecular((float)mt->getSpecularFloat()+inner);
        plight->setDirtyUBO(true);
        flylog("specular %f",mt->getSpecularFloat());
    });
    //镜面反射光泽度
    world::getInstance()->getControl()->regOnKeyPress('k', [cam,plight,inner](){
        material2* mt=plight->getMaterial();
        mt->setShiness((float)mt->getShiness()+inner);
        plight->setDirtyUBO(true);
        flylog("shiness %f",mt->getShiness());
    });
    
    world::getInstance()->getControl()->regOnKeyPress('t', [cam,plight,inner](){
        material2* mt=plight->getMaterial();
        mt->setAmbient((float)mt->getAmbientFloat()-inner);
        plight->setDirtyUBO(true);
        flylog("ambient %f",mt->getAmbientFloat());
    });
    //漫反射
    world::getInstance()->getControl()->regOnKeyPress('y', [cam,plight,inner](){
        material2* mt=plight->getMaterial();
        mt->setDiffuse((float)mt->getDiffuseFloat()-inner);
        plight->setDirtyUBO(true);
        flylog("diffuse %f",mt->getDiffuseFloat());
    });
    //镜面反射
    world::getInstance()->getControl()->regOnKeyPress('u', [cam,plight,inner](){
        material2* mt=plight->getMaterial();
        mt->setSpecular((float)mt->getSpecularFloat()-inner);
        plight->setDirtyUBO(true);
        flylog("specular %f",mt->getSpecularFloat());
    });
    //镜面反射光泽度
    world::getInstance()->getControl()->regOnKeyPress('i', [cam,plight,inner](){
        material2* mt=plight->getMaterial();
        mt->setShiness((float)mt->getShiness()-inner);
        plight->setDirtyUBO(true);
        flylog("shiness %f",mt->getShiness());
    });
}

//视差贴图
//法向量左乘TBN矩阵，把法向量从切线空间变换到世界坐标空间，光源和相机坐标不需要变换，在片元着色器中进行空间变换
void test_parallax_mapping_1(){
    camera* cam=world::getInstance()->getCamera();
    
    material2* mtLight=createMaterial(1, 1, 1, 1);
    pointLight* plight=new pointLight(glm::vec3(0.8,0.8,0.8),mtLight,0);
    if(!plight->init()){
        flylog("point light init failed!");
        return;
    }
    plight->setPosition(glm::vec3(0.1,0.1,0.2));
    plight->setScale(0.03);
    world::getInstance()->addPointLight(plight);
    glm::vec3 lightViewPos=plight->getPosition();
    glm::vec3 camPos=glm::vec3(0,0,3);
    cam->setPosition(camPos);
    
    //sprite
    sprite* spObj=new sprite("./res/bricks2.jpg");
    initPosArr(spObj);
    spObj->setPosition(glm::vec3(0,0,-2.5));
    texture2* texNormal=new texture2("./res/bricks2_normal.jpg");
    texNormal->init();
    texNormal->glInit(GL_TEXTURE0+1);
    
    texture2* texHeight=new texture2("./res/bricks2_height.jpg");
    texHeight->init();
    texHeight->glInit(GL_TEXTURE0+2);
    
    shader* sh=new shader("./res/shader/3d_1tex_board_normal.vs","./res/shader/3d_1tex_board_normal.fs");
    spObj->setShader(sh);
    sh->use();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texNormal->getTextureID());
    sh->setInt("texture_normal",1,true);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,texHeight->getTextureID());
    sh->setInt("texture_height",2,true);
    sh->setBool("bParallax", true,true);
//
    sh->setVec3("lightPos",glm::value_ptr(lightViewPos),true);
    world::getInstance()->addChild(spObj);
    world::getInstance()->getControl()->bindNode(spObj);
    spObj->setCBBeforeDrawCall([sh,plight](int programID){
        sh->use();
        sh->setVec3("lightPos",glm::value_ptr(plight->getPosition()),true);
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
    
//    bool p=true;
    world::getInstance()->getControl()->regOnKeyPress('g', [cam,plight,spObj](){
        shader* sh=spObj->getShader();
        sh->use();
        bool p=sh->getBool("bParallax");
        sh->setBool("bParallax", !p);
        flylog("set bParallax to %d",!p);
        bool p2=sh->getBool("bParallax");
    });
    
    float r=3.6;
    plight->setPositionX(plight->getPositionX()*r);
    plight->setPositionY(plight->getPositionY()*r);
    timerUtil::getInstance()->exec(0.1, [](camera* cam, pointLight* plight,sprite* spObj){
        float v=0.08;
        float x0=0;
        float z0=0;
        float x1=0;
        float z1=0;
        //灯光移动
        x0=plight->getPositionX();
        z0=plight->getPositionY();
        x1=x0*cos(v)-z0*sin(v);
        z1=z0*cos(v)+x0*sin(v);
//        plight->setPositionX(x1);
//        plight->setPositionY(z1);
        //        plight->setPositionX(x0+0.001);
        
        //cube移动
        //        spObj->moveBy(glm::vec3(0.1,0,0));
    }, cam,plight,spObj);
}
