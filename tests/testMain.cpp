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
#include "test2D.h"
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

void main_logic(){
     test3d_drawOneCube();
}


void testMainBegin(){
    initWindow();
//    flyEngine::camera* cameraObj=new flyEngine::camera();
//    flyEngine::world::getInstance()->setCamera(cameraObj);
    main_logic();
    flyEngine::world::getInstance()->start_rendering();
//    main_logic();
}




//void testMainBegin2(){
//    initWindow();
//    flyEngine::camera* cameraObj=new flyEngine::camera();
//    flyEngine::world* worldObj=flyEngine::world::getInstance();
//    worldObj->setCamera(cameraObj);
//    worldObj->start();
//
//    flyEngine::node* nodeObj=new flyEngine::node("res/fire.png");
//    worldObj->addChild(nodeObj);
//
//    while(!glfwWindowShouldClose(g_window)){
//        threadUtil::sleep(0.1);   //1000 means 1ms
//
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//
//        worldObj->draw();
//
//        glfwSwapBuffers(g_window);
//        glfwPollEvents();
//    }
//
//}


//void testMainBegin(){
//    engineInit();
//
//    initWindow();
////    drawPlane2();
//    drawCube();
//
////    std_move_forward_test();
////    testBasic();
//
////    testPrimitive();
////    testBMP();
////    test2dViewJpg();
////     test2dViewPng();
////    test2dViewMixPng();
////    test2dViewMixPng2();
////     test2dViewMixPng3();
////    test2dViewMixPng4();
////    test2dViewMixPng5();
////    test2dViewTransPng();
////    test2dViewTransPng2();
////     test2dViewTransPng3();
////     test2dViewTransPng4();
//
//   // test3dView();
////    test3dViewRotate();
////    test3dViewControl();
////    test3dViewControlCamera();
////    test3dViewMoreCube();
////    test3dViewMoreCubeCamera();
////    test3dViewMoreCubeCameraFPS();
//
////    testAniBySwapBuffer();
////    test3DPolyhedra();
////    testCurve();
////    testTex3DPng();
////    testShaderNormal();
//
////    testPhongAmbient();
//}
