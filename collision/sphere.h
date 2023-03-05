//
//  sphere.h
//  flyEngine
//
//  Created by joe on 03/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef sphere_h
#define sphere_h
#include "defines.h"
#include "volume.h"
#include "node.h"
#include "plane.h"
#include <math.h>

NS_FLYENGINE_BEGIN

struct sphere:public volume{
    glm::vec3 center={0.0f,0.0f,0.0f};
    float radius=0.0f;
    
    sphere(glm::vec3& pos,float r):volume{}{
        center=pos;
        radius=r;
    }
    
    bool isOnOrForwardPlane(plane &p) const final {
        return p.getSignedDistanceToPlane(center) > (-radius);
    }
    
    bool isOnFrustum(frustum& camFrustum,node* nodeObj) const final {
        glm::vec3 scale=nodeObj->getScale();
        glm::vec3 globalCenter=nodeObj->calcModelMatrix()*glm::vec4(center,1.0);
        float maxScale=std::max(std::max(scale.x,scale.y),scale.z);
        sphere globalSphere(globalCenter,radius*(maxScale*0.5f));
        return globalSphere.isOnOrForwardPlane(camFrustum.left) &&
        globalSphere.isOnOrForwardPlane(camFrustum.right) &&
        globalSphere.isOnOrForwardPlane(camFrustum.far) &&
        globalSphere.isOnOrForwardPlane(camFrustum.near) &&
        globalSphere.isOnOrForwardPlane(camFrustum.top) &&
        globalSphere.isOnOrForwardPlane(camFrustum.bottom);
    };
};

NS_FLYENGINE_END
#endif /* sphere_h */
