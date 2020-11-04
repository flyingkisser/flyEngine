//
//  pngUtil.h
//  flyEngine
//
//  Created by joe on 03/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef pngUtil_h
#define pngUtil_h

#include <stdio.h>
#include "baseImg.h"

namespace flyEngine{
class pngUtil{
public:
    static bool loadFile(const char* szFile,struct_texture* texinfo);
    static bool isPng(const char* szFile);
};

}

#endif /* pngUtil_h */
