//
//  quadSize.h
//  flyEngine
//
//  Created by Joe on 2023/3/14.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef quadSize_h
#define quadSize_h

#include <stdio.h>
#include "defines.h"
#include "sprite.h"

NS_FLYENGINE_BEGIN

class quadSize : public sprite{
private:
    void _reInitVertices();
    
public:
    quadSize(const char* strImageFileName);
    quadSize(unsigned int texID,float width,float height);
    bool init();
    void glInit();
    void draw();
};

NS_FLYENGINE_END
#endif /* quadSize_h */
