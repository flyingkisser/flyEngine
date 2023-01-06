//
//  skybox.cpp
//  flyEngine
//
//  Created by joe on 22/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "skybox.h"
#include "textureMgr.h"
#include "logUtil.h"
#include "shaderMgr.h"
#include "state.h"
#include "world.h"
#include "stb_image/stb_image.h"
#include "camera.h"
#ifdef BUILD_IOS
#include "ios_dirUtil.h"
#endif

USE_NS_FLYENGINE

skybox::skybox(const char* imgFileName1,const char* imgFileName2,const char* imgFileName3,
               const char* imgFileName4,const char* imgFileName5,const char* imgFileName6){
    std::vector<const char*> vecFileName;
#ifdef BUILD_IOS
    imgFileName1=ios_dirUtil::getFileFullPathName(imgFileName1);
    imgFileName2=ios_dirUtil::getFileFullPathName(imgFileName2);
    imgFileName3=ios_dirUtil::getFileFullPathName(imgFileName3);
    imgFileName4=ios_dirUtil::getFileFullPathName(imgFileName4);
    imgFileName5=ios_dirUtil::getFileFullPathName(imgFileName5);
    imgFileName6=ios_dirUtil::getFileFullPathName(imgFileName6);
#endif
    
    vecFileName.push_back(imgFileName1);
    vecFileName.push_back(imgFileName2);
    vecFileName.push_back(imgFileName3);
    vecFileName.push_back(imgFileName4);
    vecFileName.push_back(imgFileName5);
    vecFileName.push_back(imgFileName6);
    
    for(auto fileName:vecFileName){
        struct_texture st=textureMgr::getInstance()->loadTexture(fileName,true);
        if(!st.width){
            flylog("skyBox:load %s failed!",fileName);
        }
        if(!_size.width){
            _size.width=st.width;
            _size.height=st.height;
            _format=st.format;
        }
        _vecBuf.push_back(st.buf);
    }
    init();
}

skybox::skybox(unsigned int texID,shader* sh){
    _texID=texID;
    setShader(sh);
    
//    int desc[]={3,2,3};
//    initVAO(g_verticeArrWithTexCoordAndNormal,sizeof(g_verticeArrWithTexCoordAndNormal),desc,3);
    
    int descArr[]={3};
    initVAO(g_verticeArr_skybox, sizeof(g_verticeArr_skybox),descArr,1);
}

skybox::skybox(unsigned int texID){
    int descArr[]={3};
    _texID=texID;
    setShader(shaderMgr::getShader("res/shader/skybox.vs", "res/shader/skybox.fs"));
    initVAO(g_verticeArr_skybox, sizeof(g_verticeArr_skybox),descArr,1);
//    world::getInstance()->getCamera()->setPositionZ(0);
}

skybox::~skybox(){}

void skybox::glInit(){
    int descArr[]={3};
    initVAO(g_verticeArr_skybox, sizeof(g_verticeArr_skybox),descArr,1);
    
    glGenTextures(1,&_texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP,_texID);
    int i=0;
    for(auto buf:_vecBuf){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,_format,_size.width,_size.height,0,_format,GL_UNSIGNED_BYTE,buf);
        i++;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

bool skybox::init(){
    setShader(shaderMgr::getShader("res/shader/skybox.vs", "res/shader/skybox.fs"));
    glInit();
//    world::getInstance()->getCamera()->setPositionZ(0);
    return true;
}

void skybox::draw(){
    _shaderObj->use();
    if(m_cb_before_draw_call!=nullptr)
        m_cb_before_draw_call(_shaderObj->getProgramID());
    
    //skybox的中心在0,0点，所以这里不用updateModel
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(_gl_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,_texID);
    glDrawArrays(GL_TRIANGLES,0,36);
    glDepthFunc(GL_LESS);   //恢复默认
    state::log(36);
}

void skybox::drawSimple(){
    glBindVertexArray(_gl_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,_texID);
    glDrawArrays(GL_TRIANGLES,0,36);
    state::log(36);
}


