//
//  quadColor.h
//  flyEngine
//
//  Created by Joe on 2023/1/8.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef quadColor_h
#define quadColor_h

#include <stdio.h>
#include "defines.h"
#include "sprite.h"

NS_FLYENGINE_BEGIN

class quadColor : public sprite{
private:
    void _reInitVertices();
    glm::vec3 _color;
    size _size=size{0,0};
    
public:
    quadColor(glm::vec3 color,float width,float height);
//    size getContentSize(){return _size;};
//     void setContentSize(size s);
    bool init();
    void glInit();
    void draw();
};

NS_FLYENGINE_END

#endif /* quadColor_h */
