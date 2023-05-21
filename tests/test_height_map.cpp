//
//  test_height_map.cpp
//  flyEngine
//
//  Created by joe on 10/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//download earth height map
//https://tangrams.github.io/heightmappe

#include "test_height_map.h"
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
#include "heightMap.h"
#include "heightMapPatch.h"
#include "state.h"
#include "quadSize.h"
#include "texture2.h"
#include "light.h"
#include "directionLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "semiSphere.h"

#ifdef BUILD_IOS
#import "ViewController.h"
#endif

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

static void init_light_direction(){
    //设置环境光
    directionLight* dirLight=new directionLight(glm::vec3(0.7f,0.7f,0.7f),glm::vec3(-0.2,-1,-0.3),0.1,0.1,0.1,2);
    world::getInstance()->setDirectiontLight(dirLight);
}


void test_height_map_by_cpu(){
    camera* cam=world::getInstance()->getCamera();
    cam->setPosition(glm::vec3(67.0f, 627.5f, 169.9f));
    cam->setFarPlane(100000.0);
    cam->setYaw(-128.1);
    cam->setPitch(-42.4);
    
    bool showStats=true;
    state::setShowDrawCall(showStats);
    state::setShowFrameRate(showStats);
    state::setShowVertices(showStats);
    state::setShowTime(showStats);
//    cam->setScreenRatio(2560.0/1440.0);
    
//    heightMap* heightObj=new heightMap("res/heightmap/iceland_heightmap.png");
    heightMap* heightObj=new heightMap("res/heightmap/river_heightmap.png");
//    heightMap* heightObj=new heightMap("res/heightmap/river2_heightmap.png");
    if(!heightObj->init()){
      flylog("heightObj init failed!");
      return;
    }
    heightObj->setPosition(glm::vec3(0,0,0));
    heightObj->setLineMode(true);
    world::getInstance()->addChild(heightObj);
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(heightObj);
}

//曲面细分的参数在曲面细分控制器里写死(当前是16，即一条边分割出来16个小边)
void test_height_map_by_gpu_simple(){
    camera* cam=world::getInstance()->getCamera();
    cam->setFarPlane(2000.0);
    cam->setPosition(glm::vec3(6.8f, 46.3f, -6.9f));
    cam->setYaw(-124.6);
    cam->setPitch(-0.12);
    
    bool showStats=false;
    state::setShowDrawCall(showStats);
    state::setShowFrameRate(showStats);
    state::setShowVertices(showStats);
    state::setShowTime(showStats);

//    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/iceland_heightmap.png");
    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/river_heightmap.png");
    //    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/river2_heightmap.png");
    
    if(!heightObj->init()){
        flylog("heightObj init failed!");
        return;
    }
    heightObj->setLineMode(true);
    heightObj->setDrawQuad(true);
    world::getInstance()->addChild(heightObj);
    
    shader* shSimple=new shader("./res/shader/height_gpu.vs", "./res/shader/height_gpu.fs",NULL,
                        "./res/shader/height_gpu_simple.tcs","./res/shader/height_gpu.tes");
    heightObj->setShader(shSimple);
    world::getInstance()->getControl()->bindNode(heightObj);
    
    texture2* tex=heightObj->getTexObj();
    quadSize* _quadSize=new quadSize(tex->getTextureID(),tex->getWidth(),tex->getHeight());
    world::getInstance()->addChild(_quadSize);
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
}

//曲面细分的参数动态计算出来，相机近的区域分的细，远的地方分的小
void test_height_map_by_gpu(){
    camera* cam=world::getInstance()->getCamera();
     cam->setFarPlane(1000.0);
    cam->setPosition(glm::vec3(6.8f, 26.3f, -6.9f));
    cam->setYaw(-124.6);
    cam->setPitch(-0.12);
    
    bool showStats=false;
    state::setShowDrawCall(showStats);
    state::setShowFrameRate(showStats);
    state::setShowVertices(showStats);
    state::setShowTime(showStats);
    
//    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/iceland_heightmap.png");
//    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/river_heightmap.png");
    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/hainan_height.png");
    //    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/river2_heightmap.png");
    
    if(!heightObj->init()){
        flylog("heightObj init failed!");
        return;
    }
    heightObj->setLineMode(true);
//    heightObj->setDrawQuad(true);
    
    world::getInstance()->addChild(heightObj);
    world::getInstance()->getControl()->bindNode(heightObj);
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
}

