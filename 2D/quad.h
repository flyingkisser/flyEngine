//
//  quad.h
//  flyEngine
//
//  Created by joe on 10/10/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef quad_h
#define quad_h

#include <stdio.h>
#include "defines.h"
#include "sprite.h"

NS_FLYENGINE_BEGIN

class quad : public sprite{
private:
    void _reInitVertices();
    
public:
    quad(const char* strImageFileName);
    quad(unsigned int texID);
    quad(unsigned int texID,int width,int height);
    bool init();
    void glInit();
    void draw();
    void setContentSize(flyEngine::size s);
};

NS_FLYENGINE_END


#endif /* quad_h */
