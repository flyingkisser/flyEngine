//
//  shaderMgr.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/27.
//  Copyright © 2021 joe. All rights reserved.
//
#include <map>
#include "shaderMgr.h"
#include "fileUtil.h"
#include "logUtil.h"


USE_NS_FLYENGINE

static std::map<std::string,shader*> s_mapShaderCache;
static std::map<int,shader*> s_mapShaderCacheByProgram;

shader* shaderMgr::createAndCacheShader(const char* szVertFileName,const char* szFragFileName){
    std::string key=std::string(szVertFileName)+'_'+std::string(szFragFileName);
    shader* shaderObj=new shader(szVertFileName,szFragFileName);
    int programID=shaderObj->getProgramID();
    s_mapShaderCache[key]=shaderObj;
    s_mapShaderCacheByProgram[programID]=shaderObj;
    flylog("programID %d from %s %s",programID,szVertFileName,szFragFileName);
    return shaderObj;
}

shader* shaderMgr::getShader(const char* szVertFileName,const char* szFragFileName){
    std::string key=std::string(szVertFileName)+'_'+std::string(szFragFileName);
    auto it=s_mapShaderCache.find(key);
    if(it!=s_mapShaderCache.end())
        return it->second;
    return shaderMgr::createAndCacheShader(szVertFileName,szFragFileName);
}

shader* shaderMgr::getShader(int programID){
    auto it=s_mapShaderCacheByProgram.find(programID);
    if(it!=s_mapShaderCacheByProgram.end())
        return it->second;
    return NULL;
}

shader* shaderMgr::getShaderUniqueue(const char* szVertFileName,const char* szFragFileName){
    shader* shaderObj=new shader(szVertFileName,szFragFileName);
//    if(!shaderObj->init())
//        return NULL;
    return shaderObj;
}

void shaderMgr::initDefaultShader(){
//    shader* s=getDefaultShader();
//    s->glInit();
//    s=get3d1texShader();
//    s->glInit();
//    s=get3d2texShader();
//    s->glInit();
}

//void shaderMgr::useDefaultShader(){
//    shader* s=getDefaultShader();
//    s->use();
//}

shader* shaderMgr::getDefaultShader(){
    return shaderMgr::getShaderUniqueue("./res/shader/default.vs","./res/shader/default.fs");
}

shader* shaderMgr::get3dColorShader(){
    return shaderMgr::getShader("./res/shader/3d_color.vs","./res/shader/3d_color.fs");
}
shader* shaderMgr::get3d1texShader(){
    return shaderMgr::getShader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex.fs");
}
shader* shaderMgr::get3d1texPongShader(){
    return shaderMgr::getShader("./res/shader/3d_1tex_phong.vs","./res/shader/3d_1tex_phong.fs");
}
shader* shaderMgr::get3d1texPongWithSpecularTexShader(){
    return shaderMgr::getShader("./res/shader/3d_1tex_phong.vs","./res/shader/3d_1tex_phong_specularTex.fs");
}

shader* shaderMgr::get3d2texShader(){
    return shaderMgr::getShaderUniqueue("./res/shader/3d_2tex.vs","./res/shader/3d_2tex.fs");
}

shader* shaderMgr::getModelShader(){
    return shaderMgr::getShaderUniqueue("./res/shader/3d_model.vs","./res/shader/3d_model.fs");
}


unsigned int shaderMgr::createShaderFromFile(const char* szVertFileName,const char* szFragFileName){
    GLuint vertShader,fragShader,idProgram=0;
    GLint vertStatus,fragStatus,programStatus;
    GLsizei logNumWrite;
    int lenLog;
    char szLog[512]={0};

    char* szVert=(char*)fileUtil::readFile(szVertFileName);
    if(!szVert){
        return idProgram;
    }
    char* szFrag=(char*)fileUtil::readFile(szFragFileName);
    if(!szFrag){
        free(szVert);
        return idProgram;
    }
    vertShader=glCreateShader(GL_VERTEX_SHADER);
    fragShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertShader, 1, &szVert, NULL);
    glShaderSource(fragShader, 1, &szFrag, NULL);

    glCompileShader(vertShader);
    glCompileShader(fragShader);

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vertStatus);
    if(vertStatus!=GL_TRUE){
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &lenLog);
        glGetShaderInfoLog(vertShader,lenLog,&logNumWrite,szLog);
        fprintf(stderr,"shader::shader: vertSahder error: %s",szLog);
        return idProgram;
    }

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragStatus);
    if(fragStatus!=GL_TRUE){
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &lenLog);
        glGetShaderInfoLog(fragShader,lenLog,&logNumWrite,szLog);
        fprintf(stderr,"shader::shader: fragShader error: %s",szLog);
        glDeleteShader(vertShader);
        return idProgram;
    }

    idProgram=glCreateProgram();
    glAttachShader(idProgram, vertShader);
    glAttachShader(idProgram, fragShader);
    glLinkProgram(idProgram);

    glGetProgramiv(idProgram, GL_LINK_STATUS, &programStatus);
    if(programStatus!=GL_TRUE){
        glGetProgramiv(idProgram, GL_INFO_LOG_LENGTH, &lenLog);
        glGetProgramInfoLog(idProgram,lenLog,&logNumWrite,szLog);
        fprintf(stderr,"shader::shader: program link error: %s",szLog);
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        return idProgram;
    }
    glUseProgram(idProgram);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return idProgram;
}

void shaderMgr::setBool(unsigned int idProgram,const char *name, bool v){
    int pos=glGetUniformLocation(idProgram, name);
    if(pos==-1){
      flylog("shaderMgr::setBool cannot find %s",name);
      return;
    }
    glUniform1i(pos, (int)v);
}

void shaderMgr::setInt(unsigned int idProgram,const char *name, int v){
    int pos=glGetUniformLocation(idProgram, name);
    if(pos==-1){
      flylog("shaderMgr::setInt cannot find %s",name);
      return;
    }
    glUniform1i(pos,v);
}

void shaderMgr::setFloat(unsigned int idProgram,const char *name, float v){
    int pos=glGetUniformLocation(idProgram, name);
    if(pos==-1){
      flylog("shaderMgr::setFloat cannot find %s",name);
      return;
    }
    glUniform1f(pos,v);
}

void shaderMgr::setMat4(unsigned int idProgram,const char *name, float* v){
    int pos=glGetUniformLocation(idProgram, name);
    if(pos==-1){
      flylog("shaderMgr::setMat4 cannot find %s",name);
      return;
    }
    glUniformMatrix4fv(pos,1,GL_FALSE,v);
}

void shaderMgr::setVec3(unsigned int idProgram,const char *name, float* v){
    int pos=glGetUniformLocation(idProgram, name);
    if(pos==-1){
        flylog("shaderMgr::setVec3 cannot find %s",name);
        return;
    }
    glUniform3fv(pos,1,v);
}
