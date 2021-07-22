//
//  kbmControl.h
//  flyEngine
//
//  Created by joe on 11/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef _kbmControl_h
#define _kbmControl_h

#include <stdio.h>
#include <string>
using namespace std;

class kbmControl
{
private:
    glm::mat4 _matProj;
    glm::mat4 _matProjOrigin;
    string _matNameProj;
    
    glm::mat4 _matModel;
    glm::mat4 _matModelOrigin;
    string _matNameModel;
    
    glm::mat4 _matView;
    glm::mat4 _matViewOrigin;
    string _matNameView;
    
    float _move_d=0.2;
    float _mouseLeftOriginX=0;
    float _mouseLeftOriginY=0;
    float _mouseRightOriginX=0;
    float _mouseRightOriginY=0;
    float _width2PI=4000;
    float _height2PI=4000;
    
    int _shaderID=0;
    
    
public:
    kbmControl(int shader,
               string matNameModel,glm::mat4 matModel,
               string matNameView,glm::mat4 matView,
               string matNameProj,glm::mat4 matProj);
};

#endif /* _kbmControl_h */
