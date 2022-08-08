//
//  uboMgr.cpp
//  flyEngine
//
//  Created by joe on 31/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "uboMgr.h"
#include "logUtil.h"
#include <iostream>
#include <variant>  //must use c++17

USE_NS_FLYENGINE

void uboMgr::linkUBOAndBindPoint(int programID,const char* uboName,int bindPoint){
    int index=glGetUniformBlockIndex(programID,uboName);
    if(index>=0){
        glUniformBlockBinding(programID,index,bindPoint);
        flylog("uboMgr::[program %d] bind [block_index %d %s] at %d",programID,index,uboName,bindPoint);
        return;
    }
}

//指定绑定点序号,ubo内存的大小，
int uboMgr::createUBO(int bindPointIndex,int uboSize){
    unsigned int ubo;
    glGenBuffers(1,&ubo);
    glBindBuffer(GL_UNIFORM_BUFFER,ubo);
    glBufferData(GL_UNIFORM_BUFFER,uboSize,NULL,GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER,bindPointIndex,ubo,0,uboSize);
    flylog("uboMgr::create ubo id %d size %d bind at %d",ubo,uboSize,bindPointIndex);
    return ubo;
}

//写入数据
//指定ubo，描述变量的大小和内存位置的vector
void uboMgr::writeData(unsigned int ubo,int num,int sizeArr[],int offsetArr[],void* bufArr[]){
    int c=0;
    glBindBuffer(GL_UNIFORM_BUFFER,ubo);
    for(int i=0;i<num;i++){
        int size=sizeArr[i];
        int offset=offsetArr[i];
        void* buf=bufArr[i];
        glBufferSubData(GL_UNIFORM_BUFFER,offset,size,buf);
        c+=size;
    }
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    if(c)
        flylog("uboMgr::call glBufferSubData %d times total size %d to ubo id %d",num,c,ubo);
}
