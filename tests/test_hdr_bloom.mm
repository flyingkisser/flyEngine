//
//  test_parallax_mapping.cpp
//  flyEngine
//
//  Created by Joe on 2022/11/26.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_hdr_bloom.h"
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
#include "quad.h"
#include "shaderMgr.h"

#ifdef BUILD_IOS
#import "ViewController.h"
#endif

USE_NS_FLYENGINE

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
}

//创建hdr相应的frame buffer，这样颜色值可以超过1.0
void test_HDR_1(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    cubeObj->setPosition(glm::vec3(0,0,25));
    cubeObj->setScale(glm::vec3(5,5,50));
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    fboStruct st=fbo::createFBOHDR();
    quad* sp=new quad(st.texID,15,15);
    sp->flipY(true);
    
    material2* mt=createMaterial(1, 1, 1, 0);
    pointLight* light1=new pointLight(glm::vec3(200,200,200),mt);
    pointLight* light2=new pointLight(glm::vec3(0.1,0,0),mt);
    pointLight* light3=new pointLight(glm::vec3(0,0,0.2),mt);
    pointLight* light4=new pointLight(glm::vec3(0,0.1,0),mt);
    light1->setPosition(glm::vec3(0,0,0));
    light2->setPosition(glm::vec3(-1.4,-1.9,9.0));
    light3->setPosition(glm::vec3(0,-1.8,4.0));
    light4->setPosition(glm::vec3(0.8,-1.7,6.0));
    light1->setScale(0.1f);
    light2->setScale(0.1f);
    light3->setScale(0.1f);
    light4->setScale(0.1f);
    world::getInstance()->addPointLight(light1);
    world::getInstance()->addPointLight(light2);
    world::getInstance()->addPointLight(light3);
    world::getInstance()->addPointLight(light4);

    cam->setPosition(glm::vec3(0,0,40));
    
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    
world::getInstance()->setCBBeforeDrawCall([st,cam,sp,cubeObj,view,light1,light2,light3,light4](){
        cubeObj->draw();
        light1->draw();
        light2->draw();
        light3->draw();
        light4->draw();
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sp->draw();
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g',[sp](){
        shader* sh=sp->getShader();
        sh->use();
        float v=sh->getFloat("exposure");
        sh->setFloat("exposure",v+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('h',[sp](){
        shader* sh=sp->getShader();
        sh->use();
        float v=sh->getFloat("exposure");
        if(v-0.1>=0)
            sh->setFloat("exposure",v-0.1);
    });
}

//hdr启用tone mapping 色调映射
void test_HDR_2(){
    camera* cam=world::getInstance()->getCamera();
    cam->setPosition(glm::vec3(0,0,40));
    
    cubeTex* cubeObj=new cubeTex("./res/wood.png");
    cubeObj->init();
    //    cubeObj->setPosition(glm::vec3(0,0,-3));
    cubeObj->setPosition(glm::vec3(0,0,25));
    //    cubeObj->setRotation(glm::vec3(30,30,0));
    //    cubeObj->setScale(glm::vec3(2.5,2.5,27.5));
    cubeObj->setScale(glm::vec3(5,5,50));
    world::getInstance()->getControl()->bindNode(cubeObj);
    
    fboStruct st=fbo::createFBOHDR();
    quad* sp=new quad(st.texID,15,15);
//    sprite* sp=new sprite(st.texID);
//    sp->flipY(true);
//    sp->setContentSize(flyEngine::size{(float)g_winWidth,(float)g_winHigh});
    
    material2* mt=createMaterial(1, 1, 1, 0);
    pointLight* light1=new pointLight(glm::vec3(200,200,200),mt);
    pointLight* light2=new pointLight(glm::vec3(0.1,0,0),mt);
    pointLight* light3=new pointLight(glm::vec3(0,0,0.2),mt);
    pointLight* light4=new pointLight(glm::vec3(0,0.1,0),mt);
    //    light1->setPosition(glm::vec3(-0.5,0,0));
    //    light2->setPosition(glm::vec3(0.5,0,0));
    //    light3->setPosition(glm::vec3(0,1,0.5));
    //    light4->setPosition(glm::vec3(0,-1,0.5));
    light1->setPosition(glm::vec3(0,0,0));
    light2->setPosition(glm::vec3(-1.4,-1.9,9.0));
    light3->setPosition(glm::vec3(0,-1.8,4.0));
    light4->setPosition(glm::vec3(0.8,-1.7,6.0));
    light1->setScale(0.1f);
    light2->setScale(0.1f);
    light3->setScale(0.1f);
    light4->setScale(0.1f);
    world::getInstance()->addPointLight(light1);
    world::getInstance()->addPointLight(light2);
    world::getInstance()->addPointLight(light3);
    world::getInstance()->addPointLight(light4);
    
    
    
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fbo);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    
    shader* sh=sp->getShader();
    sh->use();
    sh->setBool("bToneMapping", true,true); //启用色调映射
    sh->setFloat("exposure", 0.1,true);   //曝光度越大，黑暗处会出现更多的细节，适用于晚上，曝光度越小，亮处的细节越多，适用于白天
    sp->setPosition(glm::vec3(0,0,0));
    
    world::getInstance()->setCBBeforeDrawCall([st,cam,sp,cubeObj,view,light1,light2,light3,light4](){
        cubeObj->draw();
        light1->draw();
        light2->draw();
        light3->draw();
        light4->draw();
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        sp->draw();
    });
    
    world::getInstance()->getControl()->regOnKeyPress('g',[sp](){
        shader* sh=sp->getShader();
        sh->use();
        float v=sh->getFloat("exposure");
        sh->setFloat("exposure",v+0.1);
    });
    world::getInstance()->getControl()->regOnKeyPress('h',[sp](){
        shader* sh=sp->getShader();
        sh->use();
        float v=sh->getFloat("exposure");
        if(v-0.1>=0)
            sh->setFloat("exposure",v-0.1);
    });
}

//bloom测试
//第一个fbo渲染出两张纹理，普通颜色和高光
//第二组fbo对应两张纹理，对高光纹理进行平行和垂直两个方向的高斯模糊
//最后把刚才最后一次输出的纹理，贴图到全屏quad上
void test_bloom_1(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj1=new cubeTex("./res/wood.png");
    cubeTex* cubeObj2=new cubeTex("./res/wood.png");
    cubeTex* cubeObj3=new cubeTex("./res/wood.png");
    cubeTex* cubeObj4=new cubeTex("./res/wood.png");

    cubeObj1->init();
    cubeObj2->init();
    cubeObj3->init();
    cubeObj4->init();
    
    cubeObj1->setPosition(glm::vec3(0,-1,0));
    cubeObj2->setPosition(glm::vec3(0,1.5,0));
    cubeObj3->setPosition(glm::vec3(2,0,1));
    cubeObj4->setPosition(glm::vec3(-1,-1,2));
    
    cubeObj1->setScale(glm::vec3(2.5,0.5,2.5));
    cubeObj2->setScale(0.5);
    cubeObj3->setScale(0.5);
    cubeObj4->setScale(1);
    
    cubeObj1->setRotation(glm::vec3(30,30,0));
    cubeObj4->setRotation(glm::vec3(23,0,23));
    
    world::getInstance()->getControl()->bindNode(cubeObj1);
    world::getInstance()->getControl()->bindNode(cubeObj2);
    world::getInstance()->getControl()->bindNode(cubeObj3);
    world::getInstance()->getControl()->bindNode(cubeObj4);
    
//    world::getInstance()->addChild(cubeObj1);
//    world::getInstance()->addChild(cubeObj2);
//    world::getInstance()->addChild(cubeObj3);
//    world::getInstance()->addChild(cubeObj4);
    
    fboHDRBloomGaussStruct st=fbo::createFBOHDRBloomGauss();
  
    material2* mt=createMaterial(1, 1, 1, 0);
    pointLight* light1=new pointLight(glm::vec3(50,50,50),mt);
    pointLight* light2=new pointLight(glm::vec3(20,0,0),mt);
    pointLight* light3=new pointLight(glm::vec3(0,0,50),mt);
    pointLight* light4=new pointLight(glm::vec3(0,10,0),mt);
//    light1->setPosition(glm::vec3(-0.5,0,0));
//    light2->setPosition(glm::vec3(0.5,0,0));
//    light3->setPosition(glm::vec3(0,1,0.5));
//    light4->setPosition(glm::vec3(0,-1,0.5));
    light1->setPosition(glm::vec3(0,0.5,1.5));
    light2->setPosition(glm::vec3(-4,0.5,-3));
    light3->setPosition(glm::vec3(3,0.5,1));
    light4->setPosition(glm::vec3(-0.8,2.4,2));
    
    float lightScale=0.5;
    light1->setScale(lightScale);
    light2->setScale(lightScale);
    light3->setScale(lightScale);
    light4->setScale(lightScale);
    light1->setVisible(false);
    light2->setVisible(false);
    light3->setVisible(false);
    light4->setVisible(false);
    world::getInstance()->addPointLight(light1);
    world::getInstance()->addPointLight(light2);
    world::getInstance()->addPointLight(light3);
    world::getInstance()->addPointLight(light4);
    
    cam->setPosition(glm::vec3(0,0,40));
    
    quad* quad2D=new quad(st.texHDRArr[0],15,15);
    quad2D->flipY(true);

    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fboHDR);
    });

