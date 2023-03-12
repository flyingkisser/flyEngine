//
//  plane.h
//  flyEngine
//
//  Created by joe on 27/02/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef plane_h
#define plane_h

#include "defines.h"
#include "logUtil.h"
NS_FLYENGINE_BEGIN

struct plane{
    glm::vec3 _normal={0.0f,1.0f,0.0f};
    glm::vec3 _pos;
    float _d;
    
    plane()=default;
    plane(glm::vec3 p,glm::vec3 norm,char* name){
        _normal=glm::normalize(norm);   //norm is the normal vector of the plane
        _pos=p;                 //p is the any point on this plane
        _d=-glm::dot(_normal,_pos);//d=-(Ax+By+Cz), (A,B,C) is the unit normal vector
        flylog("plane %s %f %f %f d %f",name,_normal.x,_normal.y,_normal.z,_d);
    }
    
    //p点与法向量在同一侧，结果为正，反之为负
    float getSignedDistanceToPlane(glm::vec3 p){
        float len=glm::dot(_normal,p-_pos)+_d;
        return len;
    }
};


NS_FLYENGINE_END

#endif /* plane_h */
