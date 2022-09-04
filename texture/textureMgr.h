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


NS_FLYENGINE_BEGIN
class texture2;

class textureMgr{
    
private:
    map<string,texture2*> _mapTextureCache;
    
public:
    static textureMgr* getInstance();
    textureMgr();
     ~textureMgr();
    void clear();
    
    texture2* getTexture(const char* szName,bool bFlipY=false);
    size getTextureSize(const char* szName);
    

    unsigned int getTextureID(const char *szName,bool bFlipY=false);
    struct_texture loadTexture(const char* szFileName,bool bFlipY=false);
    
};

NS_FLYENGINE_END


#endif /* textureMgr_h */


