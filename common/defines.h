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


#define NS_FLYENGINE_BEGIN namespace flyEngine {;
#define NS_FLYENGINE_END };
#define USE_NS_FLYENGINE using namespace flyEngine;

#include "types.h"
#include "values.h"
#include "error.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#ifdef BUILD_MAC
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#elif BUILD_IOS
#include <OpenGLES/ES3/glext.h>
#endif

#endif /* defines_h */
