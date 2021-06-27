//
//  3DNode.h
//  flyEngine
//
//  Created by joe on 24/06/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef _DNode_h
#define _DNode_h

#include <stdio.h>

#include "flyEngine.h"
#include <string>
#include <functional>
using namespace std;

class 3DNode
{
public:
    glm::mat4 _matModel;
    glm::mat4 _matModelOrigin;
    int _shaderID=0;
    std::string _texPath;

public:
    ~DNode(){};
    DNode(std::string texPath);
    void updateModel();
    void print();
    
private:
    
};

#endif /* _DNode_h */