#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
//
//    //渲染到两张纹理
    shader* shCubeBloom=new shader("./res/shader/3d_1tex_phong.vs","./res/shader/3d_1tex_bloom.fs");
    shader* shLightBloom=new shader("./res/shader/3d_color.vs","./res/shader/3d_color_bloom.fs");
    shader* shGuassian=new shader("./res/shader/3d_quad.vs","./res/shader/2d_gaussian.fs");
    shader* shBind=new shader("./res/shader/3d_quad.vs","./res/shader/2d_hdr_bloomblur.fs");
    shader* shQuad=quad2D->getShader();
    shader* shLight=light1->getShader();
    shBind->use();
    shBind->setInt("scene", 0);
    shBind->setInt("bloomBlur", 1);
    shBind->setFloat("exposure", 0.1);
    world::getInstance()->setCBBeforeDrawCall([st,cam,shCubeBloom,shLightBloom,shQuad,shGuassian,shBind,quad2D,cubeObj1,cubeObj2,cubeObj3,cubeObj4,view,light1,light2,light3,light4](){
        //第一步：场景渲染到st.fboHDR的两张纹理中(一张输出正常颜色，一张只写入高亮度的部分)
        cubeObj1->setShader(shCubeBloom);
        cubeObj2->setShader(shCubeBloom);
        cubeObj3->setShader(shCubeBloom);
        cubeObj4->setShader(shCubeBloom);
        light1->setShader(shLightBloom);
        light2->setShader(shLightBloom);
        light3->setShader(shLightBloom);
        light4->setShader(shLightBloom);
        
        cubeObj1->draw();
        cubeObj2->draw();
        cubeObj3->draw();
        cubeObj4->draw();
        light1->draw();
        light2->draw();
        light3->draw();
        light4->draw();

        //第二步
//        glDisable(GL_DEPTH_TEST);
//        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);

        //进行10次左右和上下方向的高斯模糊
        //每次单独写进一个framebuffer和它对应的纹理
        //第0次，绑定亮度纹理，输出到1号framebuffer对应的1号纹理
        //第一次，绑定1号纹理，但输出到0号framebuffer对应的0号纹理
        //第二次，绑定0号纹理，但输出到1号framebffer对应的1号纹理
        //这样就实现了对两个纹理的混合使用
        bool bHorizontal=true;
        bool bFirst=true;
        quad2D->setShader(shGuassian);
        shGuassian->use();
        for(int i=0;i<10;i++){
            glBindFramebuffer(GL_FRAMEBUFFER,st.fboBloomArr[bHorizontal]);
            shGuassian->setBool("horizontal", bHorizontal,true);
            unsigned int texID=bFirst?st.texHDRArr[1]:st.texGaussianArr[!bHorizontal];
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,texID);
            quad2D->setTexID(texID);
            quad2D->draw();
            bHorizontal=!bHorizontal;
            if(bFirst)
                bFirst=false;
        }
        
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
//        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad2D->setShader(shBind);
        quad2D->setTexID(st.texHDRArr[0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, st.texHDRArr[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, st.texGaussianArr[!bHorizontal]);
//         glBindTexture(GL_TEXTURE_2D, st.texGaussianArr[1]);
        quad2D->draw();
    });
    
    //    struct fboHDRBloomGaussStruct{
    //        unsigned int fboHDR;   //frame buffer id for hdr
    //        unsigned int fboBloomArr[2];   //frame buffer arr for bloom
    //        unsigned int rbo;   //render buffer id
    //        unsigned int texHDRArr[2]; //texture id arr1 for hdr color and brightness
    //        unsigned int texGaussianArr[2]; //texture id arr2 for bloom pingpong blur
    //    };
    
//    world::getInstance()->getControl()->regOnKeyPress('g',[sp](){
//        shader* sh=sp->getShader();
//        sh->use();
//        float v=sh->getFloat("exposure");
//        sh->setFloat("exposure",v+0.1);
//    });
//    world::getInstance()->getControl()->regOnKeyPress('h',[sp](){
//        shader* sh=sp->getShader();
//        sh->use();
//        float v=sh->getFloat("exposure");
//        if(v-0.1>=0)
//            sh->setFloat("exposure",v-0.1);
//    });
}


