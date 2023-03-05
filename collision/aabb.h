//
//  aabb.h
//  flyEngine
//
//  Created by joe on 03/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef aabb_h
#define aabb_h

#include "defines.h"
#include "volume.h"
#include "node.h"
#include "plane.h"
#include <math.h>

NS_FLYENGINE_BEGIN

struct aabb:public volume{
    glm::vec3 center={0.0f,0.0f,0.0f};
    glm::vec3 extents={0.0f,0.0f,0.0f};

    aabb(glm::vec3& min,glm::vec3& max):volume{}{
        center=glm::vec3((min+max)*0.5f);
        extents=glm::vec3(max.x-center.x,max.y-center.y,max.z-center.z);
    }
    
    aabb(glm::vec3 centerPos,float li,float ly,float lz){
        center=centerPos;
        extents=glm::vec3(li,ly,lz);
    }

    bool isOnOrForwardPlane(plane &p) const final {
        glm::vec3 absNormal=glm::vec3(abs(p.normal.x),std::abs(p.normal.y),std::abs(p.normal.z));
        float r=glm::dot(extents,absNormal);
        return p.getSignedDistanceToPlane(center) >= -r;
    }
    
    bool isOnFrustum(frustum& camFrustum,node* nodeObj) const final {
        glm::mat4 modelMatrix=nodeObj->calcModelMatrix();
        glm::vec3 globalCenter=modelMatrix*glm::vec4(center,1.0);
        glm::vec3 right=modelMatrix[0]*extents.x;
        glm::vec3 up=modelMatrix[1]*extents.y;
        glm::vec3 forward=-modelMatrix[2]*extents.z;

        float newLi=std::abs(glm::dot(glm::vec3(1,0,0),right))+std::abs(glm::dot(glm::vec3(1,0,0),up))+
        std::abs(glm::dot(glm::vec3(1,0,0),forward));
        float newLj=std::abs(glm::dot(glm::vec3(0,1,0),right))+std::abs(glm::dot(glm::vec3(0,1,0),up))+
        std::abs(glm::dot(glm::vec3(0,1,0),forward));
        float newLk=std::abs(glm::dot(glm::vec3(0,0,1),right))+std::abs(glm::dot(glm::vec3(0,0,1),up))+
        std::abs(glm::dot(glm::vec3(0,0,1),forward));

        aabb globalaabb(globalCenter,newLi,newLj,newLk);
        return globalaabb.isOnOrForwardPlane(camFrustum.left) &&
        globalaabb.isOnOrForwardPlane(camFrustum.right) &&
        globalaabb.isOnOrForwardPlane(camFrustum.far) &&
        globalaabb.isOnOrForwardPlane(camFrustum.near) &&
        globalaabb.isOnOrForwardPlane(camFrustum.top) &&
        globalaabb.isOnOrForwardPlane(camFrustum.bottom);
    };
};

NS_FLYENGINE_END

#endif /* aabb_h */
