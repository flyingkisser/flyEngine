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

struct fboBloomMip{
    glm::vec2 floatSize;
    glm::ivec2 intSize;
    unsigned int tex;
};

struct fboHDRBloomPhysicalStruct{
    unsigned int fboChain;   //frame buffer id for hdr
    std::vector<fboBloomMip> mips;   //frame buffer arr for bloom
    unsigned int fboHDR;   //frame buffer id for hdr
    unsigned int rbo;   //render buffer id
    unsigned int texHDRArr[2]; //texture id arr1 for hdr color and brightness
};

struct fboSSAOStruct{
    unsigned int fboGBuffer;   //frame buffer id
    unsigned int rbo;   //render buffer id
    unsigned int texPos;    //position buffer
    unsigned int texNormal; //normal buffer
    unsigned int texAlbedoSpec; //diffuse and specular buffer
    unsigned int fboSSAO;   //frame buffer id
    unsigned int texSSAOColor; //diffuse and specular buffer
    unsigned int fboSSAOBlur;
    unsigned int texSSAOColorBlur;
    unsigned int texNoise;
};

struct fboOitStruct{
    unsigned int fboOpaque;   //frame buffer id
    unsigned int fboTransparent;   //frame buffer
    unsigned int rbo;   //render buffer id
    unsigned int texOpaque;
    unsigned int texDepth;
    unsigned int texAccum;
    unsigned int texReveal;
};

struct fboCsmStruct{
    unsigned int fboLight;   //frame buffer id
    unsigned int texDepthArr;
};

struct fboWater{
    unsigned int fbo1;   //frame buffer id
    unsigned int fbo2;   //frame buffer
    unsigned int fbo3;  //fbo for depth
    unsigned int rbo;   //render buffer id
    unsigned int tex1;
    unsigned int tex2;
    unsigned int texDepth;
};


class fbo{
public:
    static fboStruct createFBO();
    static fboStruct createFBOMSAA();
    static fboStruct createFBOForDepth();
    static fboStruct createFBOForDepthSampler2DArray(int texNum);
    static fboStruct createFBOForDepthWithCubemap();
    static fboStruct createFBOForIBLWithCubemap();
    static fboStruct createFBOHDR();
    static fboHDRBloomGaussStruct createFBOHDRBloomGauss();
    static fboHDRBloomPhysicalStruct createFBOHDRBloomPhysical(int mipChainLen);
    static fboDeferredShadingStruct createFBODeferredShading();
    static fboSSAOStruct createFBOSSAO();
    static fboOitStruct createFBOOit();
    static fboStruct createFBOCsm(int depthLevel);
    static fboWater createFBOWater();
    
};

NS_FLYENGINE_END


#endif /* fbo_h */
