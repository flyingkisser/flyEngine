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
#include <string>
#include <functional>
#include "defines.h"

using namespace std;

NS_FLYENGINE_BEGIN

class control;
class shader;

class camera{
    
public:
    camera();
    
    float getPositionX(){return _cameraPos.x;};
    float getPositionY(){return _cameraPos.y;};
    float getPositionZ(){return _cameraPos.z;};
    glm::vec3 getPosition(){return _cameraPos;};
    glm::vec3 getFront(){return _cameraFront;};
    float getYaw(){return _yaw;};
    float getPitch(){return _pitch;};
    float getFov(){return _fov;};

    void setProgrameID(int program){_program=program;};
    void setPosition(glm::vec3 v);
    void setPositionX(float v);
    void setPositionY(float v);
    void setPositionZ(float v);
 
    void setYaw(float v);
    void setPitch(float v);
    
    void setFocusPos(glm::vec3 p){_focusPos=glm::normalize(p);_bFocus=true;_dirtyPos=true;};
    void disableFocus(){_bFocus=false;};

    void moveBy(glm::vec3 v);
    void rotate(glm::vec3 v);
    
    void reset();
    void print();
    
    bool init();

    void update();
    void update2D();
    
    void enableControl();
    control* getControl();
    
    glm::mat4 getLookAtMatrix();
    glm::mat4 getPerspectiveMatrix();
    glm::mat4 getLightSpaceMat();
    
    void initUBO();
    void updateUBOForShadow(glm::vec3 lightPos);
    
private:
    void _updateCamera();
    void _updateProjection();
    void _updateUBO();
    
    void _updateFront();
    
    control* _controlObj;
    shader* _shaderObj;
    
    glm::mat4 _matProjPerspective;
    glm::mat4 _matProjPerspectiveOrigin;
    glm::mat4 _matProjOrtho;
    glm::mat4 _matCamera;
    glm::mat4 _matCameraOrigin;
    
    glm::vec3 _cameraPos;
    glm::vec3 _cameraPosOrigin;
    glm::vec3 _cameraFront;
    glm::vec3 _cameraUp;
    glm::vec3 _focusPos;
    bool _bFocus;
    
    bool _dirtyPos;
    int _program=0;
    float _yaw=0;
    float _pitch=0;
    float _fov=0;
    float _fovOrigin=0;
    float _screenRatio=0.0;
    
    int _ubo_mat_3d=0;
    int _ubo_mat_2d=0;
    int _ubo_mat_3d_shadow=0;
};

NS_FLYENGINE_END
#endif /* camera_h */
