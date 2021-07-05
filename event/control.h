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

using namespace std;
class camera;

namespace flyEngine {

class control
{
private:
    glm::mat4 _matModel;
    glm::mat4 _matModelOrigin;
    string _matNameModel;
    
    float _move_d=0.1;
    float _mouseLeftOriginX=0;
    float _mouseLeftOriginY=0;
    float _mouseRightOriginX=0;
    float _mouseRightOriginY=0;
    float _width2PI=4000;
    float _height2PI=4000;
    float _yaw=-90;
    float _pitch=0;
    
    int _program=0;
    
    camera* _camera;
    
    
public:
    control(){};
    void bindCamera(camera* c);
};

}

#endif /* _control_h */
