//
//  glslUtil.cpp
//  flyEngine
//
//  Created by joe on 02/08/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "glslUtil.h"
#include "logUtil.h"
#include <map>

static std::map<int,int> glslTypeSizeMap;

void glslUtil::init(){
    glslTypeSizeMap[GL_FLOAT] = sizeof(float);
    glslTypeSizeMap[GL_FLOAT_VEC2] = sizeof(float)*2;
    glslTypeSizeMap[GL_FLOAT_VEC3] = sizeof(float)*3;
    glslTypeSizeMap[GL_FLOAT_VEC4] = sizeof(float)*4;
    
    glslTypeSizeMap[GL_DOUBLE] = sizeof(double);
    glslTypeSizeMap[GL_DOUBLE_VEC2] = sizeof(double)*2;
    glslTypeSizeMap[GL_DOUBLE_VEC3] = sizeof(double)*3;
    glslTypeSizeMap[GL_DOUBLE_VEC4] = sizeof(double)*4;
    
    glslTypeSizeMap[GL_SAMPLER_1D] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_2D] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_3D] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_CUBE] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_1D_SHADOW] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_2D_SHADOW] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_1D_ARRAY] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_2D_ARRAY] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_1D_ARRAY_SHADOW] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_2D_ARRAY_SHADOW] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_2D_MULTISAMPLE] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_2D_MULTISAMPLE_ARRAY] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_CUBE_SHADOW] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_BUFFER] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_2D_RECT] = sizeof(int);
    glslTypeSizeMap[GL_SAMPLER_2D_RECT_SHADOW] = sizeof(int);
    glslTypeSizeMap[GL_INT_SAMPLER_1D] = sizeof(int);
    glslTypeSizeMap[GL_INT_SAMPLER_2D] = sizeof(int);
    glslTypeSizeMap[GL_INT_SAMPLER_3D] = sizeof(int);
    glslTypeSizeMap[GL_INT_SAMPLER_CUBE] = sizeof(int);
    glslTypeSizeMap[GL_INT_SAMPLER_1D_ARRAY] = sizeof(int);
    glslTypeSizeMap[GL_INT_SAMPLER_2D_ARRAY] = sizeof(int);
    glslTypeSizeMap[GL_INT_SAMPLER_2D_MULTISAMPLE] = sizeof(int);
    glslTypeSizeMap[GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY] = sizeof(int);
    glslTypeSizeMap[GL_INT_SAMPLER_BUFFER] = sizeof(int);
    glslTypeSizeMap[GL_INT_SAMPLER_2D_RECT] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_SAMPLER_1D] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_SAMPLER_2D] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_SAMPLER_3D] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_SAMPLER_CUBE] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_SAMPLER_1D_ARRAY] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_SAMPLER_2D_ARRAY] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_SAMPLER_BUFFER] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_SAMPLER_2D_RECT] = sizeof(int);
    glslTypeSizeMap[GL_BOOL] = sizeof(int);
    glslTypeSizeMap[GL_INT] = sizeof(int);
    glslTypeSizeMap[GL_BOOL_VEC2] = sizeof(int)*2;
    glslTypeSizeMap[GL_INT_VEC2] = sizeof(int)*2;
    glslTypeSizeMap[GL_BOOL_VEC3] = sizeof(int)*3;
    glslTypeSizeMap[GL_INT_VEC3] = sizeof(int)*3;
    glslTypeSizeMap[GL_BOOL_VEC4] = sizeof(int)*4;
    glslTypeSizeMap[GL_INT_VEC4] = sizeof(int)*4;
    
    glslTypeSizeMap[GL_UNSIGNED_INT] = sizeof(int);
    glslTypeSizeMap[GL_UNSIGNED_INT_VEC2] = sizeof(int)*2;
    glslTypeSizeMap[GL_UNSIGNED_INT_VEC3] = sizeof(int)*2;
    glslTypeSizeMap[GL_UNSIGNED_INT_VEC4] = sizeof(int)*2;
    
    glslTypeSizeMap[GL_FLOAT_MAT2] = sizeof(float)*4;
    glslTypeSizeMap[GL_FLOAT_MAT3] = sizeof(float)*9;
    glslTypeSizeMap[GL_FLOAT_MAT4] = sizeof(float)*16;
    glslTypeSizeMap[GL_FLOAT_MAT2x3] = sizeof(float)*6;
    glslTypeSizeMap[GL_FLOAT_MAT2x4] = sizeof(float)*8;
    glslTypeSizeMap[GL_FLOAT_MAT3x2] = sizeof(float)*6;
    glslTypeSizeMap[GL_FLOAT_MAT3x4] = sizeof(float)*12;
    glslTypeSizeMap[GL_FLOAT_MAT4x2] = sizeof(float)*8;
    glslTypeSizeMap[GL_FLOAT_MAT4x3] = sizeof(float)*12;
    glslTypeSizeMap[GL_DOUBLE_MAT2] = sizeof(double)*4;
    glslTypeSizeMap[GL_DOUBLE_MAT3] = sizeof(double)*9;
    glslTypeSizeMap[GL_DOUBLE_MAT4] = sizeof(double)*16;
    glslTypeSizeMap[GL_DOUBLE_MAT2x3] = sizeof(double)*6;
    glslTypeSizeMap[GL_DOUBLE_MAT2x4] = sizeof(double)*8;
    glslTypeSizeMap[GL_DOUBLE_MAT3x2] = sizeof(double)*6;
    glslTypeSizeMap[GL_DOUBLE_MAT3x4] = sizeof(double)*12;
    glslTypeSizeMap[GL_DOUBLE_MAT4x2] = sizeof(double)*8;
    glslTypeSizeMap[GL_DOUBLE_MAT4x3] = sizeof(double)*12;
}

