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

using namespace std;

namespace flyEngine{

class textureMgr{
    
private:
    //width,height,format,internalFormat,id,buf
    map<string,textureTuple> _mapTextureCache;
    
    bool _loadTexture(const char* szName);
    
public:
    static textureMgr* getInstance();
    textureMgr();
     ~textureMgr();
    void clear();
    
    textureTuple getTextureTuple(const char* szName);
    unsigned int getTextureID(const char *szName,unsigned int texRGBType);
    size getTextureSize(const char* szName);
};

}


#endif /* textureMgr_h */


