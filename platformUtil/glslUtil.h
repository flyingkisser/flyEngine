//
//  glslUtil.h
//  flyEngine
//
//  Created by joe on 02/08/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef glslUtil_h
#define glslUtil_h

#include <stdio.h>

class glslUtil{
public:
    enum Types { TYPE_UNKNOWN, TYPE_INT, TYPE_UNSIGNED_INT, TYPE_FLOAT, TYPE_DOUBLE};
    static void init();
    static int _getRows(int type);
    static int _getColumns(int type);
    static void _printInt(int rows,int columns,int *f);
    static void _printUnsignedInt(int rows,int columns,unsigned int *f);
    static void _printFloat(int rows,int columns,float *f);
    static Types _getType(int type);
    static int  _getUniformSize(int uniSize,int uniType,int uniArrayStride,int uniMatStride);
    static void _printUniformValueByTypeAndLocation(int programID,int type,int uniLocation);

    static void printAllUniforms(int programID);
    static void printAllUniformAndBlock(int programID);
    static void printUniformValue(int programID,const char* uboName);
    static void printUBOInfo(int program,const char* uboName,const char* uniName);
    
    static void printAllSSBO(int programID);
    
    
};

#endif /* glslUtil_h */
