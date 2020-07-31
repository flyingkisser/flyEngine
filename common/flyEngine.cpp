//
//  flyEngine.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "flyEngine.h"

textureMgr* g_textureMgr;
render* g_render;

void engineInit(){
    g_textureMgr=textureMgr::getInstance();
    g_render=render::getInstance();
    g_render->start();
}
