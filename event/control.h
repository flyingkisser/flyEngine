//
//  control.h
//  flyEngine
//
//  Created by joe on 11/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef _control_h
#define _control_h

#include <stdio.h>
#include <string>

#include "flyEngine.h"
#include "camera.h"
#include "node.h"
#include "keyboardEvent.h"
#include "mouseEvent.h"
using namespace std;


namespace flyEngine {

class camera;
class node;

class control
{
private:
    float _move_d=0.1;
    float _mouseLeftLastX=0;
    float _mouseLeftLastY=0;
    
    float _mouseRightOriginX=0;
    float _mouseRightOriginY=0;
    float _mouseRightLastX=0;
    float _mouseRightLastY=0;

    float _width2PI=4000;
    float _height2PI=4000;
    float _yaw=-90;
    float _pitch=0;
    flyEngine::camera* _camera=NULL;
    keyboardEvent* _kbEventObj=NULL;
    mouseEvent* _msEventObj=NULL;
    node* _bindNode=NULL;
    
public:
    control(){};
    void bindCamera(flyEngine::camera* c);
    void bindNode(flyEngine::node* nodeObj);
};

}

#endif /* _control_h */
