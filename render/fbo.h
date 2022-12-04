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
struct fboDeferredShadingStruct{
    unsigned int fbo;   //frame buffer id
    unsigned int rbo;   //render buffer id
    unsigned int texPos;    //position buffer
    unsigned int texNormal; //normal buffer
    unsigned int texAlbedoSpec; //diffuse and specular buffer
};

struct fboHDRBloomGaussStruct{
    unsigned int fboHDR;   //frame buffer id for hdr
    unsigned int fboBloomArr[2];   //frame buffer arr for bloom
    unsigned int rbo;   //render buffer id
    unsigned int texHDRArr[2]; //texture id arr1 for hdr color and brightness
    unsigned int texGaussianArr[2]; //texture id arr2 for bloom pingpong blur
};


class fbo{
public:
    static fboStruct createFBO();
    static fboStruct createFBOMSAA();
    static fboStruct createFBOForDepth();
    static fboStruct createFBOForDepthWithCubemap();
    static fboStruct createFBOHDR();
    static fboHDRBloomGaussStruct createFBOHDRBloomGauss();
    static fboDeferredShadingStruct createFBODeferredShading();
};

NS_FLYENGINE_END


#endif /* fbo_h */