// gets the number of rows for a GLSL type
int glslUtil::_getRows(int type) {
    switch(type) {
        case GL_DOUBLE_MAT2:
        case GL_DOUBLE_MAT2x3:
        case GL_DOUBLE_MAT2x4:
        case GL_FLOAT_MAT2:
        case GL_FLOAT_MAT2x3:
        case GL_FLOAT_MAT2x4:
            return 2;
        case GL_DOUBLE_MAT3:
        case GL_DOUBLE_MAT3x2:
        case GL_DOUBLE_MAT3x4:
        case GL_FLOAT_MAT3:
        case GL_FLOAT_MAT3x2:
        case GL_FLOAT_MAT3x4:
            return 3;
        case GL_DOUBLE_MAT4:
        case GL_DOUBLE_MAT4x2:
        case GL_DOUBLE_MAT4x3:
        case GL_FLOAT_MAT4:
        case GL_FLOAT_MAT4x2:
        case GL_FLOAT_MAT4x3:
            return 4;
        default: return 1;
    }
}

// gets the number of columns for a GLSL type
int glslUtil::_getColumns(int type) {
    switch(type) {
        case GL_DOUBLE_MAT2:
        case GL_FLOAT_MAT2:
        case GL_DOUBLE_MAT3x2:
        case GL_FLOAT_MAT3x2:
        case GL_DOUBLE_MAT4x2:
        case GL_FLOAT_MAT4x2:
        case GL_UNSIGNED_INT_VEC2:
        case GL_INT_VEC2:
        case GL_BOOL_VEC2:
        case GL_FLOAT_VEC2:
        case GL_DOUBLE_VEC2:
            return 2;
        case GL_DOUBLE_MAT2x3:
        case GL_FLOAT_MAT2x3:
        case GL_DOUBLE_MAT3:
        case GL_FLOAT_MAT3:
        case GL_DOUBLE_MAT4x3:
        case GL_FLOAT_MAT4x3:
        case GL_UNSIGNED_INT_VEC3:
        case GL_INT_VEC3:
        case GL_BOOL_VEC3:
        case GL_FLOAT_VEC3:
        case GL_DOUBLE_VEC3:
            return 3;
        case GL_DOUBLE_MAT2x4:
        case GL_FLOAT_MAT2x4:
        case GL_DOUBLE_MAT3x4:
        case GL_FLOAT_MAT3x4:
        case GL_DOUBLE_MAT4:
        case GL_FLOAT_MAT4:
        case GL_UNSIGNED_INT_VEC4:
        case GL_INT_VEC4:
        case GL_BOOL_VEC4:
        case GL_FLOAT_VEC4:
        case GL_DOUBLE_VEC4:
            return 4;
        default: return 1;
    }
}

