//
//  heightMap.h
//  flyEngine
//
//  Created by joe on 10/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef heightMap_h
#define heightMap_h

#include <stdio.h>
#include <string>
#include <functional>

#include "defines.h"
#include "node.h"

using namespace std;

NS_FLYENGINE_BEGIN
class texture2;

class heightMap: public node
{
private:
    texture2* _texObj=NULL;
    const char* _texPath=NULL;
    std::vector<float> _vertices;
    std::vector<int> _indices;
    unsigned int num_strips;
    unsigned int num_verts_per_strip;
    bool _bLineMode=false;

public:
    unsigned int _gl_texture0=0;
    
    heightMap(const char* texPath);
    ~heightMap(){};

    bool init();
    void resetPos();
    void draw();
    void setPipelineValue();
    void drawCall();
    void setLineMode(bool v){_bLineMode=v;};
};

NS_FLYENGINE_END

#endif /* heightMap_h */
