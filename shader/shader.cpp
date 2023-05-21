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

shader::shader(const char* szVertFileName,const char* szFragFileName,const char* szGeometryFileName,
               const char* szTessControlFileName,const char* szTessEvalFileName){
    _idProgram=0;
#ifdef BUILD_MAC
    _szVertFileName=(char*)szVertFileName;
    _szFragFileName=(char*)szFragFileName;
    _szGeometryFileName=(char*)szGeometryFileName;
    _szTessControlFileName=(char*)szTessControlFileName;
    _szTessEvalFileName=(char*)szTessEvalFileName;
#elif BUILD_IOS
    std::string strVertFullPath;
    std::string strFragFullPath;
    std::string strGeoFullPath;
    std::string strTessControlFullPath;
    std::string strTessEvalFullPath;
    if(szVertFileName[0]!='/'){
        strVertFullPath=ios_dirUtil::getFileFullPathName(szVertFileName);
        _szVertFileName=(char*)strVertFullPath.c_str();
        strFragFullPath=ios_dirUtil::getFileFullPathName(szFragFileName);
        _szFragFileName=(char*)strFragFullPath.c_str();
        if(szGeometryFileName!=NULL){
            strGeoFullPath=ios_dirUtil::getFileFullPathName(szGeometryFileName);
            _szGeometryFileName=(char*)strGeoFullPath.c_str();
        }
        if(szTessControlFileName!=NULL){
            strTessControlFullPath=ios_dirUtil::getFileFullPathName(szTessControlFileName);
            _szTessControlFileName=(char*)strTessControlFullPath.c_str();
        }
        if(szTessEvalFileName!=NULL){
            strTessEvalFullPath=ios_dirUtil::getFileFullPathName(szTessEvalFileName);
            _szTessEvalFileName=(char*)strTessEvalFullPath.c_str();
        }
    }else{
        _szVertFileName=(char*)szVertFileName;
        _szFragFileName=(char*)szFragFileName;
        _szGeometryFileName=(char*)szGeometryFileName;
        _szGeometryFileName=(char*)szTessControlFileName;
        _szTessEvalFileName=(char*)szTessEvalFileName;
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
    flylog("programID %d %s %s",_idProgram,stringUtil::getStrAfterFromLast(std::string(_szVertFileName), "/").c_str(),
           stringUtil::getStrAfterFromLast(std::string(_szFragFileName), "/").c_str());
    
    //setup inital values
    uboMgr::linkUBOAndBindPoint(_idProgram,"mat3d", ubo_binding_mat_3d);
    uboMgr::linkUBOAndBindPoint(_idProgram,"mat3d_shadow", ubo_binding_mat_3d_shadow);
    uboMgr::linkUBOAndBindPoint(_idProgram,"mat2d", ubo_binding_mat_2d);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_dir", ubo_binding_light_dir);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_point", ubo_binding_light_point);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_spot", ubo_binding_light_spot);
    uboMgr::linkUBOAndBindPoint(_idProgram,"light_area", ubo_binding_light_area);
    use();
    setInt("texture_shadow", texture_shadow);
    setInt("texture_depth_cube", texture_depth_cube);

//  ssboMgr::linkSSBOAndBindPoint(_idProgram,"light_spot", ubo_binding_light_spot);
}

bool shader::bindGeometry(const char* szGeometryFileName){
#if GL_VER > 320
    
#ifdef BUILD_MAC
    _szGeometryFileName=(char*)szGeometryFileName;
#elif BUILD_IOS
    std::string strGeoFullPath;
    if(szGeometryFileName[0]!='/'){
        strGeoFullPath=ios_dirUtil::getFileFullPathName(szGeometryFileName);
        _szGeometryFileName=(char*)strGeoFullPath.c_str();
    }else{
        _szGeometryFileName=(char*)szGeometryFileName;
    }
#endif
    char* szGeo=(char*)fileUtil::readFile(_szGeometryFileName);
    _szGeo=szGeo;
    if(!_szGeo)
        return false;
#ifdef BUILD_IOS
    const char* buf=stringUtil::replace(_szGeo,"version 330","version 300");
    if(buf!=NULL){
        free(_szGeo);
        _szGeo=(char*)buf;
    }
#endif
    GLuint geoShader;
    GLint  gsStatus,programStatus;
    geoShader=glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geoShader, 1, &_szGeo, NULL);
    glCompileShader(geoShader);
    glGetShaderiv(geoShader, GL_COMPILE_STATUS, &gsStatus);
    if(gsStatus!=GL_TRUE){
       int lenLog;
       GLsizei num;
       glGetShaderiv(geoShader, GL_INFO_LOG_LENGTH, &lenLog);
       char* szLog=(char*)malloc(lenLog);
       glGetShaderInfoLog(geoShader,lenLog,&num,szLog);
       fprintf(stderr,"%s\nshader::shader: gsShader error: %s",_szGeometryFileName,szLog);
       fprintf(stderr,_szGeo);
       free(szLog);
       return false;
    }
    
    glAttachShader(_idProgram, geoShader);
    glLinkProgram(_idProgram);
    
    glGetProgramiv(_idProgram, GL_LINK_STATUS, &programStatus);
    if(programStatus!=GL_TRUE){
       int lenLog;
       GLsizei num;
       glGetProgramiv(_idProgram, GL_INFO_LOG_LENGTH, &lenLog);
       char* szLog=(char*)malloc(lenLog);
       glGetProgramInfoLog(_idProgram,lenLog,&num,szLog);
       fprintf(stderr,"shader::shader: program link error: %s",szLog);
       fprintf(stderr, _szVert);
       fprintf(stderr, _szFrag);
       free(szLog);
       return false;
    }
    glDeleteShader(geoShader);
    return true;
