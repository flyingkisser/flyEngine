//
//  skybox.h
//  flyEngine
//
//  Created by joe on 22/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef skybox_h
#define skybox_h

#include <stdio.h>
#include "defines.h"
#include "node.h"
#include "camera.h"
#include <vector>

NS_FLYENGINE_BEGIN

class skybox : public node{
private:
    unsigned int _texID;
    size _size{0};
    int _format=0;
    std::vector<unsigned char*> _vecBuf;
    
public:
    //exp:sky_1.png
    skybox(const char* imgFileName1,const char* imgFileName2,const char* imgFileName3,
           const char* imgFileName4,const char* imgFileName5,const char* imgFileName6);
    ~skybox();
    void glInit();
    bool init();
    void draw(camera* cam);
    unsigned int getTexID(){return _texID;};
};

NS_FLYENGINE_END

#endif /* skybox_h */
