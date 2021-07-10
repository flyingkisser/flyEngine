//
//  shader.cpp
//  flyEngine
//
//  Created by joe on 10/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "shader.h"
#include "fileUtil.h"

shader::shader(const char* szVertFileName,const char* szFragFileName){
    _idProgram=0;
    _szVertFileName=(char*)szVertFileName;
    _szFragFileName=(char*)szFragFileName;
}

bool shader::init(){
    _idProgram=0;

    char* szVert=(char*)readFile(_szVertFileName);
    if(!szVert){
        return false;
    }
    char* szFrag=(char*)readFile(_szFragFileName);
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

void shader::glInit(){
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
       fprintf(stderr,"shader::shader: vertSahder error: %s",szLog);
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
       fprintf(stderr,"shader::shader: fragShader error: %s",szLog);
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

void shader::setBool(const char *name, bool v){
    glUniform1i(glGetUniformLocation(_idProgram, name), (int)v);
}

void shader::setInt(const char *name, int v){
    glUniform1i(glGetUniformLocation(_idProgram, name),v);
}

void shader::setFloat(const char *name, float v){
    glUniform1f(glGetUniformLocation(_idProgram, name),v);
}
