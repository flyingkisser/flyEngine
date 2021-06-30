//
//  world.h
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/27.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef world_h
#define world_h

#include <stdio.h>
#include "node.h"

namespace flyEngine {

class world{
private:
    std::vector<node*> _vector_child;
    camera* _camera;
    
public:
    world();
    ~world();
    static world* getInstance();
    void addChild(node* node);
    void setCamera(camera* c);
    void start();
    void pause();
    void end();
    
    static void main_loop();
    
    void draw();
};

}


#endif /* world_h */
