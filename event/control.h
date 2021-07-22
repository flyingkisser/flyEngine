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

#include "camera.h"
#include "node.h"

using namespace std;


NS_FLYENGINE_BEGIN

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
    camera* _camera=NULL;
    keyboardEvent* _kbEventObj=NULL;
    mouseEvent* _msEventObj=NULL;
    node* _bindNode=NULL;
    
public:
    control(){};
    void bindCamera(camera* c);
    void bindNode(node* nodeObj);
};

NS_FLYENGINE_END

#endif /* _control_h */
