//
//  camera.h
//  flyEngine
//
//  Created by wu mingzhou on 2021/7/4.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef camera_h
#define camera_h

#include <stdio.h>


#include <stdio.h>

#include "flyEngine.h"
#include <string>
#include <functional>
using namespace std;

namespace flyEngine {

class camera{
    
public:
    camera();
    
    float getPositionX(){return _cameraPos.x;};
    float getPositionY(){return _cameraPos.y;};
    float getPositionZ(){return _cameraPos.z;};
    
    void setProgrameID(int program){_program=program;};
    void setPosition(glm::vec3 v);
    void setPositionX(float v);
    void setPositionY(float v);
    void setPositionZ(float v);
    void setPositionFront(glm::vec3 v);

    void moveBy(glm::vec3 v);
    void rotate(glm::vec3 v);
    
    void reset();
    void print();
    

    bool init();
//    void glInit(int programID);
    
    void use(int programID);
   
private:
    void _updateCamera();
    void _updateProjection();
    
    glm::mat4 _matProj;
    glm::mat4 _matProjOrigin;
    glm::mat4 _matCamera;
    glm::mat4 _matCameraOrigin;
    
    glm::vec3 _cameraPos;
    glm::vec3 _cameraFront;
    glm::vec3 _cameraUp;
    
    bool _dirtyPos;
    int _program=0;
    float _yaw=0;
    float _pitch=0;
    float _fov=0;
    float _fovOrigin=0;
    float _screenRatio=0.0;
    
};

}
#endif /* camera_h */
