//
//  textureMgr.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "textureMgr.h"
#include "platformUtil.h"

#include <map>
#include <string>
#include <iostream>

namespace flyEngine {

textureMgr* s_instance;

textureMgr* textureMgr::getInstance(){
    if(s_instance)
        return s_instance;
    s_instance=new textureMgr();
    return s_instance;
}

textureMgr::textureMgr(){

}

textureMgr::~textureMgr(){
    clear();
}

void textureMgr::clear(){
    for(auto f : _mapTextureCache){
        free(f.second);
    }
}

unsigned char* textureMgr::_loadTexture(const char *szFileName){
    unsigned char* szBuf=(unsigned char*)readFile(szFileName);
    if(!szBuf){
        return 0;
    }
    _mapTextureCache[szFileName]=szBuf;
    return szBuf;
}

unsigned char* textureMgr::getTexture(const char *szName){
    map<string, unsigned char*>::iterator it=_mapTextureCache.find(szName);
    if(it!=_mapTextureCache.end())
        return it->second;
    return _loadTexture(szName);
}


}