glslUtil::Types glslUtil::_getType(int type) {
    switch (type) {
        case GL_DOUBLE:
        case GL_DOUBLE_MAT2:
        case GL_DOUBLE_MAT2x3:
        case GL_DOUBLE_MAT2x4:
        case GL_DOUBLE_MAT3:
        case GL_DOUBLE_MAT3x2:
        case GL_DOUBLE_MAT3x4:
        case GL_DOUBLE_MAT4:
        case GL_DOUBLE_MAT4x2:
        case GL_DOUBLE_MAT4x3:
        case GL_DOUBLE_VEC2:
        case GL_DOUBLE_VEC3:
        case GL_DOUBLE_VEC4:
            return glslUtil::TYPE_DOUBLE;
        case GL_FLOAT:
        case GL_FLOAT_MAT2:
        case GL_FLOAT_MAT2x3:
        case GL_FLOAT_MAT2x4:
        case GL_FLOAT_MAT3:
        case GL_FLOAT_MAT3x2:
        case GL_FLOAT_MAT3x4:
        case GL_FLOAT_MAT4:
        case GL_FLOAT_MAT4x2:
        case GL_FLOAT_MAT4x3:
        case GL_FLOAT_VEC2:
        case GL_FLOAT_VEC3:
        case GL_FLOAT_VEC4:
            return glslUtil::TYPE_FLOAT;
        case GL_BOOL:
        case GL_BOOL_VEC2:
        case GL_BOOL_VEC3:
        case GL_BOOL_VEC4:
        case GL_INT:
        case GL_INT_SAMPLER_1D:
        case GL_INT_SAMPLER_1D_ARRAY:
        case GL_INT_SAMPLER_2D:
        case GL_INT_SAMPLER_2D_ARRAY:
        case GL_INT_SAMPLER_2D_MULTISAMPLE:
        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        case GL_INT_SAMPLER_2D_RECT:
        case GL_INT_SAMPLER_3D:
        case GL_INT_SAMPLER_BUFFER:
        case GL_INT_SAMPLER_CUBE:
        case GL_INT_VEC2:
        case GL_INT_VEC3:
        case GL_INT_VEC4:
        case GL_SAMPLER_1D:
        case GL_SAMPLER_1D_ARRAY:
        case GL_SAMPLER_1D_ARRAY_SHADOW:
        case GL_SAMPLER_1D_SHADOW:
        case GL_SAMPLER_2D:
        case GL_SAMPLER_2D_ARRAY:
        case GL_SAMPLER_2D_ARRAY_SHADOW:
        case GL_SAMPLER_2D_MULTISAMPLE:
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
        case GL_SAMPLER_2D_RECT:
        case GL_SAMPLER_2D_RECT_SHADOW:
        case GL_SAMPLER_2D_SHADOW:
        case GL_SAMPLER_3D:
        case GL_SAMPLER_BUFFER:
        case GL_SAMPLER_CUBE:
        case GL_SAMPLER_CUBE_SHADOW:
            return glslUtil::TYPE_INT;
        case GL_UNSIGNED_INT:
        case GL_UNSIGNED_INT_SAMPLER_1D:
        case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
        case GL_UNSIGNED_INT_SAMPLER_2D:
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
        case GL_UNSIGNED_INT_SAMPLER_3D:
        case GL_UNSIGNED_INT_SAMPLER_BUFFER:
        case GL_UNSIGNED_INT_SAMPLER_CUBE:
        case GL_UNSIGNED_INT_VEC2:
        case GL_UNSIGNED_INT_VEC3:
        case GL_UNSIGNED_INT_VEC4:
            return glslUtil::TYPE_UNSIGNED_INT;
        default:
            return glslUtil::TYPE_UNKNOWN;
    }
}

int glslUtil::_getUniformSize(int uniSize,int uniType,int uniArrayStride,int uniMatStride) {
    int retSize;
    if (uniArrayStride > 0)
        retSize = uniArrayStride * uniSize;
    else if (uniMatStride > 0) {
        switch(uniType) {
            case GL_FLOAT_MAT2:
            case GL_FLOAT_MAT2x3:
            case GL_FLOAT_MAT2x4:
            case GL_DOUBLE_MAT2:
            case GL_DOUBLE_MAT2x3:
            case GL_DOUBLE_MAT2x4:
                retSize = 2 * uniMatStride;
                break;
            case GL_FLOAT_MAT3:
            case GL_FLOAT_MAT3x2:
            case GL_FLOAT_MAT3x4:
            case GL_DOUBLE_MAT3:
            case GL_DOUBLE_MAT3x2:
            case GL_DOUBLE_MAT3x4:
                retSize = 3 * uniMatStride;
                break;
            case GL_FLOAT_MAT4:
            case GL_FLOAT_MAT4x2:
            case GL_FLOAT_MAT4x3:
            case GL_DOUBLE_MAT4:
            case GL_DOUBLE_MAT4x2:
            case GL_DOUBLE_MAT4x3:
                retSize = 4 * uniMatStride;
                break;
        }
    }
    else
        retSize = glslTypeSizeMap[uniType];
    return retSize;
}

