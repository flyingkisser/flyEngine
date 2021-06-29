//
//  textureMgr.h
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef textureMgr_h
#define textureMgr_h

#include <stdio.h>
#include <map>
#include <tuple>
#include "defines.h"
#include "baseImg.h"
#include "texture.h"

using namespace std;
class texture;

namespace flyEngine{

class textureMgr{
    
private:
    map<string,texture> _mapTextureCache;
    
public:
    static textureMgr* getInstance();
    textureMgr();
     ~textureMgr();
    void clear();
    
    texture* getTexture(const char* szName);
    size getTextureSize(const char* szName);
    

    unsigned int getTextureID(const char *szName);
    
};

}


#endif /* textureMgr_h */


