//
//  shaderMgr.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/27.
//  Copyright © 2021 joe. All rights reserved.
//

#include "shaderMgr.h"
#include "fileUtil.h"

static unsigned int s_default_shader_id=0;
static unsigned int s_3d_1tex_shader_id=0;
static unsigned int s_3d_2tex_shader_id=0;
static std::map<std::string,unsigned int> s_programIDMap;

unsigned int shaderMgr::createShaderWithoutCache(const char* szVertFileName,const char* szFragFileName){
    GLuint vertShader,fragShader,idProgram=0;
    GLint vertStatus,fragStatus,programStatus;
    GLsizei logNumWrite;
    int lenLog;
    char szLog[512]={0};
    
    char* szVert=(char*)readFile(szVertFileName);
    if(!szVert){
        return idProgram;
    }
    char* szFrag=(char*)readFile(szFragFileName);
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

unsigned int shaderMgr::createShader(const char* szVertFileName,const char* szFragFileName){
    GLuint idProgram=0;

    if(s_programIDMap[szVertFileName]!=0)
        return s_programIDMap[szVertFileName];

    idProgram=shaderMgr::createShaderWithoutCache(szVertFileName,szFragFileName);
    if(idProgram==0)
        return 0;
    s_programIDMap[szVertFileName]=idProgram;
    
    return idProgram;
}

void shaderMgr::useDefaultShader(){
    if(!s_default_shader_id){
        s_default_shader_id=shaderMgr::createShader("./res/shader/default.vs","./res/shader/default.fs");
        if(!s_default_shader_id)
            return;
    }
    shaderMgr::useShader(s_default_shader_id);
}

unsigned int shaderMgr::getDefaultShader(){
    if(!s_default_shader_id)
           s_default_shader_id=shaderMgr::createShader("./res/shader/default.vs","./res/shader/default.fs");
    return s_default_shader_id;
}

unsigned int shaderMgr::get3d1texShader(){
    if(!s_3d_1tex_shader_id)
           s_3d_1tex_shader_id=shaderMgr::createShader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex.fs");
    return s_3d_1tex_shader_id;
}
unsigned int shaderMgr::get3d2texShader(){
    if(!s_3d_2tex_shader_id)
           s_3d_2tex_shader_id=shaderMgr::createShader("./res/shader/3d_2tex.vs","./res/shader/3d_2tex.fs");
    return s_3d_2tex_shader_id;
}

void shaderMgr::useShader(unsigned int idProgram){
    glUseProgram(idProgram);
}

void shaderMgr::setBool(unsigned int idProgram,const char *name, bool v){
    glUniform1i(glGetUniformLocation(idProgram, name), (int)v);
}

void shaderMgr::setInt(unsigned int idProgram,const char *name, int v){
    glUniform1i(glGetUniformLocation(idProgram, name),v);
}

void shaderMgr::setFloat(unsigned int idProgram,const char *name, float v){
    glUniform1f(glGetUniformLocation(idProgram, name),v);
}