void glslUtil::_printInt(int rows,int columns,int *f) {
    for (int i = 0; i < rows; ++i) {
        if(columns==1)
            flylogNoTime("%d",f[i*columns]);
        else if(columns==2)
            flylogNoTime("%d %d",f[i*columns],f[i*columns+1]);
        else if(columns==3)
            flylogNoTime("%d %d %d",f[i*columns],f[i*columns+1],f[i*columns+2]);
        else if(columns==4)
            flylogNoTime("%d %d %d %d",f[i*columns],f[i*columns+1],f[i*columns+2],f[i*columns+3]);
    }
}

void glslUtil::_printUnsignedInt(int rows,int columns,unsigned int *f) {
    for (int i = 0; i < rows; ++i) {
        if (columns==1)
            flylogNoTime("%u",f[i*columns]);
        else if(columns==2)
            flylogNoTime("%u %u",f[i*columns],f[i*columns+1]);
        else if(columns==3)
            flylogNoTime("%u %u %u",f[i*columns],f[i*columns+1],f[i*columns+2]);
        else if(columns==4)
            flylogNoTime("%u %u %u %u",f[i*columns],f[i*columns+1],f[i*columns+2],f[i*columns+3]);
    }
}

void glslUtil::_printFloat(int rows,int columns,float *f) {
    for (int i = 0; i < rows; ++i) {
        if (columns==1)
            flylogNoTime("%f",f[i*columns]);
        else if(columns==2)
            flylogNoTime("%f %f",f[i*columns],f[i*columns+1]);
        else if(columns==3)
            flylogNoTime("%f %f %f",f[i*columns],f[i*columns+1],f[i*columns+2]);
        else if(columns==4)
            flylogNoTime("%f %f %f %f",f[i*columns],f[i*columns+1],f[i*columns+2],f[i*columns+3]);
    }
}

void glslUtil::printUBOInfo(int program,const char* uboName,const char* uniName) {
    if (!glIsProgram(program)) {
        flylog("%d is not a program", program);
        return;
    }
    int uboIndex = glGetUniformBlockIndex(program,uboName);
    if (uboIndex == GL_INVALID_INDEX) {
        flylog("%s is not a valid name in block %s",uniName,uboName);
        return;
    }
    int bindIndex,uboID;
    glGetActiveUniformBlockiv(program,uboIndex,GL_UNIFORM_BLOCK_BINDING, &bindIndex);
    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, bindIndex, &uboID);
    flylog("%s binding point: %d uboIndex %d uboID %d",uboName,bindIndex,uboIndex,uboID);
    
    unsigned int uniIndex;
    glGetUniformIndices(program, 1, &uniName, &uniIndex);
    flylog("index of uni: %u", uniIndex);
    
    int uniType, uniOffset, uniSize, uniArrayStride, uniMatStride;
    glGetActiveUniformsiv(program, 1, &uniIndex, GL_UNIFORM_TYPE, &uniType);
    glGetActiveUniformsiv(program, 1, &uniIndex, GL_UNIFORM_OFFSET, &uniOffset);
    glGetActiveUniformsiv(program, 1, &uniIndex, GL_UNIFORM_SIZE, &uniSize);
    glGetActiveUniformsiv(program, 1, &uniIndex, GL_UNIFORM_ARRAY_STRIDE, &uniArrayStride);
    glGetActiveUniformsiv(program, 1, &uniIndex, GL_UNIFORM_MATRIX_STRIDE, &uniMatStride);
    int totalSize = _getUniformSize(uniSize,uniType,uniArrayStride,uniMatStride);
    int rows = _getRows(uniType);
    int columns = uniSize / (rows * sizeof(float));
    int type=_getType(uniType);
    flylog("%s.%s size %d offset %d",uboName,uniName,uniSize,uniOffset);
    
    glBindBuffer(GL_UNIFORM_BUFFER, uboID);
    if(type==glslUtil::TYPE_FLOAT) {
        float values[16];
        glGetBufferSubData(GL_UNIFORM_BUFFER,uniOffset,totalSize,values);
        _printFloat(rows,columns,values);
    }
    else if(_getType(uniType)==glslUtil::TYPE_INT) {
        int values[16];
        glGetBufferSubData(GL_UNIFORM_BUFFER,uniOffset,totalSize,values);
        _printInt(rows,columns,values);
    }
    else if(_getType(uniType)==glslUtil::TYPE_UNSIGNED_INT) {
        unsigned int values[16];
        glGetBufferSubData(GL_UNIFORM_BUFFER,uniOffset,totalSize,values);
        _printUnsignedInt(rows,columns,values);
    }
    else if(_getType(uniType)==glslUtil::TYPE_DOUBLE) {
        double values[16];
        glGetBufferSubData(GL_UNIFORM_BUFFER,uniOffset,totalSize,values);
        _printFloat(rows,columns,(float*)values);
    }
}

