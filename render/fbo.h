//
//  fbo.h
//  flyEngine
//
//  Created by joe on 15/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef fbo_h
#define fbo_h

#include <stdio.h>
#include "defines.h"
NS_FLYENGINE_BEGIN

struct fboStruct{
    unsigned int fbo;   //frame buffer id
    unsigned int rbo;   //render buffer id
    unsigned int texID; //texture id bind as an attachment with fbo
};

class fbo{
public:
    static fboStruct createFBO();
    static fboStruct createFBOMSAA();
    static fboStruct createFBOForDepth();
    static fboStruct createFBOForDepthWithCubemap();
};

NS_FLYENGINE_END


#endif /* fbo_h */
