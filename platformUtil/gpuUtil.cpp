//
//  gpuUtil.cpp
//  flyEngine
//
//  Created by joe on 31/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "gpuUtil.h"
#include "defines.h"
#include <string>

const char* getGPUBrand(){
    GLubyte* b=(GLubyte*)glGetString(GL_RENDER);
    return (const char*)glGetString(GL_RENDER);
}

const char* getGPUVender(){
    return (const char*)glGetString(GL_VENDOR);
}

const char* getGLVersion(){
    return (const char*)glGetString(GL_VERSION);
}

const char* getGLSLVersion(){
    return (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}


const char* getGPUSupportExtensions(){
    return (const char*)glGetString(GL_EXTENSIONS);
}

bool isSupportETC1(){
    const char* extStrBuf=(const char*)glGetString(GL_EXTENSIONS);
    if(!extStrBuf)
        return false;
    std::string extStr=extStrBuf;
    return extStr.find("GL_OES_compressed_ETC1_RGB8_texture")!=std::string::npos;
    
}

bool isSupportPVR(){
    const char* extStrBuf=(const char*)glGetString(GL_EXTENSIONS);
    if(!extStrBuf)
           return false;
       std::string extStr=extStrBuf;
    return extStr.find("GL_IMG_texture_compression_pvrtc")!=std::string::npos;
}

