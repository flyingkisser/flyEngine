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
        flylog("uboMgr::set binding%d=%s",bindPoint,uboName);
//        flylog("shader::can't find interface block \"mat\" in this shader!");
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
    return ubo;
}

//写入数据
//指定ubo，描述变量的大小和内存位置的vector
void uboMgr::writeData(unsigned int ubo,int num,int sizeArr[],void* bufArr[],int firstOffset){
    glBindBuffer(GL_UNIFORM_BUFFER,ubo);
    int offset=firstOffset;
    for(int i=0;i<num;i++){
        int size=sizeArr[i];
        void* buf=bufArr[i];
        glBufferSubData(GL_UNIFORM_BUFFER,offset,size,buf);
        offset+=size;
    }
    glBindBuffer(GL_UNIFORM_BUFFER,0);
}
