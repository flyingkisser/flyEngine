//
//  flyEngine.h
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef flyEngine_h
#define flyEngine_h

#include "defines.h"

//render
#include "render.h"
#include "VAOMgr.h"

//base
#include "node.h"
#include "matrix.h"
#include "glRef.h"

//ui
#include "ui.h"

//texture
#include "bmpUtil.h"
#include "pngUtil.h"
#include "jpgUtil.h"
#include "texture.h"
#include "textureMgr.h"



//2d
#include "2DTrans.h"

//3d
#include "3DTrans.h"
#include "node.h"

//shader
#include "shader.h"

//platform utils
#include "platformUtil.h"

//events
#include "keyboardEventMgr.h"
#include "mouseEventMgr.h"

//control
#include "control.h"

//camera
#include "camera.h"



//world
#include "world.h"

using namespace flyEngine;

void engineInit();

extern float g_verticeArr[180];
extern int g_winWidth;
extern int g_winHeight;

#endif /* flyEngine_h */
