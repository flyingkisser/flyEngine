//
//  testMain.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "test_node.h"
#include "test_action.h"
#include "test_pointLight.h"
#include "test_spotLight.h"
#include "test_model.h"
#include "test_depth_stencil_blend_facecull.h"
#include "test_text.h"
#include "test_framebuffer.h"
#include "test_sprite.h"
#include "test_skybox.h"
#include "test_ubo.h"
void testMainBegin(){
    //code cube test
//    test_cubeColor();
    
    //texture cube test
//     test_oneCubeTex();
    
//      test_twoCubeTex();
//    test_multiCubeTex(3);
    
    //action test
//    test_actionMove();
//    test_actionSequence();
//    test_actionSpawn();
//    test_actionRepeat();
//    test_actionForever();
//    test_actionForeverAndStop();
    
    
    //light

    
    //点光源测试
//    test_onePointLight_oneCube();
// test_onePointLight_oneCube_changingColor();
//    test_onePointLight_twoCube();
//    test_twoPointLight_oneCube();
    
    //点光源测试(高亮贴图)
//    test_onePointLight_oneCube_specularMap();
//    test_onePointLight_twoCube_specularMap();
//    test_onePointLight_multiCube_specularMap();
    
    //手电筒光源测试
//    test_oneSpotLight_oneCube();
//    test_oneSpotLight_multiCube_specularMap();
//    test_oneSpotLight_multiCube_specularMap_cameraControl();
//    test_vector();
//    test_one_model();
    
    test_depths();
//    test_depths_2();
//    test_stencil();
//    test_stencil_2();
//    test_blend_1();
//    test_blend_2();
//        test_text1();
    
//    test_facecull();
//    test_facecull_2();
//    test_sprite_1();
    
//    test_framebuffer();
//    test_framebuffer_mirror();
//    test_framebuffer_kernel();
    
//    test_skybox_1();
//    test_skybox_reflect();
//    test_skybox_reflect_2();
//    test_skybox_reflect_3();
//    test_skybox_refract_1();
//    test_ubo_1();
}
