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

//shader
#include "shader.h"

//platform utils
#include "platformUtil.h"

//events
#include "keyboardEventMgr.h"
#include "mouseEventMgr.h"
#include "kbmControl.h"

//camera
#include "camera.h"

using namespace flyEngine;

void engineInit();

#endif /* flyEngine_h */
