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
#include <GLUT/GLUT.h>
#include "baseImg.h"

namespace flyEngine{
class pngUtil{
public:
    static struct_texture* loadFile(const char* szFile);
};

}

#endif /* pngUtil_h */