void test_height_terrain(){
    camera* cam=world::getInstance()->getCamera();
     cam->setFarPlane(1000.0);
    cam->setPosition(glm::vec3(6.8f, 26.3f, -6.9f));
    cam->setYaw(-124.6);
    cam->setPitch(-0.12);
    
    bool showStats=false;
    state::setShowDrawCall(showStats);
    state::setShowFrameRate(showStats);
    state::setShowVertices(showStats);
    state::setShowTime(showStats);
    
//    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/iceland_heightmap.png");
//    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/river_heightmap.png");
    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/hainan_height.png");
    if(!heightObj->init()){
        flylog("heightObj init failed!");
        return;
    }
    heightObj->setLineMode(false);
    heightObj->setDrawQuad(false);
    
//    texture2* tex=new texture2("res/heightmap/river_heightmap.png");
    texture2* tex=new texture2("res/rock.jpg");
    tex->init();
    tex->glInit();
  
    
    shader* sh=new shader("./res/shader/height_gpu.vs", "./res/shader/height_gpu_terrain.fs",NULL,
                          "./res/shader/height_gpu.tcs","./res/shader/height_gpu.tes");
    heightObj->setShader(sh);
    sh->use();
    sh->setInt("heightMap",0,true);
    sh->setInt("tex_terrain", 1,true);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,heightObj->getTexObj()->getTextureID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,tex->getTextureID());
    
    
    world::getInstance()->addChild(heightObj);
    world::getInstance()->getControl()->bindNode(heightObj);
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
    });
}


void test_height_hainan(){
    camera* cam=world::getInstance()->getCamera();
     cam->setFarPlane(2000.0);
    cam->setPosition(glm::vec3(6.8f, 26.3f, 121.1f));
    cam->setYaw(-141.6);
    cam->setPitch(-5.6);
    
    bool showStats=false;
    state::setShowDrawCall(showStats);
    state::setShowFrameRate(showStats);
    state::setShowVertices(showStats);
    state::setShowTime(showStats);
    
    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/hainan_height.png");
    if(!heightObj->init()){
        flylog("heightObj init failed!");
        return;
    }
//    heightObj->setPosition(glm::vec3(6.8,26.3,121.1));
    heightObj->setRotation(glm::vec3(0,-183,0));
    heightObj->setLineMode(false);
    heightObj->setDrawQuad(false);
    
    texture2* tex=new texture2("res/rock.jpg");
    tex->init();
    tex->glInit();
    
    shader* sh=new shader("./res/shader/height_gpu.vs", "./res/shader/height_gpu_terrain.fs",NULL,
                          "./res/shader/height_gpu.tcs","./res/shader/height_gpu.tes");
    heightObj->setShader(sh);
    sh->use();
    sh->setInt("heightMap",0,true);
    sh->setInt("tex_terrain", 1,true);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,heightObj->getTexObj()->getTextureID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,tex->getTextureID());
    
    world::getInstance()->addChild(heightObj);
    world::getInstance()->getControl()->bindNode(heightObj);
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam,heightObj](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
        glm::vec3 heiPos=heightObj->getPosition();
        glm::vec3 rotate=heightObj->getRotation();
        flylog("heightObj pos %f %f %f,rotation %f %f %f",pos.x,pos.y,pos.z,rotate.x,rotate.y,rotate.z);
    });
}

void test_height_hainan_light(){
    
    init_light_direction();
    
    camera* cam=world::getInstance()->getCamera();
    cam->setFarPlane(2000.0);
    cam->setPosition(glm::vec3(6.8f, 26.3f, 249.1));
    cam->setYaw(-446);
    cam->setPitch(-9.1);
    
    bool showStats=false;
    state::setShowDrawCall(showStats);
    state::setShowFrameRate(showStats);
    state::setShowVertices(showStats);
    state::setShowTime(showStats);
    
    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/hainan_height.png");
    if(!heightObj->init()){
        flylog("heightObj init failed!");
        return;
    }
//    heightObj->setPosition(glm::vec3(6.8,26.3,121.1));
    heightObj->setRotation(glm::vec3(0,-183,0));
    heightObj->setLineMode(false);
    heightObj->setDrawQuad(false);
    
    texture2* tex=new texture2("res/rock.jpg");
    tex->init();
    tex->glInit();
    
    shader* sh=new shader("./res/shader/height_gpu.vs", "./res/shader/height_gpu_light.fs",NULL,
                          "./res/shader/height_gpu.tcs","./res/shader/height_gpu_light.tes");
    heightObj->setShader(sh);
    sh->use();
    sh->setInt("heightMap",0,true);
    sh->setInt("texture0", 1,true);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,heightObj->getTexObj()->getTextureID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,tex->getTextureID());
    
    world::getInstance()->addChild(heightObj);
    world::getInstance()->getControl()->bindNode(heightObj);
    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(1,0.2,0.1),mtLight,0);
    if(!lightObj1->init()){
       flylog("point light init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(6.8f, 15, 200));
    lightObj1->setScale(10);
    world::getInstance()->addPointLight(lightObj1);
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam,heightObj](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
        glm::vec3 heiPos=heightObj->getPosition();
        glm::vec3 rotate=heightObj->getRotation();
        flylog("heightObj pos %f %f %f,rotation %f %f %f",pos.x,pos.y,pos.z,rotate.x,rotate.y,rotate.z);
    });
    
    world::getInstance()->setCBBeforeDrawCall([sh,cam](){
        sh->use();
        sh->setVec3("uni_cam_pos", cam->getPosition());
    });
    
    pointLight* plight0=lightObj1;
    float move=10;
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


