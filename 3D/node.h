//
//  Node.h
//  flyEngine
//
//  Created by joe on 24/06/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef _node_h
#define _node_h

#include <stdio.h>
#include <string>
#include <functional>

#include "defines.h"
#include "glRef.h"

using namespace std;

NS_FLYENGINE_BEGIN

class camera;
class action;
class texture;
class shader;

class node: public glRef
{
private:
    glm::mat4 _matModel;
    glm::mat4 _matModelOrigin;
    
    glm::vec3 _pos=glm::vec3(0,0,0);
    glm::vec3 _scale=glm::vec3(1,1,1);
    glm::vec3 _rorate=glm::vec3(0,0,0);
    
    unsigned int _gl_program=0;
    unsigned int _gl_texture0=0;
    unsigned int _gl_vao=0;
    bool _dirtyPos=false;
    
    texture* _texObj;
    shader* _shaderObj;
    
    const char* _texPath;
    
//    bool initTexture(const char* texPath);
//    bool initShader(int id);
//    bool initShader(const char* vsPath,const char* fsPath);

public:
    ~node(){};
    node(const char* texPath);
    bool init();
    
    void glInit();
   
    void updateModel();
    void print();
    void draw(camera* cameraObj);
    
    void setPosition(glm::vec3 p);
    void setPositionX(float v);
    void setPositionY(float v);
    void setPositionZ(float v);
    
    glm::vec3& getPosition(){return _pos;};
    float getPositionX(){return _pos.x;};
    float getPositionY(){return _pos.y;};
    float getPositionZ(){return _pos.z;};
    
    void setScale(glm::vec3 v);
    
    
    void rotate(glm::vec3 v);
    void moveBy(glm::vec3 v);
    
    void runAction(action* act);

};

NS_FLYENGINE_END


#endif /* _node_h */


