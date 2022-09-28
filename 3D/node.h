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
class material2;
class node: public glRef
{
protected:
    glm::mat4 _matModel=glm::mat4(1.0f);
    glm::mat4 _matModelOrigin=glm::mat4(1.0f);
    
    glm::vec3 _pos=glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 _scale=glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 _rotate=glm::vec3(0.0f,0.0f,0.0f);
  
    bool _bDisableLogState=false;
    bool _dirtyPos=false;
    bool _dirtyMT=false;
    bool _dirtyUBO=false;
    
    shader* _shaderObj=NULL;
    material2* m_material=NULL;
    // material2* m_material2=NULL;

    unsigned int _gl_program=0;
    unsigned int _gl_vao=0;
    unsigned int _gl_vbo=0;
   
 
    bool _bVisible=true;
    float* _vertice_arr=nullptr;
    int _vertice_arr_size=0;
    int* _desc_arr=NULL;
    int _desc_arr_size=0;
    
public:
    node();
    ~node(){};
    
    virtual bool init()=0;
    virtual void draw()=0;
    void print();
    void updateModel();
    
    void initVAO(float* arr,int arrSize,int descArr[],int descArrSize);
    
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
    glm::vec3& getRotation(){return _rotate;};
    
    void setRotation(glm::vec3 v);
    void setScale(glm::vec3 v);
    void setScale(float v);
    
    void moveBy(glm::vec3 v);
    void scaleBy(glm::vec3 v);
    void rotateBy(glm::vec3 v);
    
    void setVisible(bool s){_bVisible=s;};
    bool visible(){return _bVisible;};
    
    bool isDirtyPos(){return _dirtyPos;};
    void setDirtyPos(bool v){_dirtyPos=v;};
    
    bool isDirtyMT(){return _dirtyMT;};
    void setDirtyMT(bool v){_dirtyMT=v;};
    
    bool isDirtyUBO(){return _dirtyUBO;};
    void setDirtyUBO(bool v){_dirtyUBO=v;};
    
    void setLogInStatebool(bool s){_bDisableLogState=!s;};
    bool isDisableLogState(){return _bDisableLogState;};
    
    void runAction(action* act);
    void stopAction(action* act);
    
    void setMaterial(material2* mt);
    material2* getMaterial(){return m_material;};
    // material2* getMaterial2(){return m_material2;};
    
    shader* getShader(){return _shaderObj;};
    void setShader(shader* shaderObj);
    glm::mat4 getModelMatrix(){return _matModel;};
};

NS_FLYENGINE_END


#endif /* _node_h */


