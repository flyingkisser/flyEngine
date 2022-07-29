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

extern float g_verticeArr[108];
extern float g_verticeArrWithTexCoord[180];
extern float g_verticeArrWithNormal[216];
extern float g_verticeArrWithTexCoordAndNormal[288];

extern float g_verticeArrWithTexCoord_plane[30];
extern float g_verticeArrWithTexCoord_2d[24];
extern float g_verticeArr_skybox[108];

#define uniform_name_camera_pos "camera_pos"
#define uniform_name_mat_proj   "matProj"
#define uniform_name_mat_camera "matCamera"
#define uniform_name_mat_model  "matModel"

#define uniform_name_material_enabled   "mt.enabled"
#define uniform_name_material_ambient   "mt.ambient"
#define uniform_name_material_diffuse   "mt.diffuse"
#define uniform_name_material_specular_tex   "mt.specular_tex"
#define uniform_name_material_specular  "mt.specular"
#define uniform_name_material_shininess "mt.shiness"

//#define uniform_name_global_ambient_color "global_ambient_color"
//direction light
#define uniform_name_light_direction_enabled  "light_direction.enabled"
#define uniform_name_light_direction_direction"light_direction.direction"
#define uniform_name_light_direction_color    "light_direction.color"
#define uniform_name_light_direction_ambient  "light_direction.ambient"
#define uniform_name_light_direction_diffuse  "light_direction.diffuse"
#define uniform_name_light_direction_specular "light_direction.specular"
#define uniform_name_light_direction_shiness  "light_direction.shiness"

//point light
#define uniform_name_light_point_enabled  "light_point_arr[%d].enabled"
#define uniform_name_light_point_pos      "light_point_arr[%d].pos"
#define uniform_name_light_point_color    "light_point_arr[%d].color"
#define uniform_name_light_point_ambient  "light_point_arr[%d].ambient"
#define uniform_name_light_point_diffuse  "light_point_arr[%d].diffuse"
#define uniform_name_light_point_specular "light_point_arr[%d].specular"
#define uniform_name_light_point_shiness  "light_point_arr[%d].shiness"
#define uniform_name_light_point_constant "light_point_arr[%d].constant"
#define uniform_name_light_point_linear   "light_point_arr[%d].linear"
#define uniform_name_light_point_quadratic"light_point_arr[%d].quadratic"


//spot light
#define uniform_name_light_spot_enabled  "light_spot_arr[%d].enabled"
#define uniform_name_light_spot_pos      "light_spot_arr[%d].pos"
#define uniform_name_light_spot_color    "light_spot_arr[%d].color"
#define uniform_name_light_spot_ambient  "light_spot_arr[%d].ambient"
#define uniform_name_light_spot_diffuse  "light_spot_arr[%d].diffuse"
#define uniform_name_light_spot_specular "light_spot_arr[%d].specular"
#define uniform_name_light_spot_constant "light_spot_arr[%d].constant"
#define uniform_name_light_spot_linear "light_spot_arr[%d].linear"
#define uniform_name_light_spot_quadratic "light_spot_arr[%d].quadratic"
#define uniform_name_light_spot_cutoff_inner "light_spot_arr[%d].cutoff_inner"
#define uniform_name_light_spot_cutoff_outer "light_spot_arr[%d].cutoff_outer"
#define uniform_name_light_spot_direction "light_spot_arr[%d].direction"

#endif /* defines_h */
