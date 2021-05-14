//
//  VAORender.h
//  flyEngine
//
//  Created by joe on 04/09/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef VAOMgr_h
#define VAOMgr_h

#include <stdio.h>
#include <tuple>
#include "defines.h"

namespace flyEngine {

class VAOMgr{
public:
    VAOMgr(){};
    static unsigned int createVAO(void* verticeBuf,unsigned int sizeOfVertice,int numPerVertex,int stride,bool transPos);
    
    //有颜色指定，不使用ebo
    static unsigned int createVAO(void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
                                   int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos);
    //有颜色指定，使用ebo
    static unsigned int createVAO(void* verticeBuf,int sizeOfVertice,void* verticeIndexBuf,int sizeOfVerticeIndex,int numPerVertex,int numPerColor,int numPerTexture,int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos);
    
    //没有颜色指定，使用ebo
    static unsigned int createVAO(void* verticeBuf,int sizeOfVertice,void* verticeIndexBuf,int sizeOfVerticeIndex,int numPerVertex,int numPerTexture,int stride,int offsetVertex,int offsetTexture);
    
    //没有颜色指定，不使用ebo
    static unsigned int createVAO(void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerTexture,
                                   int stride,int offsetVertex,int offsetTexture);
    
    
//    void useShader(const char* szFileVS,const char* szFileFS);
    static void drawPrimitive(unsigned int vao,unsigned int primitiveType,unsigned int vertexNumTotal);
    static void drawPrimitive(unsigned int vao,unsigned int textureID,unsigned int primitiveType,unsigned int vertexNumTotal);
};

}

#endif /* VAOMgr_h */


