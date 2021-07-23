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


extern float g_verticeArr[180];

#endif /* defines_h */
