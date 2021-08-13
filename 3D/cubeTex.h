//
//  cubeTex.h
//  flyEngine
//
//  Created by wu mingzhou on 2021/8/1.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef cubeTex_h
#define cubeTex_h

#include <stdio.h>
#include <string>
#include <functional>

#include "defines.h"
#include "node.h"

using namespace std;

NS_FLYENGINE_BEGIN

class cubeTex: public node
{
private:
    unsigned int _gl_texture0=0;
    texture* _texObj;
    const char* _texPath;

public:
    cubeTex(const char* texPath);
    ~cubeTex(){};
    
    bool init();
    void glInit();

    void draw(camera* cameraObj);
};

NS_FLYENGINE_END

#endif /* cubeTex_h */
