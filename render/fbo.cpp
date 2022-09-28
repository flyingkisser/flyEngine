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

USE_NS_FLYENGINE
fboStruct fbo::createFBO(){
    unsigned int fbo;
    unsigned int texID;
    unsigned int rbo;
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
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
        fboStruct st={0,0,0};
        return st;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    fboStruct st={fbo,rbo,texID};
    return st;
}
