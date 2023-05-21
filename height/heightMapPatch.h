//
//  heightMapPatch.h
//  flyEngine
//
//  Created by joe on 12/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef heightMapPatch_h
#define heightMapPatch_h

#include <stdio.h>
#include <string>
#include <functional>

#include "defines.h"
#include "node.h"

using namespace std;

NS_FLYENGINE_BEGIN
class texture2;
class quadSize;

class heightMapPatch: public node
{
private:
    texture2* _texObj=NULL;
    const char* _texPath=NULL;
    std::vector<float> _vertices;
    std::vector<float> _vertices2;
    unsigned int _gl_vao2;
    unsigned int _gl_vbo2;
    unsigned int num_strips;
    unsigned int num_verts_per_strip;
    unsigned int _rez;
    bool _bLineMode=false;

    bool _bDrawQuadSize=false;
    quadSize* _quadSize=NULL;
    shader* _shQuadSize=NULL;
    
public:
    unsigned int _gl_texture0=0;
    
    
    heightMapPatch(const char* texPath);
    ~heightMapPatch(){};

    bool init();
    void resetPos();
    void draw();
    void setPipelineValue();
    void drawCall();
    void setLineMode(bool v){_bLineMode=v;};
    void setDrawQuad(bool v){_bDrawQuadSize=v;};
    texture2* getTexObj(){return _texObj;};
};

NS_FLYENGINE_END
#endif /* heightMapPatch_h */
