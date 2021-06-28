//
//  world.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/27.
//  Copyright © 2021 joe. All rights reserved.
//

#include "world.h"
world::world(){
    
}

void world::addChild(node *node){
    _vector_child.push_back(node);
}

void world::start(){
    threadUtil::createThread(world::main_loop);
}

void world::draw(){
    for(auto c : _vector_child){
        node* nodeObj=c;
        nodeObj->draw(NULL);
    }
}

void world::main_loop(){
    while (true) {
        world::draw();
        threadUtil::sleep(0.01);
    }
}

void world::pause(){
    
}

void world::end(){
    
}
