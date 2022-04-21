//
//  testMain.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//
#include "test_node.h"
#include "test_action.h"
#include "test_light.h"
#include "test_pointLight.h"
#include "test_spotLight.h"

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

    
    test_oneLight_oneCube();
//     test_oneLight_oneCube_changingColor();
//    test_oneLight_twoCube();
//    test_twoLight_oneCube();
    
    //带高亮贴图
//    test_oneLight_oneCube_specularMap();
//    test_oneLight_twoCube_specularMap();
//    test_oneLight_multiCube_specularMap();
    
//    test_onePointLight_oneCube();
//    test_onePointLight_multiCube_specularMap();
    
//    test_oneSpotLight_oneCube();
//    test_oneSpotLight_multiCube_specularMap();
    
//    test_multiLight_multiCube_specularMap();
   

}
