//
//  nodeIns.h
//  flyEngine
//
//  Created by joe on 25/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef nodeIns_h
#define nodeIns_h

#include <stdio.h>
#include <vector>
#include "defines.h"
#include "node.h"

NS_FLYENGINE_BEGIN

class nodeIns:public node{
protected:
    int _insCount=0;
    std::vector<glm::vec3> _posVec;
    std::vector<glm::vec3> _rotationVec;
    std::vector<glm::vec3> _scaleVec;
    std::vector<glm::mat4> _matModelVec;
    
    std::vector<bool> _dirtyPosVec;
    std::vector<bool> _dirtyUBOVec;
    
    bool _safeIndex(int i);
    std::function <void()> _cb_before_draw=nullptr;
    unsigned int _insVBO=0;
    int _insVBOIndex=0;
    
public:
    nodeIns(int count);
    void setCBDrawCall(std::function<void()> f){_cb_before_draw=f;};
    
    void updateModel();
    void useInsByVBO(int attribIndex);
    void updateInsVBO();
    
    void setCount(int count);
    void setPosition(int i,glm::vec3 p);
    void setPositionX(int i,float v);
    void setPositionY(int i,float v);
    void setPositionZ(int i,float v);
    void setRotation(int i,glm::vec3 v);
    void setScale(int i,glm::vec3 v);
    void setScale(int i,float v);

    void moveBy(int i,glm::vec3 v);
    void scaleBy(int i,glm::vec3 v);
    void rotateBy(int i,glm::vec3 v);
    
    void rotateBy(glm::vec3 v);
    void setPosition(glm::vec3 v);
    void setRotation(glm::vec3 v);
    void setPositionX(float v);
    void setPositionY(float v);
    void setPositionZ(float v);
};

NS_FLYENGINE_END

#endif /* nodeIns_h */
