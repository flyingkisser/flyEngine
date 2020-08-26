//
//  base.h
//  flyEngine
//
//  Created by joe on 05/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef base_h
#define base_h

struct struct_texture
{
    GLsizei width;
    GLsizei height;
    GLenum format;
    GLint internalFormat;
    GLuint id;
    GLubyte *buf;
};


#endif /* base_h */
