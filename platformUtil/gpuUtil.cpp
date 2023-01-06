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

const char* gpuUtil::getGPUBrand(){
    return (const char*)glGetString(GL_RENDER);
}

const char* gpuUtil::getGPUVender(){
    return (const char*)glGetString(GL_VENDOR);
}

const char* gpuUtil::getGLVersion(){
    return (const char*)glGetString(GL_VERSION);
}

const char* gpuUtil::getGLSLVersion(){
    return (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}


const char* gpuUtil::getGPUSupportExtensions(){
    return (const char*)glGetString(GL_EXTENSIONS);
}

bool gpuUtil::isSupportETC1(){
    const char* extStrBuf=(const char*)glGetString(GL_EXTENSIONS);
    if(!extStrBuf)
        return false;
    std::string extStr=extStrBuf;
    return extStr.find("GL_OES_compressed_ETC1_RGB8_texture")!=std::string::npos;
    
}

bool gpuUtil::isSupportPVR(){
    const char* extStrBuf=(const char*)glGetString(GL_EXTENSIONS);
    if(!extStrBuf)
           return false;
       std::string extStr=extStrBuf;
    return extStr.find("GL_IMG_texture_compression_pvrtc")!=std::string::npos;
}

int gpuUtil::getMaxBinding(){
    int v=0;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS,&v);
    return v;
}


int gpuUtil::getMaxVSUniform(){
    int v=0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS,&v);
    return v;
}
int gpuUtil::getMaxFSUniform(){
    int v=0;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,&v);
    return v;
}

void  glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}


void gpuUtil::printGpuInfo(){
    fprintf(stdout,"gpu brand:%s\ngpu vender:%s\nGL version:%s\nGLSL version:%s\nsupport etc:%d\nsupport pvr:%d\nmax binding point %d\nmax vs uniform %d\nmax fs unifrom %d\ngpu extension:\n%s\n",
        gpuUtil::getGPUBrand(),
        gpuUtil::getGPUVender(),
        gpuUtil::getGLVersion(),
        gpuUtil::getGLSLVersion(),
        gpuUtil::isSupportETC1(),
        gpuUtil::isSupportPVR(),
        gpuUtil::getMaxBinding(),
        gpuUtil::getMaxVSUniform(),
        gpuUtil::getMaxFSUniform(),
        gpuUtil::getGPUSupportExtensions()
           );
    
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT){
        fprintf(stdout,"GL_CONTEXT_FLAG_DEBUG_BIT is enabled!");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    
}
