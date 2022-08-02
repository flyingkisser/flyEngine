//
//  glslUtil.cpp
//  flyEngine
//
//  Created by joe on 02/08/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "glslUtil.h"
#include "logUtil.h"



void glslUtil::printUBOInfo(int programID,const char* uboName){
    if (!glIsProgram(programID)) {
        flylog("printUBOInfo: %d is not a program", programID);
        return;
    }
    GLenum type;
    GLsizei l;
    GLint s;
    char c[50];
    int loc = glGetUniformLocation((int)programID, uboName);
    glGetActiveUniform(programID, loc, 0, &l, &s, &type, c);

    if (loc != -1) {
        flylog("values for uniform %s in program %d", uboName, programID);
        int rows = getRows(type), columns = getColumns(type);
        if (getType(type) == FLOAT) {
            float f[16];
            glGetUniformfv(programID, loc, f);
            displayUniformf(f,rows,columns);
        }
        else if (getType(type) == INT) {
            int f[16];
            glGetUniformiv(programID, loc, f);
            displayUniformi(f,rows,columns);
        }
        else if (getType(type) == UNSIGNED_INT) {
            unsigned int f[16];
            glGetUniformuiv(programID, loc, f);
            displayUniformui(f,rows,columns);
        }
        else if (getType(type) == DOUBLE) {
            double f[16];
            glGetUniformdv(programID, loc, f);
            displayUniformd(f,rows,columns);
        }
    }
    else
        flylog("%s is not an active uniform in program %u", uboName, programID);
}
