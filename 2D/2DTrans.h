//
//  2dUtil.hpp
//  lines
//
//  Created by joe on 30/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef _2dTrans_h
#define _2dTrans_h

#include <stdio.h>
#include "defines.h"

//void move2dBy(p2* p1,float x,float y);
//void scale2d(p2* p1,float sx,float sy);
//void rotate2d(p2* p1,float theta);
//
//
//void move(float x,float y);

namespace flyEngine {

class _2dTrans{
public:
    static void move(float x,float y);
    static void rotate(float theta);
    static void scale(float sx,float sy);
    static void reset();
};


}




#endif
