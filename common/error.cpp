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
    int error=0;
#ifdef BUILD_MAC
    const char *strError;
    error=(int)glfwGetError(&strError);
    if(error!=GL_NO_ERROR){
        fprintf( stderr, "OpenGL error: %s\n", strError );
    }
#elif BUILD_IOS
    error = glGetError();
    if (error!=GL_NO_ERROR)
    {
//        const GLubyte *string;
        //string = gluErrorString(error);
        fprintf( stderr, "OpenGL error code : %d\n", error );
    }
#endif
    return error;
}
