//
//  testMain.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "testMain.h"
#include "flyEngine.h"
#include "testPrimitive.h"
#include "test2d.h"
#include "testPixel.h"
#include "test3D.h"
#include "testAniBySwapBuffer.h"
#include "test3DPolyhedra.h"
#include "testCurve.h"
#include "testTexXD.h"
#include "testShader.h"

void testMainBegin(){
    engineInit();
    setCurrentWorkDir("/Users/joe/Documents/work_OpenGL/flyEngine/");
//        testPrimitive();
//    testBMP();
//    test2dTrans();
//    test3dView();
//    testAniBySwapBuffer();
//    test3DPolyhedra();
//    testCurve();
//    testTex2D();
//    testTex2DJpg();
//    testTex2DPng();
//    testTex3DPng();
    testShaderNormal();
}
