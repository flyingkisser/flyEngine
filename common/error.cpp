//
//  error.cpp
//  lines
//
//  Created by Joe on 2017/4/26.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "error.h"
#include "defines.h"


int checkGLError()
{
//    GLenum code;
//    const GLubyte *string;
//    code = glGetError ();
//    if (code != GL_NO_ERROR)
//    {
//        string = gluErrorString (code);
//        fprintf( stderr, "OpenGL error: %s\n", string );
//    }
//    return code;
    const char *strError;
    int error=(int)glfwGetError(&strError);
    if(error!=GL_NO_ERROR){
        fprintf( stderr, "OpenGL error: %s\n", strError );
    }
    return error;
}
