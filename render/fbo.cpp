//
//  fbo.cpp
//  flyEngine
//
//  Created by joe on 15/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "fbo.h"
#include "defines.h"

#include "window.h"
#include "logUtil.h"

USE_NS_FLYENGINE
fboStruct fbo::createFBO(){
    unsigned int fbo=0;
    unsigned int texID=0;
    unsigned int rbo=0;
    //1.create fbo
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    //2.1 create texture
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_2D,texID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,g_winWidth,g_winHigh,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //2.2 bind texture to fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texID,0);
    //3.1 create rbo(depth and stencil)
    glGenRenderbuffers(1,&rbo);
    glBindRenderbuffer(GL_RENDERBUFFER,rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,g_winWidth,g_winHigh);
    //3.2 bind rbo to fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
    int status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status!=GL_FRAMEBUFFER_COMPLETE){
        fboStruct st={0,0,0};
        flylog("createFBO: glStatus is 0x%x!failed!！！！！！！！！！！！！！！---------------------",status);
        return st;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    fboStruct st={fbo,rbo,texID};
    return st;
}
//request opengl 3.2+
fboStruct fbo::createFBOMSAA(){
#if GL_VER >= 320
    unsigned int fbo=0;
    unsigned int texID=0;
    unsigned int rbo=0;
    int sampleNum=4;
    //1.create fbo
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    //2.1 create texture
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,texID);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,sampleNum,GL_RGB,g_winWidth,g_winHigh,GL_TRUE);
    //2.2 bind texture to fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,texID,0);
    //3.1 create rbo(depth and stencil)
    glGenRenderbuffers(1,&rbo);
    glBindRenderbuffer(GL_RENDERBUFFER,rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER,sampleNum,GL_DEPTH24_STENCIL8,g_winWidth,g_winHigh);
    //3.2 bind rbo to fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
    int status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status!=GL_FRAMEBUFFER_COMPLETE){
        fboStruct st={0,0,0};
        flylog("createFBOMSAA: glStatus is 0x%x!failed!！！！！！！！！！！！！！！---------------------",status);
        return st;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    fboStruct st={fbo,rbo,texID};
    return st;
#else
    fboStruct st={0,0,0};
    return st;
#endif
}

fboStruct fbo::createFBOForDepth(){
    unsigned int fbo=0;
    unsigned int texID=0;
    unsigned int rbo=0;
    //1.create fbo
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    //2.1 create texture
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_2D,texID);
#ifdef BUILD_IOS
    //check https://registry.khronos.org/OpenGL-Refpages/es3.0/
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32F,g_shadowWidth,g_shadowHigh,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  //需要把边界及以外的地方，保证深度贴图的采样值为1.0
    //2.2 bind texture to fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,texID,0);
    GLenum bufNameArr[]={GL_NONE};
    glDrawBuffers(1, bufNameArr);
    glReadBuffer(GL_NONE);
#else
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,g_shadowWidth,g_shadowHigh,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  //需要把边界及以外的地方，保证深度贴图的采样值为1.0
    //2.2 bind texture to fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,texID,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
#endif
    
    int status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status!=GL_FRAMEBUFFER_COMPLETE){
        fboStruct st={0,0,0};
        flylog("createFBOForDepth: glStatus is 0x%x!failed!！！！！！！！！！！！！！！---------------------",status);
        return st;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    fboStruct st={fbo,rbo,texID};
    return st;
}

fboStruct fbo::createFBOForDepthWithCubemap(){
    unsigned int fbo=0;
    unsigned int texID=0;
    unsigned int rbo=0;
    //1.create fbo
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    //2.1 create texture
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP,texID);
    unsigned int depthComponent=GL_DEPTH_COMPONENT;
    
#ifdef BUILD_IOS
    depthComponent=GL_DEPTH_COMPONENT32F;
#endif
    for(int i=0;i<6;i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,depthComponent,g_shadowWidth,g_shadowHigh,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,texID,0);
    
#ifdef BUILD_IOS
    GLenum bufNameArr[]={GL_NONE};
    glDrawBuffers(1, bufNameArr);
#else
    glDrawBuffer(GL_NONE);
#endif
    glReadBuffer(GL_NONE);
    
    int status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status!=GL_FRAMEBUFFER_COMPLETE){
        fboStruct st={0,0,0};
        flylog("createFBOForDepthWithCubemap: glStatus is 0x%x!failed!！！！！！！！！！！！！！！---------------------",status);
        return st;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    fboStruct st={fbo,rbo,texID};
    return st;
}
