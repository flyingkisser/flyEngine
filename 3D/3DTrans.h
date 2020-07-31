//
//  3DTrans.h
//  flyEngine
//
//  Created by joe on 13/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef _DTrans_h
#define _DTrans_h

#include "defines.h"
#include "matrix.h"

namespace flyEngine {

class _3dTrans{
public:
    static void _setMoveMatrix(Matrix4x4 m,float x,float y,float z);
    static void _setRotateMatrix(Matrix4x4 mat,structPos3* axisStart,structPos3* axisEnd,float theta);
    static void _setScaleMatrix(Matrix4x4 m,float sx,float sy,float sz);
    static void _setReflectMatrix(Matrix4x4 m,bool x,bool y,bool z);
    
    static void _rotate(structPos3* p,structPos3* axisStart,structPos3* axisEnd,float theta);
    
    static void rotate(structPos3* p,structPos3* axis,float theta);
    static void scale(structPos3* pos,float sx,float sy,float sz);

};


}

#endif /* _DTrans_h */
