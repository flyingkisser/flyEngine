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

unsigned int g_ubo_id_mat_2d=0;
unsigned int g_ubo_id_mat_3d=0;
unsigned int g_ubo_id_arr[100]={0};

USE_NS_FLYENGINE

//uniform buffer object
void uboMgr::linkUBOAndBindPoint(int programID,const char* uboNameInShader,int bindPoint){
    int index=glGetUniformBlockIndex(programID,uboNameInShader);
    if(index>=0){
        glUniformBlockBinding(programID,index,bindPoint);
        flylog("uboMgr::linkUBOAndBindPoint [program %d] bind [block_index %d %s] at %d",programID,index,uboNameInShader,bindPoint);
        return;
    }else{
        flylog("uboMgr::linkUBOAndBindPoint [program %d] cannot find uniform block index of %s,ommit!",programID,uboNameInShader);
    }
}

//指定绑定点序号,ubo内存的大小，
int uboMgr::createUBO(int bindPoint,int uboSize,const char* uboNameOnlyForLog){
    unsigned int ubo;
    glGenBuffers(1,&ubo);
    glBindBuffer(GL_UNIFORM_BUFFER,ubo);
    glBufferData(GL_UNIFORM_BUFFER,uboSize,NULL,GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER,bindPoint,ubo,0,uboSize);
    checkGLError("createUBO glBindBufferRange");
    if(uboNameOnlyForLog==NULL)
       flylog("uboMgr::createUBO ubo id %d size %d bind at %d",ubo,uboSize,bindPoint);
    else
       flylog("uboMgr::createUBO ubo id %d size %d bind at %d %s",ubo,uboSize,bindPoint,uboNameOnlyForLog);
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
//    if(c)
//        flylog("uboMgr::writeData glBufferSubData %d times total size %d to ubo id %d",num,c,ubo);
}

void uboMgr::initAllUbo(){
    g_ubo_id_mat_2d=uboMgr::createUBO(ubo_binding_mat_2d,ubo_size_mat_2d,"mat2d");
    g_ubo_id_mat_3d=uboMgr::createUBO(ubo_binding_mat_3d,ubo_size_mat_3d,"mat3d");
    g_ubo_id_arr[ubo_binding_light_dir]=uboMgr::createUBO(ubo_binding_light_dir, ubo_size_light_dir_arr,"light_dir");
    g_ubo_id_arr[ubo_binding_light_point]=uboMgr::createUBO(ubo_binding_light_point, ubo_size_light_point_arr,"light_point");
    g_ubo_id_arr[ubo_binding_light_spot]=uboMgr::createUBO(ubo_binding_light_spot, ubo_size_light_spot_arr,"light_spot");
}
