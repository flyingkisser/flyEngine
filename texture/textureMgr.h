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

using namespace std;

namespace flyEngine{

class textureMgr{

private:
    map<string,unsigned char*> _mapTextureCache;
    
    unsigned char* _loadTexture(const char* szName);
    
public:
     static textureMgr* getInstance();
    textureMgr();
     ~textureMgr();
    unsigned char* getTexture(const char* szName);
    void clear();
    
};

}


#endif /* textureMgr_h */


