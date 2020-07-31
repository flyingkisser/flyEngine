//
//  matrix.h
//  flyEngine
//
//  Created by joe on 13/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef matrix_h
#define matrix_h

#include "defines.h"

namespace flyEngine {
class matrix{
public:
    static void setIdentity(Matrix4x4 mat);
    static void preMultiply(Matrix4x4 m1,Matrix4x4 m2);
    static void preMultiplyWithVector(Matrix4x4 m1,structPos4* pos);
};

}

#endif /* matrix_h */
