//
//  jpgUtil.h
//  flyEngine
//
//  Created by joe on 05/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef jpgUtil_h
#define jpgUtil_h

#include <stdio.h>

#include <stdio.h>
#include "baseImg.h"

namespace flyEngine{
class jpgUtil{
public:
    static bool loadFile(const char* szFile,struct_texture* texinfo,bool bFlipY=false);
    static bool isJpg(const char* szFile);
};

}

#endif /* jpgUtil_h */
