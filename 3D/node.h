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

#include "flyEngine.h"
#include <string>
#include <functional>
using namespace std;

namespace flyEngine {

class node
{
private:
    glm::mat4 _matModel;
    glm::mat4 _matModelOrigin;
    unsigned int _gl_program=0;
    unsigned int _gl_texture0=0;
    unsigned int _gl_vao=0;
    
    void _glInit();

public:
    ~node(){};
    node(const char* texPath);
    bool init(const char* texPath);
    void updateModel();
    void print();
    void draw(camera* cameraObj);
    
};

}


#endif /* _node_h */


