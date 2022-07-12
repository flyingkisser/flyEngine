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

#include "keyboardEvent.h"
#include "mouseEvent.h"

using namespace std;

NS_FLYENGINE_BEGIN

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
    
    float _cameraMoveNodeZ=-3;
    
    camera* _camera=NULL;
    node* _bindNode=NULL;
    node* _bindNodeWithCameraMove=NULL;
    
    keyboardEvent* _kbEventObj=NULL;
    mouseEvent* _msEventObj=NULL;
    
    
public:
    control(){};
    void bindCamera(camera* c);
    void bindNode(node* nodeObj);
    //节点一直保持和相机同样的位置
    void bindNodeWithCameraMove(flyEngine::node* nodeObj);
    void regOnKeyPress(char key, std::function<void ()> cb);
};

NS_FLYENGINE_END

#endif /* _control_h */