//physical based bloom
void test_bloom_2(){
    camera* cam=world::getInstance()->getCamera();
    cubeTex* cubeObj1=new cubeTex("./res/wood.png");
    cubeTex* cubeObj2=new cubeTex("./res/wood.png");
    cubeTex* cubeObj3=new cubeTex("./res/wood.png");
    cubeTex* cubeObj4=new cubeTex("./res/wood.png");
    
    cubeObj1->init();
    cubeObj2->init();
    cubeObj3->init();
    cubeObj4->init();
    
    cubeObj1->setPosition(glm::vec3(0,-1,0));
    cubeObj2->setPosition(glm::vec3(0,1.5,0));
    cubeObj3->setPosition(glm::vec3(2,0,1));
    cubeObj4->setPosition(glm::vec3(-1,-1,2));
    
    cubeObj1->setScale(glm::vec3(2.5,0.5,2.5));
    cubeObj2->setScale(0.5);
    cubeObj3->setScale(0.5);
    cubeObj4->setScale(1);
    
    cubeObj1->setRotation(glm::vec3(30,30,0));
    cubeObj4->setRotation(glm::vec3(23,0,23));
    
    world::getInstance()->getControl()->bindNode(cubeObj1);
    world::getInstance()->getControl()->bindNode(cubeObj2);
    world::getInstance()->getControl()->bindNode(cubeObj3);
    world::getInstance()->getControl()->bindNode(cubeObj4);
    
    fboHDRBloomPhysicalStruct st=fbo::createFBOHDRBloomPhysical(5);
    
    material2* mt=createMaterial(1, 1, 1, 0);
    pointLight* light1=new pointLight(glm::vec3(50,50,50),mt);
    pointLight* light2=new pointLight(glm::vec3(20,0,0),mt);
    pointLight* light3=new pointLight(glm::vec3(0,0,50),mt);
    pointLight* light4=new pointLight(glm::vec3(0,10,0),mt);
    light1->setPosition(glm::vec3(0,0.5,1.5));
    light2->setPosition(glm::vec3(-4,0.5,-3));
    light3->setPosition(glm::vec3(3,0.5,1));
    light4->setPosition(glm::vec3(-0.8,2.4,2));
    
    float lightScale=0.5;
    light1->setScale(lightScale);
    light2->setScale(lightScale);
    light3->setScale(lightScale);
    light4->setScale(lightScale);
    light1->setVisible(false);
    light2->setVisible(false);
    light3->setVisible(false);
    light4->setVisible(false);
    world::getInstance()->addPointLight(light1);
    world::getInstance()->addPointLight(light2);
    world::getInstance()->addPointLight(light3);
    world::getInstance()->addPointLight(light4);
    
    cam->setPosition(glm::vec3(0,0,40));
    
    quad* quad2D=new quad(st.texHDRArr[0],15,15);
    quad2D->flipY(true);
    
    world::getInstance()->setCBBeforeAnyGLCall([st](){
        glBindFramebuffer(GL_FRAMEBUFFER,st.fboHDR);
    });
    
#ifdef BUILD_IOS
    GLKView* view=[ViewController getView];
#else
    int view=0;
#endif
    //
    //    //渲染到两张纹理
    shader* shCubeBloom=new shader("./res/shader/3d_1tex_phong.vs","./res/shader/3d_1tex_bloom.fs");
    shader* shLightBloom=new shader("./res/shader/3d_color.vs","./res/shader/3d_color_bloom.fs");
    shader* shBloomDown=new shader("./res/shader/2d_bloom.vs","./res/shader/2d_bloom_down_sample.fs");
    shader* shBloomUp=new shader("./res/shader/2d_bloom.vs","./res/shader/2d_bloom_up_sample.fs");
    shader* shBind=new shader("./res/shader/3d_quad.vs","./res/shader/2d_hdr_bloomblur.fs");
    shader* shQuad=quad2D->getShader();
    shader* shLight=light1->getShader();
    shBind->use();
    shBind->setInt("scene", 0);
    shBind->setInt("bloomBlur", 1);
    shBind->setFloat("exposure", 0.1);
    
    shBloomDown->use();
    shBloomDown->setInt("srcTexture", 0);
    
    shBloomUp->use();
    shBloomUp->setInt("srcTexture", 0);
    
    glm::vec2 viewportFloatSize=glm::vec2((float)g_winWidth,(float)g_winHigh);
    glm::ivec2 viewportIntSize=glm::ivec2((int)g_winWidth,(int)g_winHigh);
    
    world::getInstance()->setCBBeforeDrawCall([st,cam,shCubeBloom,shLightBloom,shQuad,shBloomDown,shBloomUp,shBind,quad2D,cubeObj1,cubeObj2,cubeObj3,cubeObj4,view,light1,light2,light3,light4](){
       
        cubeObj1->setShader(shCubeBloom);
        cubeObj2->setShader(shCubeBloom);
        cubeObj3->setShader(shCubeBloom);
        cubeObj4->setShader(shCubeBloom);
        
        light1->setShader(shLightBloom);
        light2->setShader(shLightBloom);
        light3->setShader(shLightBloom);
        light4->setShader(shLightBloom);
        
        cubeObj1->draw();
        cubeObj2->draw();
        cubeObj3->draw();
        cubeObj4->draw();
        light1->draw();
        light2->draw();
        light3->draw();
        light4->draw();
        
        //第二步
        
        shBloomDown->use();
        shBloomDown->setVec2("srcResolution",viewportFloatSize);
        cubeObj1->setShader(shBloomDown);
        cubeObj2->setShader(shBloomDown);
        cubeObj3->setShader(shBloomDown);
        cubeObj4->setShader(shBloomDown);
        cubeObj1->draw();
        cubeObj2->draw();
        cubeObj3->draw();
        cubeObj4->draw();
        
        //        glDisable(GL_DEPTH_TEST);
        //        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        //        glClear(GL_COLOR_BUFFER_BIT);
        
        //进行10次左右和上下方向的高斯模糊
        //每次单独写进一个framebuffer和它对应的纹理
        //第0次，绑定亮度纹理，输出到1号framebuffer对应的1号纹理
        //第一次，绑定1号纹理，但输出到0号framebuffer对应的0号纹理
        //第二次，绑定0号纹理，但输出到1号framebffer对应的1号纹理
        //这样就实现了对两个纹理的混合使用
        bool bHorizontal=true;
        bool bFirst=true;
        quad2D->setShader(shGuassian);
        shGuassian->use();
        for(int i=0;i<10;i++){
            glBindFramebuffer(GL_FRAMEBUFFER,st.fboBloomArr[bHorizontal]);
            shGuassian->setBool("horizontal", bHorizontal,true);
            unsigned int texID=bFirst?st.texHDRArr[1]:st.texGaussianArr[!bHorizontal];
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,texID);
            quad2D->setTexID(texID);
            quad2D->draw();
            bHorizontal=!bHorizontal;
            if(bFirst)
                bFirst=false;
        }
        
#ifdef BUILD_MAC
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
#elif BUILD_IOS
        [view bindDrawable];    //绑定GLKView创建的framebuffer
#endif
        //        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad2D->setShader(shBind);
        quad2D->setTexID(st.texHDRArr[0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, st.texHDRArr[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, st.texGaussianArr[!bHorizontal]);
        //         glBindTexture(GL_TEXTURE_2D, st.texGaussianArr[1]);
        quad2D->draw();
    });
    
    //    struct fboHDRBloomGaussStruct{
    //        unsigned int fboHDR;   //frame buffer id for hdr
    //        unsigned int fboBloomArr[2];   //frame buffer arr for bloom
    //        unsigned int rbo;   //render buffer id
    //        unsigned int texHDRArr[2]; //texture id arr1 for hdr color and brightness
    //        unsigned int texGaussianArr[2]; //texture id arr2 for bloom pingpong blur
    //    };
    
    //    world::getInstance()->getControl()->regOnKeyPress('g',[sp](){
    //        shader* sh=sp->getShader();
    //        sh->use();
    //        float v=sh->getFloat("exposure");
    //        sh->setFloat("exposure",v+0.1);
    //    });
    //    world::getInstance()->getControl()->regOnKeyPress('h',[sp](){
    //        shader* sh=sp->getShader();
    //        sh->use();
    //        float v=sh->getFloat("exposure");
    //        if(v-0.1>=0)
    //            sh->setFloat("exposure",v-0.1);
    //    });
}
