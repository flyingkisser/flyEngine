//
//  error.cpp
//  lines
//
//  Created by Joe on 2017/4/26.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "error.h"
#include <stdio.h>


GLenum errorCheck ()
{
    GLenum code;
    const GLubyte *string;
    code = glGetError ();
    if (code != GL_NO_ERROR)
    {
        string = gluErrorString (code);
        fprintf( stderr, "OpenGL error: %s\n", string );
    }
    return code;
}
