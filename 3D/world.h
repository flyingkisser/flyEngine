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
#include "defines.h"
#include "control.h"
#include "camera.h"

namespace flyEngine {

class node;

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
    void start_rendering();
    void pause();
    void end();
    
    static void _main_loop();
    
    void draw();
    control* getControl();
    
    float getFrameRate(){return CONST_FRAME_RATE;};
};

}


#endif /* world_h */
