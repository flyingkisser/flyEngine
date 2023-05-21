//
//  test_shadow_mapping.h
//  flyEngine
//
//  Created by joe on 09/10/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef test_shadow_mapping_h
#define test_shadow_mapping_h

#include <stdio.h>


void test_shadow_mapping_to_tex();
void test_shadow_mapping_normal();

void test_shadow_mapping_normal_multi_shadow_by_fbo();
void test_shadow_mapping_normal_multi_shadow_sampler2DArray();


void test_shadow_mapping_light_move();
void test_shadow_by_cubemap();


#endif /* test_shadow_mapping_h */
