//
//  defines.h
//  lines
//
//  Created by joe on 30/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef defines_h
#define defines_h

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define NS_FLYENGINE_BEGIN namespace flyEngine {;
#define NS_FLYENGINE_END };
#define USE_NS_FLYENGINE using namespace flyEngine;

#define CONST_FRAME_RATE 0.0167


#include "types.h"
#include "error.h"

extern float g_verticeArrWithTexCoord[180];
extern float g_verticeArr[108];
extern float g_verticeArrWithTexCoordAndNormal[288];

#define uniform_name_camera_pos "camera_pos"
#define uniform_name_mat_proj   "matProj"
#define uniform_name_mat_camera "matCamera"
#define uniform_name_mat_model  "matModel"
#define uniform_name_material_ambient   "material_ambient"
#define uniform_name_material_diffuse   "material_diffuse"
#define uniform_name_material_specular  "material_specular"
#define uniform_name_material_shininess "material_shininess"

#define uniform_name_light_ambient_color "ambient_color"

#define uniform_name_light_pos_0 "light_pos_0"
#define uniform_name_light_pos_1 "light_pos_1"
#define uniform_name_light_pos_2 "light_pos_2"
#define uniform_name_light_pos_3 "light_pos_3"
#define uniform_name_light_pos_4 "light_pos_4"
#define uniform_name_light_pos_5 "light_pos_5"
#define uniform_name_light_pos_6 "light_pos_6"
#define uniform_name_light_pos_7 "light_pos_7"
#define uniform_name_light_pos_8 "light_pos_8"
#define uniform_name_light_pos_9 "light_pos_9"
#define uniform_name_light_pos_10 "light_pos_10"
#define uniform_name_light_pos_11 "light_pos_11"
#define uniform_name_light_pos_12 "light_pos_12"
#define uniform_name_light_pos_13 "light_pos_13"
#define uniform_name_light_pos_14 "light_pos_14"
#define uniform_name_light_pos_15 "light_pos_15"
#define uniform_name_light_pos_16 "light_pos_16"
#define uniform_name_light_pos_17 "light_pos_17"
#define uniform_name_light_pos_18 "light_pos_18"
#define uniform_name_light_pos_19 "light_pos_19"
#define uniform_name_light_pos_20 "light_pos_20"
#define uniform_name_light_pos_21 "light_pos_21"
#define uniform_name_light_pos_22 "light_pos_22"
#define uniform_name_light_pos_23 "light_pos_23"

#endif /* defines_h */
