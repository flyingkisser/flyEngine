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
class material;

class node: public glRef
{
private:
    glm::mat4 _matModel;
    glm::mat4 _matModelOrigin;
    
    glm::vec3 _pos=glm::vec3(0,0,0);
    glm::vec3 _scale=glm::vec3(1,1,1);
    glm::vec3 _rorate=glm::vec3(0,0,0);
    
public:
    shader* _shaderObj=NULL;
    material* m_material=NULL;
    unsigned int _gl_program=0;
    unsigned int _gl_vao=0;
    bool _dirtyPos=false;
    
public:
    node();
    ~node(){};
    
    virtual bool init()=0;
    virtual void glInit()=0;
    virtual void draw(camera* cameraObj)=0;
    void print();
    void updateModel(camera* cameraObj);
    
    void glInitVAO();
    void glInitVAOWithTexCoord();
    void glInitVAOWithTexCoordAndNormal();
    
    void glInitShader();
    bool glInitMaterial();
    void glUpdateLight();
    
    void setPosition(glm::vec3 p);
    void setPositionX(float v);
    void setPositionY(float v);
    void setPositionZ(float v);
    
    float getPositionX(){return _pos.x;};
    float getPositionY(){return _pos.y;};
    float getPositionZ(){return _pos.z;};
    glm::vec3& getPosition(){return _pos;};
    glm::vec3& getScale(){return _scale;};
    glm::vec3& getRotation(){return _rorate;};
    
    void setScale(glm::vec3 v);
    void setScale(float v);
    
    void moveBy(glm::vec3 v);
    void scaleBy(glm::vec3 v);
    void rotateBy(glm::vec3 v);
    
    bool isDirty(){return _dirtyPos;};
    
    void runAction(action* act);
    void stopAction(action* act);
    
    void setMaterial(material* mt);
    material* getMaterial(){return m_material;};
};

NS_FLYENGINE_END


#endif /* _node_h */


