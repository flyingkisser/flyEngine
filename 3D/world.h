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
#include "defines.h"


NS_FLYENGINE_BEGIN

class node;
class control;
class camera;

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

NS_FLYENGINE_END


#endif /* world_h */
