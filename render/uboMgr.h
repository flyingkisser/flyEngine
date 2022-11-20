//
//  uboMgr.h
//  flyEngine
//
//  Created by joe on 31/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef uboMgr_h
#define uboMgr_h

#include <stdio.h>
#include <vector>
#include <variant>
#include "defines.h"

USE_NS_FLYENGINE

extern unsigned int g_ubo_id_mat_2d;
extern unsigned int g_ubo_id_mat_3d;
extern unsigned int g_ubo_id_mat_3d_shadow;
//extern unsigned int g_ubo_id_light_dir0;
//extern unsigned int g_ubo_id_light_point0;
//extern unsigned int g_ubo_id_light_point1;
//extern unsigned int g_ubo_id_light_point2;
//extern unsigned int g_ubo_id_light_point3;
//extern unsigned int g_ubo_id_light_spot0;
//extern unsigned int g_ubo_id_light_spot1;
//extern unsigned int g_ubo_id_light_spot2;
//extern unsigned int g_ubo_id_light_spot3;

extern unsigned int g_ubo_id_arr[100];

class uboMgr {
public:
    static void initAllUbo();
    static void linkUBOAndBindPoint(int programID,const char* uboName,int bindPoint,bool debug=false);
    static int createUBO(int bindPointIndex,int uboSize,const char* uboName=NULL);
    static void writeData(unsigned int ubo,int num,int sizeArr[],int offsetArr[],void* bufArr[]);
};

#endif /* uboMgr_h */
