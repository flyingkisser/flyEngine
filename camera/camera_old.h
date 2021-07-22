//
//  camera.h
//  flyEngine
//
//  Created by joe on 16/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef camera_h
#define camera_h

#include <stdio.h>
#include <string>
#include <functional>
using namespace std;

class camera
{
public:
    glm::mat4 _matProj;
    glm::mat4 _matProjOrigin;
    string _matNameProj;
    
    glm::mat4 _matModel;
    glm::mat4 _matModelOrigin;
    string _matNameModel;

    glm::vec3 _cameraPos;
    glm::vec3 _cameraFront;
    glm::vec3 _cameraUp;
    glm::mat4 _matCamera;
    glm::mat4 _matCameraOrigin;
    string _matNameCamera;
    
    float _move_speed=0.4;
    float _move_d=0.2;
    float _mouseLeftOriginX=0;
    float _mouseLeftOriginY=0;
    float _mouseRightOriginX=0;
    float _mouseRightOriginY=0;
    float _width2PI=4000;
    float _height2PI=4000;
    
    float _yaw=-90;
    float _pitch=0;
    
    float _fov=30.0;
    float _fovOrigin=30;
    float _screenRatio=0.0;
    
    int _shaderID=0;
    bool _modeFPS=false;
    
    std::function<void(void)> _cbUpdateCamera;
    
public:
    camera(){};
    camera(int shader,
        string matNameModel,glm::mat4 matModel,
        string matNameView,glm::vec3 cameraPos,glm::vec3 cameraUp,
        string matNameProj,float screenRatio);
    void updateCamera();
    void setFPSMode(){_modeFPS=true;};
    void print();
    
private:
    
    void updateProjection();
    void updateModel();
};

#endif /* camera_h */