#else
    return false;
#endif
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
    if(_szGeometryFileName!=NULL){
        char* szGeo=(char*)fileUtil::readFile(_szGeometryFileName);
        _szGeo=szGeo;
    }
    if(_szTessControlFileName!=NULL){
        char* sz=(char*)fileUtil::readFile(_szTessControlFileName);
        _szTessControl=sz;
    }
    if(_szTessEvalFileName!=NULL){
        char* sz=(char*)fileUtil::readFile(_szTessEvalFileName);
        _szTessEval=sz;
    }
   
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
    if(_szGeo){
        buf=stringUtil::replace(_szGeo,"version 330","version 300");
        if(buf!=NULL){
            free(_szGeo);
            _szGeo=(char*)buf;
        }
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
    if(_szGeo!=NULL){
        free(_szGeo);
        _szGeo=NULL;
    }
    if(_szTessControl!=NULL){
        free(_szTessControl);
        _szTessControl=NULL;
    }
    if(_szTessEval!=NULL){
        free(_szTessEval);
        _szTessEval=NULL;
    }
}

void shader::compile(){
    glRef::glInit();
    GLuint vertShader=0,fragShader=0,geoShader=0,tessControlShader=0,tessEvalShader=0,idProgram=0;
    GLint vertStatus,fragStatus,gsStatus,tess1Status,tess2Status,programStatus;
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


    if(_szGeo){
        geoShader=glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geoShader, 1, &_szGeo, NULL);
        glCompileShader(geoShader);
        glGetShaderiv(geoShader, GL_COMPILE_STATUS, &gsStatus);
        if(gsStatus!=GL_TRUE){
           int lenLog;
           GLsizei num;
           glGetShaderiv(geoShader, GL_INFO_LOG_LENGTH, &lenLog);
           char* szLog=(char*)malloc(lenLog);
           glGetShaderInfoLog(geoShader,lenLog,&num,szLog);
           fprintf(stderr,"%s\nshader::shader: gsShader error: %s",_szGeometryFileName,szLog);
           fprintf(stderr,_szGeo);
           free(szLog);
           glDeleteShader(vertShader);
           glDeleteShader(fragShader);
           return;
        }
        glAttachShader(idProgram, geoShader);
    }

    flylog("gl_ver %d",GL_VER);
    

    if(_szTessControl){
        tessControlShader=glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tessControlShader, 1, &_szTessControl, NULL);
        glCompileShader(tessControlShader);
        glGetShaderiv(tessControlShader, GL_COMPILE_STATUS, &tess1Status);
        if(tess1Status!=GL_TRUE){
            int lenLog;
            GLsizei num;
            glGetShaderiv(tessControlShader, GL_INFO_LOG_LENGTH, &lenLog);
            char* szLog=(char*)malloc(lenLog);
            glGetShaderInfoLog(tessControlShader,lenLog,&num,szLog);
            fprintf(stderr,"%s\nshader::shader: tessControlShader error: %s",_szTessControlFileName,szLog);
            fprintf(stderr,_szTessControl);
            free(szLog);
            glDeleteShader(vertShader);
            glDeleteShader(fragShader);
            return;
        }
        glAttachShader(idProgram, tessControlShader);
    }
    if(_szTessEval){
        tessEvalShader=glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tessEvalShader, 1, &_szTessEval, NULL);
        glCompileShader(tessEvalShader);
        glGetShaderiv(tessEvalShader, GL_COMPILE_STATUS, &tess2Status);
        if(tess2Status!=GL_TRUE){
            int lenLog;
            GLsizei num;
            glGetShaderiv(tessEvalShader, GL_INFO_LOG_LENGTH, &lenLog);
            char* szLog=(char*)malloc(lenLog);
            glGetShaderInfoLog(tessEvalShader,lenLog,&num,szLog);
            fprintf(stderr,"%s\nshader::shader: tessEvalShader error: %s",_szTessEvalFileName,szLog);
            fprintf(stderr,_szTessEval);
            free(szLog);
            glDeleteShader(vertShader);
            glDeleteShader(fragShader);
            return;
        }
        glAttachShader(idProgram, tessEvalShader);
    }


    glLinkProgram(idProgram);

    glGetProgramiv(idProgram, GL_LINK_STATUS, &programStatus);
    if(programStatus!=GL_TRUE){
       int lenLog;
       GLsizei num;
       glGetProgramiv(idProgram, GL_INFO_LOG_LENGTH, &lenLog);
       char* szLog=(char*)malloc(lenLog);
       glGetProgramInfoLog(idProgram,lenLog,&num,szLog);
       fprintf(stderr,"shader::shader: program link error: %s",szLog);
       fprintf(stderr, _szVertFileName);
       fprintf(stderr, _szFragFileName);
       free(szLog);
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    if(geoShader)
        glDeleteShader(geoShader);
    if(tessControlShader)
        glDeleteShader(tessControlShader);
    if(tessEvalShader)
        glDeleteShader(tessEvalShader);
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
void shader::setBool(std::string name, bool v,bool debug){
    setBool(name.c_str(),v,debug);
}
bool shader::getBool(const char* name,bool debug){
    unsigned int value[16]={0};
    int pos=glGetUniformLocation(_idProgram, name);
    if(pos==-1){
        if(debug)
            flylog("shader::getBool cannot find %s",name);
        return false;
    }
    glGetUniformuiv(_idProgram,pos,value);
    return (bool)value[0];
}
float shader::getFloat(const char* name,bool debug){
    float value[16]={0};
    int pos=glGetUniformLocation(_idProgram, name);
    if(pos==-1){
        if(debug)
            flylog("shader::getFloat cannot find %s",name);
        return false;
    }
    glGetUniformfv(_idProgram,pos,value);
    return value[0];
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
void shader::setInt(std::string name, int v,bool debug){
    setInt(name.c_str(),v,debug);
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
void shader::setFloat(std::string name, float v,bool debug){
    setFloat(name.c_str(),v,debug);
}

void shader::setVec2(const char *name, float* v,bool debug){
    int pos=glGetUniformLocation(_idProgram, name);
    if(pos==-1){
        if(debug)
            flylog("shader::setVec2 cannot find %s",name);
       return;
    }
    glUniform2fv(pos,1,v);
}
void shader::setVec2(const char *name, glm::vec2 vector2,bool debug){
    return setVec2(name,glm::value_ptr(vector2),debug);
}
void shader::setVec2(const char *name, float v1,float v2,bool debug){
    return setVec2(name,(float*)glm::value_ptr(glm::vec2(v1,v2)),debug);
}
void shader::setVec2(std::string name, glm::vec2 vector2,bool debug){
    return setVec2(name.c_str(),glm::value_ptr(vector2),debug);
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
void shader::setVec3(const char *name, glm::vec3 vector3,bool debug){
    return setVec3(name,glm::value_ptr(vector3),debug);
}
void shader::setVec3(const char *name, float v1,float v2,float v3,bool debug){
    return setVec3(name,(float*)glm::value_ptr(glm::vec3(v1,v2,v3)),debug);
}
void shader::setVec3(std::string name, glm::vec3 vector3,bool debug){
    return setVec3(name.c_str(),glm::value_ptr(vector3),debug);
}

void shader::setVec4(const char *name, float* v,bool debug){
    int pos=glGetUniformLocation(_idProgram, name);
    if(pos==-1){
        if(debug)
            flylog("shader::setVec3 cannot find %s",name);
        return;
    }
    glUniform4fv(pos,1,v);
}
void shader::setVec4(const char *name, glm::vec4 vector4,bool debug){
    return setVec4(name,glm::value_ptr(vector4),debug);
}
void shader::setVec4(const char *name, float v1,float v2,float v3,float v4,bool debug){
    return setVec4(name,(float*)glm::value_ptr(glm::vec4(v1,v2,v3,v4)),debug);
}
void shader::setVec4(std::string name, glm::vec4 vector4,bool debug){
    return setVec4(name.c_str(),glm::value_ptr(vector4),debug);
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
void shader::setMat4(const char *name, glm::mat4 v,bool debug){
    return setMat4(name,glm::value_ptr(v),debug);
}
void shader::setMat4(std::string name, glm::mat4 v,bool debug){
    return setMat4(name.c_str(),glm::value_ptr(v),debug);
}

void shader::setMat4Multi(const char *name, float* v,int count,bool debug){
    int pos=glGetUniformLocation(_idProgram, name);
    if(pos==-1){
        if(debug)
            flylog("shader::setMat4 cannot find %s",name);
       return;
    }
    glUniformMatrix4fv(pos,count,GL_FALSE,v);
}

