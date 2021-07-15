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

using namespace std;


namespace flyEngine {

class camera;

class control
{
private:
    float _move_d=0.1;
    float _mouseLeftOriginX=0;
    float _mouseLeftOriginY=0;
    float _mouseRightOriginX=0;
    float _mouseRightOriginY=0;
    float _width2PI=4000;
    float _height2PI=4000;
    float _yaw=-90;
    float _pitch=0;
    flyEngine::camera* _camera;
public:
    control(){};
    void bindCamera(flyEngine::camera* c);
};

}

#endif /* _control_h */
