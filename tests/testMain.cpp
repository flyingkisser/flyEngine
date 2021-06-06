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
#include "testWindow.h"
#include "testPhong.h"

#include "std_move_forward.h"

//class A
//{
//    int x;
//  public:
//    A(int x):x(x) { cout << "ctor invoked x="<<x << endl; }
//    virtual ~A() { cout << "dtor invoked x=" <<x<< endl; }
//    void sayHi() const { cout << "HI x="<<x << endl; }
//};
//
//class B : public A { };
//
//void testBasic(){
//    std::unique_ptr<A> a(new A{ 2 });
//    // unique_ptr<X> c = a; //does not compile! no copy constructor
//    std::unique_ptr<A> b = std::move(a); //valid: move constructor
////    a->sayHi();
//    b->sayHi();
//}

void testMainBegin(){
    engineInit();
    
    initWindow();
//    std_move_forward_test();
//    testBasic();
    
//    testPrimitive();
//    testBMP();
//    test2dViewJpg();
//     test2dViewPng();
//    test2dViewMixPng();
//    test2dViewMixPng2();
//     test2dViewMixPng3();
//    test2dViewMixPng4();
//    test2dViewMixPng5();
//    test2dViewTransPng();
//    test2dViewTransPng2();
//     test2dViewTransPng3();
//     test2dViewTransPng4();

//    test3dView();
//    test3dViewRotate();
//    test3dViewControl();
//    test3dViewControlCamera();
//    test3dViewMoreCube();
//    test3dViewMoreCubeCamera();
//    test3dViewMoreCubeCameraFPS();
    
//    testAniBySwapBuffer();
//    test3DPolyhedra();
//    testCurve();
//    testTex3DPng();
//    testShaderNormal();
    
    testPhongAmbient();
}
