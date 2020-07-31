//
//  node.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "node.h"
#include "2dTrans.h"

node::node(){
    _pos.x=0;
    _pos.y=0;
    _refCount=0;
}

node::~node(){
    
}

p2 node::getPosition(){
    return _pos;
}

void node::setPosition(float x,float y){
    _pos.x=x;
    _pos.y=y;
}
void node::setPosition(p2* pos){
    _pos.x=pos->x;
    _pos.y=pos->y;
}

void node::addChild(node* c){
    _vectorChild.push_back(c);
}

void node::move(float x,float y){
    _2dTrans::move(x,y);
    _dirty=1;
}


