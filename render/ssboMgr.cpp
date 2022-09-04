//
//  ssboMgr.cpp
//  flyEngine
//
//  Created by joe on 31/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "ssboMgr.h"
#include "logUtil.h"
#include <iostream>
#include <variant>  //must use c++17

USE_NS_FLYENGINE

#ifdef BUILD_MAC
//only works on openGL 4.30+
void ssboMgr::linkSSBOAndBindPoint(int programID,const char* ssboName,int bindPoint){
    int index=glGetProgramResourceIndex(programID,GL_SHADER_STORAGE_BLOCK,ssboName);
    if(index>=0){
        glShaderStorageBlockBinding(programID,index,bindPoint);
        flylog("ssboMgr::linkssboAndBindPoint [program %d] bind [block_index %d %s] at %d",programID,index,ssboName,bindPoint);
        return;
    }
}

//指定绑定点序号,ssbo内存的大小，
int ssboMgr::createSSBO(int bindPointIndex,int ssboSize,const char* ssboName){
    unsigned int ssbo;
    glGenBuffers(1,&ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER,ssboSize,NULL,GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER,bindPointIndex,ssbo,0,ssboSize);
    if(ssboName==NULL)
        flylog("ssboMgr::createssbo ssbo id %d size %d bind at %d",ssbo,ssboSize,bindPointIndex);
    else
        flylog("ssboMgr::createssbo ssbo id %d size %d bind at %d %s",ssbo,ssboSize,bindPointIndex,ssboName);
    return ssbo;
}

//写入数据
//指定ssbo，描述变量的大小和内存位置的vector
void ssboMgr::writeData(unsigned int ssbo,int size,void *buf){
    int c=0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo);
    void* mapBuf=glMapBuffer(GL_SHADER_STORAGE_BUFFER,GL_WRITE_ONLY);
    memcpy(mapBuf,buf,size);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
    if(c)
        flylog("ssboMgr::writeData %d bytes to ssbo id %d",size,ssbo);
}
#endif
