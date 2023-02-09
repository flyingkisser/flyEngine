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
#include "test_sprite_quad.h"
#include "test_skybox.h"
#include "test_ubo.h"
#include "test_gs.h"
#include "test_cubeTexIns.h"
#include "test_msaa.h"
#include "test_gamma.h"
#include "test_shadow_mapping.h"
#include "test_normal_parallax_mapping.h"
#include "test_hdr_bloom.h"
#include "test_deferred_shading.h"
#include "test_ssao.h"
#include "test_pbr.h"
#include "test_oit.h"
#include "test_animation.h"

void testMainBegin(){
    //code cube test
//    test_cubeColor();
    
    //texture cube test
//     test_oneCubeTex();
    
//     test_twoCubeTex();
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
//    test_onePointLight_oneCube_BlinnPhong();

// test_onePointLight_oneCube_changingColor();
//    test_onePointLight_twoCube();
//    test_twoPointLight_oneCube();
    
    //点光源测试(高亮贴图)
//    test_onePointLight_oneCube_specularMap();
//    test_onePointLight_twoCube_specularMap();
//    test_onePointLight_multiCube_specularMap();
    
    //手电筒光源测试
//    test_oneSpotLight_oneCube();
//         test_oneSpotLight_oneCube_BlinnPhong();
        
//    test_oneSpotLight_multiCube_specularMap();
//    test_oneSpotLight_multiCube_specularMap_cameraControl();
//    test_vector();
//    test_one_model();
//    test_one_model_fbx();
//    test_one_model_onePointLight_BlinnPhong();
//    test_onePointLight_oneCube_BlinnPhong_2();
    
//    test_depths();
//    test_depths_2();
//    test_stencil();
//    test_stencil_2();
//    test_stencil_3();
//    test_stencil_4();
    
//    test_blend_1();
//    test_blend_2();
//        test_text1(); //not working on ios
    
//    test_facecull();
//    test_facecull_2();
//    test_sprite_1();
//    test_sprite_2();
//    test_quad_1();
    
//    test_framebuffer();
//    test_framebuffer_mirror();
//    test_framebuffer_kernel();
    
//    test_skybox_1();
//    test_skybox_reflect();
//    test_skybox_reflect_2();
//    test_skybox_reflect_3();
//    test_skybox_refract_1();
//    test_ubo_1();
//    test_gs_1();
//    test_gs_2();
//    test_gs_model_explode();
//    test_gs_model_show_normals();
//    test_gs_model_gen2();
//    test_sprite_gs();
    
//    test_cubeIns_cube_0();
//    test_cubeIns_cube_1();
//    test_cubeIns_cube_2();
//    test_cubeIns_model_0();
//    test_cubeIns_model_1();
//    test_cubeIns_model_2();
//    test_cubeIns_asteroid_1();
//    test_cubeIns_asteroid_2();
//    test_msaa_1();
//    test_msaa_framebuffer_no_msaa();
//    test_msaa_framebuffer_normal();
//    test_msaa_framebuffer_texture();
    
//    test_gamma_1();
//    test_gamma_2();
//    test_shadow_mapping_1();
//    test_shadow_mapping_to_tex();
//    test_shadow_mapping_normal();
//    test_shadow_mapping_use_pass();
//    test_shadow_mapping_shadow_static();
//    test_shadow_mapping_light_move();
//    test_shadow_cubemap_light_move();
    
//    test_normal_mapping_1();
//    test_normal_mapping_2();
//    test_normal_mapping_3();
//     test_normal_mapping_model();
//    test_parallax_mapping_1();
//    test_HDR_1();
//    test_HDR_2();
//    test_bloom_1();
//    test_deferred_shading();
//    test_deferred_shading_2();
//    test_deferred_shading_volumn();
//    test_ssao_0();
//    test_ssao();
//    test_PBR_cubeColor();
//    test_PBR_cubeTex();
    
//    test_PBR_ibl_1();
//    test_PBR_ibl_irradiance();
//    test_PBR_ibl_irradiance2();
//    test_PBR_ibl_diffuse_specular();
//    test_PBR_ibl_diffuse_specular_ball2();
//    test_PBR_ibl_diffuse_model_fbx_gun();
    
//    test_quad_color();
    test_oit_quad_color();
//    test_ani1();
}