void glslUtil::printAllUniforms(int programID){
    int maxCount=0;
    glGetProgramiv(programID,GL_ACTIVE_UNIFORMS, &maxCount);
    for(int i=0;i<maxCount;i++){
        int retType=0;
        int retLen=0;
        int retSize=0;
        char retName[50]={0};
        int uniLocation = i;
        glGetActiveUniform(programID, uniLocation, sizeof(retName), (GLsizei*)&retLen, (GLint*)&retSize,(GLenum*)&retType, retName);
        if (retLen!=0) {
            flylog("[program %d]index %d name %s",programID, uniLocation,retName);
            _printUniformValueByTypeAndLocation(programID,retType,uniLocation);
        }
        else
            flylog("program %s uniLocation %d is not an active uniform", programID,uniLocation);
    }
}

void glslUtil::_printUniformValueByTypeAndLocation(int programID,int type,int uniLocation){
    int rows=_getRows(type), columns=_getColumns(type);
    int type_enum=_getType(type);
    if (type_enum==glslUtil::TYPE_FLOAT) {
        float value[16];
        glGetUniformfv(programID,uniLocation,value);
        _printFloat(rows,columns,value);
    }
    else if(type_enum==glslUtil::TYPE_INT) {
        int value[16];
        glGetUniformiv(programID,uniLocation,value);
        _printInt(rows,columns,value);
    }
    else if(type_enum==glslUtil::TYPE_UNSIGNED_INT) {
        unsigned int value[16];
        glGetUniformuiv(programID,uniLocation,value);
        _printUnsignedInt(rows,columns,value);
    }
    else if(type_enum==glslUtil::TYPE_DOUBLE) {
        double value[16];
        glGetUniformdv(programID, uniLocation, value);
        _printFloat(rows,columns,(float*)value);
    }
}

void glslUtil::printUniformValue(int programID,const char* uniformName){
    if (!glIsProgram(programID)) {
        flylog("printUBOInfo: %d is not a program", programID);
        return;
    }
    int retType=0;
    int retLen=0;
    int retSize=0;
    char retName[50]={0};
    char findName[50]={0};
    strcpy(findName,uniformName);
    //这里获取得到的location不准确
    int uniLocation = glGetUniformLocation((int)programID, findName);
    glGetActiveUniform(programID, uniLocation, sizeof(retName), (GLsizei*)&retLen, (GLint*)&retSize,(GLenum*)&retType, retName);
    if (retLen!=0) {
        flylog("[%s]values in program %d %s", uniformName, programID,retName);
        _printUniformValueByTypeAndLocation(programID,retType,uniLocation);
    }
    else
        flylog("%s is not an active uniform in program %u", uniformName, programID);
}

