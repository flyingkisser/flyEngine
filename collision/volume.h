//
//  volume.h
//  flyEngine
//
//  Created by joe on 03/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef volume_h
#define volume_h
#include "defines.h"
#include "frustum.h"
#include "node.h"
#include "plane.h"

NS_FLYENGINE_BEGIN


//struct volume{
//    virtual bool isOnFrustum(frustum& camFrustum,node* node) const=0;
//    virtual bool isOnOrForwardPlane(plane& p) const=0;
//    bool isOnFrustum(frustum& camFrustum) const{
//        return isOnOrForwardPlane(camFrustum.left) &&
//        isOnOrForwardPlane(camFrustum.right) &&
//        isOnOrForwardPlane(camFrustum.top) &&
//        isOnOrForwardPlane(camFrustum.bottom) &&
//        isOnOrForwardPlane(camFrustum.near) &&
//        isOnOrForwardPlane(camFrustum.far);
//    }
//};

NS_FLYENGINE_END



#endif /* volume_h */
