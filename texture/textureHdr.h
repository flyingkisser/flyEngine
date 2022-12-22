//
//  textureHdr.h
//  flyEngine
//
//  Created by joe on 21/12/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef textureHdr_h
#define textureHdr_h

#include <stdio.h>

#include "defines.h"
#include "texture2.h"

using namespace std;

NS_FLYENGINE_BEGIN


class textureHdr: public texture2{
    
private:
    float* _dataBuf;
    
public:
    textureHdr(const char* szPath,bool _bFlipY=false);
    bool init();
    float* getBuf(){return _dataBuf;};
    void glInit(int texturePos=0);
}

NS_FLYENGINE_END
#endif /* textureHdr_h */
