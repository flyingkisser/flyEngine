//
//  values.h
//  flyEngine
//
//  Created by joe on 15/08/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef values_h
#define values_h

#define CONST_FRAME_RATE 0.0167

#ifdef BUILD_IOS
#define GL_RENDER 0x1C00
#define GL_DOUBLE 0x140A
#define GL_DOUBLE_VEC2 0x8FFC
#define GL_DOUBLE_VEC3 0x8FFD
#define GL_DOUBLE_VEC4 0x8FFE
#define GL_DOUBLE_MAT2 0x8F46
#define GL_DOUBLE_MAT3 0x8F47
#define GL_DOUBLE_MAT4 0x8F48
#define GL_DOUBLE_MAT2x3 0x8F49
#define GL_DOUBLE_MAT2x4 0x8F4A
#define GL_DOUBLE_MAT3x2 0x8F4B
#define GL_DOUBLE_MAT3x4 0x8F4C
#define GL_DOUBLE_MAT4x2 0x8F4D
#define GL_DOUBLE_MAT4x3 0x8F4E
#define GL_SAMPLER_1D 0x8B5D
#define GL_SAMPLER_1D_SHADOW 0x8B61
#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_SAMPLER_2D_RECT 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW 0x8B64
#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_INT_SAMPLER_2D_RECT 0x8DCD
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT 0x8DD5
#define GL_FRAMEBUFFER_SRGB 0x8DB9
#endif

#define uniform_name_camera_pos "camera_pos"
#define uniform_name_mat_proj   "matProj"
#define uniform_name_mat_camera "matCamera"
#define uniform_name_mat_model  "matModel"
#define uniform_name_mat_model_arr  "matModelArr"

#define uniform_name_material_enabled   "mt.enabled"
#define uniform_name_material_ambient   "mt.ambient"
#define uniform_name_material_diffuse   "mt.diffuse"
#define uniform_name_material_specular  "mt.specular"
#define uniform_name_material_shininess "mt.shiness"
#define uniform_name_material_sp_tex_enabled   "mt.sp_tex_enabled"
#define uniform_name_material_specular_tex   "mt.specular_tex"


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

//area light
#define uniform_name_light_area_enabled  "light_area_arr[%d].enabled"
#define uniform_name_light_area_pos      "light_area_arr[%d].pos"
#define uniform_name_light_area_color    "light_area_arr[%d].color"
#define uniform_name_light_area_points  "light_area_arr[%d].points"
#define uniform_name_light_area_intensity  "light_area_arr[%d].intensity"
#define uniform_name_light_area_twoSided "light_area_arr[%d].twoSided"
#define uniform_name_light_area_roughness "light_area_arr[%d].roughness"

//ubo binding point index
//on ios binding point max is 23,otherwise will not work!!

#define ubo_binding_mat_3d 0
#define ubo_binding_mat_2d 1
#define ubo_binding_mat_3d_shadow 2
#define ubo_binding_light_dir 10
#define ubo_binding_light_point 11
#define ubo_binding_light_spot 12
#define ubo_binding_light_area 13

#define ubo_size_mat_3d 140
#define ubo_size_mat_3d_shadow 140
#define ubo_size_mat_2d 140

#define ubo_size_light_dir 64
#define ubo_size_light_dir_arr 256
#define ubo_size_light_point 112
#define ubo_size_light_point_arr 448
#define ubo_size_light_spot 128
#define ubo_size_light_spot_arr 512
#define ubo_size_light_area 48
#define ubo_size_light_area_arr 192

//texture id
#define texture0 0
#define mt_specular_tex 1
#define texture_specular_mesh_0 2
#define texture_normal_mesh_0 3
#define texture_shadow 4
#define texture_depth_cube 5

extern float g_verticeArr[108];
extern float g_verticeArrWithTexCoord[180];
extern float g_verticeArrWithNormal[216];
extern float g_verticeArrWithTexCoordAndNormal[288];
extern float g_verticeArrWithNormalAndTexCoord[288];

extern float g_verticeArrWithTexCoordAndNormalReverse[288];

extern float g_verticeArrWithTexCoord_plane[30];
extern float g_verticeArrWithTexCoordAndNormal_plane[54];
extern float g_verticeArrWithTexCoordAndNormal_plane3[54];
extern float g_verticeArrWithTexCoordAndNormal_plane_areaLight[54];
extern float g_verticeArrWithTexCoord_2d[24];
extern float g_verticeArrWithTexCoord_quad[20];
extern float g_verticeArrWithTexCoord_quadColor[30];

extern float g_verticeArr_skybox[108];


#endif /* values_h */