void test_height_hainan_light_pbr(){
    init_light_direction();
    
    camera* cam=world::getInstance()->getCamera();
    cam->setFarPlane(2000.0);
    cam->setPosition(glm::vec3(6.8f, 26.3f, 249.1));
    cam->setYaw(-446);
    cam->setPitch(-9.1);
    
    bool showStats=false;
    state::setShowDrawCall(showStats);
    state::setShowFrameRate(showStats);
    state::setShowVertices(showStats);
    state::setShowTime(showStats);
    
    heightMapPatch* heightObj=new heightMapPatch("res/heightmap/hainan_height.png");
    if(!heightObj->init()){
        flylog("heightObj init failed!");
        return;
    }
//    heightObj->setPosition(glm::vec3(6.8,26.3,121.1));
    heightObj->setRotation(glm::vec3(0,-183,0));
    heightObj->setLineMode(false);
    heightObj->setDrawQuad(false);
    
    world::getInstance()->addChild(heightObj);
    world::getInstance()->getControl()->bindNode(heightObj);
   
    texture2* tex=new texture2("res/rock.jpg");
    tex->init();
    tex->glInit();
    
    shader* sh=new shader("./res/shader/height_gpu.vs", "./res/shader/height_gpu_light_pbr.fs",NULL,
                          "./res/shader/height_gpu.tcs","./res/shader/height_gpu_light.tes");
    heightObj->setShader(sh);
    sh->use();
    sh->setInt("heightMap",0,true);
    sh->setInt("tex_albedo", 1);
    sh->setInt("tex_normal", 2);
    sh->setInt("tex_metallic", 3);
    sh->setInt("tex_roughness", 4);
    sh->setInt("tex_ao", 5);
    
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
    
    world::getInstance()->setCBBeforeDrawCall([sh,cam,heightObj,tex0,tex1,tex2,tex3,tex4](){
//        sh->use();
//        sh->setVec3("uni_cam_pos", cam->getPosition());
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,heightObj->getTexObj()->getTextureID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,tex0->getTextureID());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,tex1->getTextureID());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,tex2->getTextureID());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,tex3->getTextureID());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D,tex4->getTextureID());
    });

    
    //光源1
    material2* mtLight=createMaterial(0.3, 0.8, 0.8, 2);
    pointLight* lightObj1=new pointLight(glm::vec3(1,0.2,0.1),mtLight,0);
    if(!lightObj1->init()){
       flylog("point light init failed!");
       return;
    }
    lightObj1->setPosition(glm::vec3(6.8f, 15, 200));
    lightObj1->setScale(10);
    world::getInstance()->addPointLight(lightObj1);
    
    world::getInstance()->getControl()->regOnKeyPress('g', [cam,heightObj](){
        glm::vec3 pos=cam->getPosition();
        float yaw=cam->getYaw();
        float pitch=cam->getPitch();
        flylog("cam at %f %f %f,yaw %f pitch %f",pos.x,pos.y,pos.z,yaw,pitch);
        glm::vec3 heiPos=heightObj->getPosition();
        glm::vec3 rotate=heightObj->getRotation();
        flylog("heightObj pos %f %f %f,rotation %f %f %f",pos.x,pos.y,pos.z,rotate.x,rotate.y,rotate.z);
    });
    
    
    //add texobj
    
//    shader* shPBR=new shader("res/shader/3d_1tex_pbr.vs","res/shader/3d_1tex_pbr.fs");
    shader* shPBR=new shader("res/shader/3d_1tex_pbr.vs","res/shader/height_gpu_light_pbr.fs");
    
    shPBR->use();
    shPBR->setInt("tex_albedo", 0);
    shPBR->setInt("tex_normal", 1);
    shPBR->setInt("tex_metallic", 2);
    shPBR->setInt("tex_roughness", 3);
    shPBR->setInt("tex_ao", 4);
    
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
    world::getInstance()->getControl()->regOnKeyPress('g', [cam,lightObj1](){
        cam->setPosition(glm::vec3(0,0,0));
        lightObj1->setPosition(glm::vec3(-0.5,0.5,-1));
        lightObj1->setScale(glm::vec3(0.2));
    });
    
    
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
