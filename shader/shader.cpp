//
//  shader.cpp
//  flyEngine
//
//  Created by joe on 10/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "shader.h"
#include "fileUtil.h"
#include "logUtil.h"
#include "uboMgr.h"
#include "ssboMgr.h"
#include "stringUtil.h"
#ifdef BUILD_IOS
#include "ios_dirUtil.h"
#endif

using namespace flyEngine;

shader::shader(const char* szVertFileName,const char* szFragFileName){
    _idProgram=0;
#ifdef BUILD_MAC
    _szVertFileName=(char*)szVertFileName;
    _szFragFileName=(char*)szFragFileName;
#elif BUILD_IOS
    std::string strVertFullPath;
    std::string strFragFullPath;
    if(szVertFileName[0]!='/'){
        strVertFullPath=ios_dirUtil::getFileFullPathName(szVertFileName);
        _szVertFileName=(char*)strVertFullPath.c_str();
        strFragFullPath=ios_dirUtil::getFileFullPathName(szFragFileName);
        _szFragFileName=(char*)strFragFullPath.c_str();
    }else{
        _szVertFileName=(char*)szVertFileName;
        _szFragFileName=(char*)szFragFileName;
    }
#endif
    
    if(!readFile()){
        flylog("shader:init failed!");
        return;
    }
    compile();
    if(_idProgram<=0){
        flylog("shader::compile failed!");
        return;
    }
    
    uboMgr::linkUBOAndBindPoint(_idProgram,"mat3d", ubo_binding_mat_3d);
    uboMgr::linkUBOAndBindPoint(_idProgram,"mat2d", ubo_binding_mat_2d);

    uboMgr::linkUBOAndBindPoint(_idProgram,"light_dir0", ubo_binding_light_dir0);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_point0", ubo_binding_light_point0);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_point1", ubo_binding_light_point1);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_point2", ubo_binding_light_point2);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_point3", ubo_binding_light_point3);
    
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_spot0", ubo_binding_light_spot0);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_spot1", ubo_binding_light_spot1);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_spot2", ubo_binding_light_spot2);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_spot3", ubo_binding_light_spot3);

//  ssboMgr::linkSSBOAndBindPoint(_idProgram,"light_spot", ubo_binding_light_spot);
}

bool shader::readFile(){
    _idProgram=0;
    char* szVert=(char*)fileUtil::readFile(_szVertFileName);
    if(!szVert){
        return false;
    }
    char* szFrag=(char*)fileUtil::readFile(_szFragFileName);
    if(!szFrag){
        free(szVert);
        return false;
    }
    _szVert=szVert;
    _szFrag=szFrag;
    
#ifdef BUILD_IOS
    const char* buf=stringUtil::replace(szVert,"version 330","version 300");
    if(buf!=NULL){
        free(szVert);
        _szVert=(char*)buf;
    }
    buf=stringUtil::replace(szFrag,"version 330","version 300");
    if(buf!=NULL){
        free(szFrag);
        _szFrag=(char*)buf;
    }
#endif
    
    return true;
}

shader::~shader(){
    if(_szVert!=NULL){
        free(_szVert);
        _szVert=NULL;
    }
    if(_szFrag!=NULL){
        free(_szFrag);
        _szFrag=NULL;
    }
}

void shader::compile(){
    glRef::glInit();
    GLuint vertShader,fragShader,idProgram;
    GLint vertStatus,fragStatus,programStatus;
    vertShader=glCreateShader(GL_VERTEX_SHADER);
    fragShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertShader, 1, &_szVert, NULL);
    glShaderSource(fragShader, 1, &_szFrag, NULL);
    glCompileShader(vertShader);
    glCompileShader(fragShader);

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vertStatus);
    if(vertStatus!=GL_TRUE){
       int lenLog;
       GLsizei num;
       glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &lenLog);
       char* szLog=(char*)malloc(lenLog);
       glGetShaderInfoLog(vertShader,lenLog,&num,szLog);
       fprintf(stderr,"%s\nshader::shader: vertSahder error: %s",_szVertFileName,szLog);
       fprintf(stderr,_szVert);
       free(szLog);
       return;
    }

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragStatus);
    if(fragStatus!=GL_TRUE){
       int lenLog;
       GLsizei num;
       glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &lenLog);
       char* szLog=(char*)malloc(lenLog);
       glGetShaderInfoLog(fragShader,lenLog,&num,szLog);
       fprintf(stderr,"%s\nshader::shader: fragShader error: %s",_szFragFileName,szLog);
       fprintf(stderr,_szFrag);
       free(szLog);
       glDeleteShader(vertShader);
       return;
    }

    idProgram=glCreateProgram();
    glAttachShader(idProgram, vertShader);
    glAttachShader(idProgram, fragShader);
    glLinkProgram(idProgram);

    glGetProgramiv(idProgram, GL_LINK_STATUS, &programStatus);
    if(programStatus!=GL_TRUE){
       int lenLog;
       GLsizei num;
       glGetProgramiv(idProgram, GL_INFO_LOG_LENGTH, &lenLog);
       char* szLog=(char*)malloc(lenLog);
       glGetProgramInfoLog(idProgram,lenLog,&num,szLog);
       fprintf(stderr,"shader::shader: program link error: %s",szLog);
       free(szLog);
       glDeleteShader(vertShader);
       glDeleteShader(fragShader);
       return;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    _idProgram=idProgram;
}

void shader::use(){
    glUseProgram(_idProgram);
}

bool shader::isSuccess(){
    return (bool)_idProgram;
}

void shader::setBool(const char *name, bool v,bool debug){
    int pos=glGetUniformLocation(_idProgram, name);
    if(pos==-1){
        if(debug)
            flylog("shader::setBool cannot find %s",name);
        return;
    }
    glUniform1i(pos, (int)v);
}

void shader::setInt(const char *name, int v,bool debug){
    int pos=glGetUniformLocation(_idProgram, name);
    if(pos==-1){
        if(debug)
            flylog("shader::setInt cannot find %s",name);
       return;
    }
    glUniform1i(pos,v);
}

void shader::setFloat(const char *name, float v,bool debug){
    int pos=glGetUniformLocation(_idProgram, name);
    if(pos==-1){
        if(debug)
            flylog("shader::setFloat cannot find %s",name);
       return;
    }
    glUniform1f(pos,v);
}

void shader::setVec3(const char *name, float* v,bool debug){
    int pos=glGetUniformLocation(_idProgram, name);
    if(pos==-1){
        if(debug)
            flylog("shader::setVec3 cannot find %s",name);
       return;
    }
    glUniform3fv(pos,1,v);
}

void shader::setMat4(const char *name, float* v,bool debug){
    int pos=glGetUniformLocation(_idProgram, name);
    if(pos==-1){
        if(debug)
            flylog("shader::setMat4 cannot find %s",name);
       return;
    }
    glUniformMatrix4fv(pos,1,GL_FALSE,v);
}


