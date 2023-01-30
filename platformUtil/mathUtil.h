//
//  mathUtil.h
//  flyEngine
//
//  Created by joe on 21/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef mathUtil_h
#define mathUtil_h

#include <stdio.h>
#include "assimp/quaternion.h"
#include "assimp/vector3.h"
#include "assimp/matrix4x4.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

class mathUtil{
public:
    static int ceil(float v);
    static int floor(float v);
    static int round(float v);
    static float max(float v1,float v2);
    static int max(int v1,int v2);
    static float min(float v1,float v2);
    static int min(int v1,int v2);
    
    static float abs(float v);
    static int abs(int v);
    static float sqrt(float v);
    static float pow(float v,float s);
    static void extendPosRange(float* arr,int rows,int columns,float v);
    static float lerp(float y1,float y2,float weight);
    static void test();
    
    static glm::mat4 convertMatrixToGLMFormat(const aiMatrix4x4& from);
    static glm::vec3 getGLMVec(const aiVector3D& vec);
    static glm::quat getGLMQuat(const aiQuaternion& pOrientation);

};

#endif /* mathUtil_h */