void glslUtil::printAllUniformAndBlock(int programID){
    int uniCount=0;
    glGetProgramiv(programID,GL_ACTIVE_UNIFORMS, &uniCount);
    for(int i=0;i<uniCount;i++){
        char uniName[256]={0};
        int uniType=0;
        int uniSize=0;
        int uniArrayStride=0;
        int oneSize=0;
        int uniIndex=0;
        int retLen=0;
        glGetActiveUniformsiv(programID, 1, (const GLuint*)&i, GL_UNIFORM_BLOCK_INDEX, &uniIndex);
        if (uniIndex == -1) {
            //常规uniform，不是uniform block
            glGetActiveUniformName(programID, i, sizeof(uniName), &retLen, uniName);
            glGetActiveUniformsiv(programID, 1,(const GLuint*)&i, GL_UNIFORM_TYPE, &uniType);
            glGetActiveUniformsiv(programID, 1,(const GLuint*)&i, GL_UNIFORM_SIZE, &uniSize);
            glGetActiveUniformsiv(programID, 1,(const GLuint*)&i, GL_UNIFORM_ARRAY_STRIDE, &uniArrayStride);
            if (uniArrayStride>0)
                oneSize = uniArrayStride * uniSize;
            else
                oneSize = glslTypeSizeMap[uniType];
            flylogNoTime("[program %d]index %d name %s size %d",programID,i,uniName,oneSize);
        }
    }
    
    int blockCount=0;
    glGetProgramiv(programID,GL_ACTIVE_UNIFORM_BLOCKS,&blockCount);
    for(int i=0;i<blockCount;i++){
        char blockName[256]={0};
        int bindIndex=0;
        int uboID=0;
        int uniSize=0;
        int retLen=0;
        int oneSize=0;
        glGetActiveUniformBlockName(programID, i, sizeof(blockName),&retLen,blockName);
        glGetActiveUniformBlockiv(programID, i, GL_UNIFORM_BLOCK_DATA_SIZE, &uniSize);
        glGetActiveUniformBlockiv(programID, i, GL_UNIFORM_BLOCK_BINDING, &bindIndex);
        glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, bindIndex, &uboID);
        
        
        int uniCount=0;
        glGetActiveUniformBlockiv(programID, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniCount);
        int* indexBuf=(int*)malloc(sizeof(int)*uniCount);
        glGetActiveUniformBlockiv(programID, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,indexBuf);
        
        for(int j=0;j<uniCount;j++){
            char uniName[256]={0};
            int retLen=0;
            int uniType=0;
            int uniSize=0;
            int uniOffset=0;
            int uniArrayStride=0;
            int uniMatStride=0;
            int oneSize=0;
            glGetActiveUniformName(programID, indexBuf[j], sizeof(uniName), &retLen, uniName);
            glGetActiveUniformsiv(programID, 1,(const GLuint*)&indexBuf[j], GL_UNIFORM_TYPE, &uniType);
            glGetActiveUniformsiv(programID, 1,(const GLuint*)&indexBuf[j], GL_UNIFORM_SIZE, &uniSize);
            glGetActiveUniformsiv(programID, 1,(const GLuint*)&indexBuf[j], GL_UNIFORM_OFFSET, &uniOffset);
            glGetActiveUniformsiv(programID, 1,(const GLuint*)&indexBuf[j], GL_UNIFORM_ARRAY_STRIDE, &uniArrayStride);
            glGetActiveUniformsiv(programID, 1,(const GLuint*)&indexBuf[j], GL_UNIFORM_MATRIX_STRIDE, &uniMatStride);
            oneSize=_getUniformSize(uniSize, uniType, uniArrayStride, uniMatStride);
            flylogNoTime("[block %d]index %d name %s size %d offset %d",i,indexBuf[j],uniName,oneSize,uniOffset);
            
            int rows = _getRows(uniType);
            int columns = _getColumns(uniType);
            if(columns<=0)
                columns=1;
            
            if(uboID>0){
                glBindBuffer(GL_UNIFORM_BUFFER, uboID);
                int type_enum=_getType(uniType);
                if(type_enum==glslUtil::TYPE_FLOAT) {
                    float values[16];
                    glGetBufferSubData(GL_UNIFORM_BUFFER,uniOffset,oneSize,values);
                    _printFloat(rows,columns,values);
                }
                else if(type_enum==glslUtil::TYPE_INT) {
                    int values[16];
                    glGetBufferSubData(GL_UNIFORM_BUFFER,uniOffset,oneSize,values);
                    _printInt(rows,columns,values);
                }
                else if(type_enum==glslUtil::TYPE_UNSIGNED_INT) {
                    unsigned int values[16];
                    glGetBufferSubData(GL_UNIFORM_BUFFER,uniOffset,oneSize,values);
                    _printUnsignedInt(rows,columns,values);
                }
                else if(type_enum==glslUtil::TYPE_DOUBLE) {
                    double values[16];
                    glGetBufferSubData(GL_UNIFORM_BUFFER,uniOffset,oneSize,values);
                    _printFloat(rows,columns,(float*)values);
                }
            }
        }
        flylogNoTime("[program %d]index %d block %s size %d",programID,i,blockName,oneSize);
    }
}


