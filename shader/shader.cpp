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

using namespace flyEngine;

shader::shader(const char* szVertFileName,const char* szFragFileName){
    _idProgram=0;
    _szVertFileName=(char*)szVertFileName;
    _szFragFileName=(char*)szFragFileName;
    if(!readFile()){
        flylog("shader:init failed!");
        return;
    }
    compile();
    if(_idProgram<=0){
        flylog("shader::compile failed!");
        return;
    }
    uboMgr::linkUBOAndBindPoint(_idProgram,"mat", ubo_binding_mat);
    uboMgr::linkUBOAndBindPoint(_idProgram,"mat2d", ubo_binding_mat_2d);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_dir", ubo_binding_light_dir);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_point", ubo_binding_light_point);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_spot", ubo_binding_light_spot);
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


