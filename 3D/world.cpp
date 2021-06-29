//
//  world.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/27.
//  Copyright © 2021 joe. All rights reserved.
//

#include "world.h"
world::world(){
    _camera=NULL;
}

void world::addChild(node *node){
    _vector_child.push_back(node);
}

void world::start(){
    threadUtil::createThread(world::main_loop);
}

void world::setCamera(camera* c){
    _camera=c;
}

void world::draw(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    for(auto c : _vector_child){
        node* nodeObj=c;
        nodeObj->draw(_camera);
    }
}

void world::main_loop(){
    while (true) {
        world::draw();
        threadUtil::sleep(1);
    }
}

void world::pause(){
    
}

void world::end(){
    
}
