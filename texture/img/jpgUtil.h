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
#include <GLUT/GLUT.h>
#include "baseImg.h"

namespace flyEngine{
class jpgUtil{
public:
    static struct_texture* loadFile(const char* szFile);
};

}

#endif /* jpgUtil_h */
