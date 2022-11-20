//
//  error.cpp
//  lines
//
//  Created by Joe on 2017/4/26.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "error.h"
#include "defines.h"

static const char * getGLErrorStr(GLenum error)
{
    const char *str;
    switch( error )
    {
        case GL_NO_ERROR:
            str = "GL_NO_ERROR";
            break;
        case GL_INVALID_ENUM:
            str = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            str = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            str = "GL_INVALID_OPERATION";
            break;
#ifdef __gl_h_
        case GL_STACK_OVERFLOW:
            str = "GL_STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            str = "GL_STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            str = "GL_OUT_OF_MEMORY";
            break;
//        case GL_TABLE_TOO_LARGE:
//            str = "GL_TABLE_TOO_LARGE";
//            break;
#endif
#if GL_EXT_framebuffer_object
        case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
            str = "GL_INVALID_FRAMEBUFFER_OPERATION_EXT";
            break;
#endif
        default:
            str = "(ERROR: Unknown Error Enum)";
            break;
    }
    return str;
}

int checkGLError(const char *pos)
{
    int error=0;
#ifdef BUILD_MAC
    const char *strError;
    error=(int)glfwGetError(&strError);
    if(error!=GL_NO_ERROR){
        if(pos!=NULL)
            fprintf( stderr, "OpenGL error: %s [%s]\n", strError,pos);
        else
            fprintf( stderr, "OpenGL error: %s\n", strError);
    }
#elif BUILD_IOS
    error = glGetError();
    if (error!=GL_NO_ERROR){
        if(pos!=NULL)
            fprintf( stderr, "OpenGL error code : %d %s [%s]\n", error, getGLErrorStr(error),pos);
        else
            fprintf( stderr, "OpenGL error code : %d %s\n", error, getGLErrorStr(error));
    }
#endif
    return error;
}
