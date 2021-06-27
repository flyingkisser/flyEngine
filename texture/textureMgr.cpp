//
//  textureMgr.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//
#include <map>
#include <string>
#include <iostream>

#include "textureMgr.h"
#include "platformUtil.h"
#include "baseImg.h"
#include "bmpUtil.h"
#include "pngUtil.h"
#include "jpgUtil.h"

using namespace std;

static textureMgr* s_instance;

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
        auto texObj=f.second;
        delete texObj;
//        free(std::get<5>(line));
    }
    _mapTextureCache.clear();
}

texture* textureMgr::getTexture(const char *szFileName){
    auto it=_mapTextureCache.find(szFileName);
    if(it!=_mapTextureCache.end())
        return it->second;
    texture* texObj=new texture(szFileName);
    if(!texObj->init())
        return NULL;
    _mapTextureCache[szFileName]=texObj;
    return texObj;
}

flyEngine::size textureMgr::getTextureSize(const char* szName){
    auto it=_mapTextureCache.find(szName);
    if(it!=_mapTextureCache.end())
        return it->second->getSize();
    return flyEngine::size{0,0};
}

//textureTuple textureMgr::getTextureTuple(const char *szName){
//    auto it=_mapTextureCache.find(szName);
//    if(it!=_mapTextureCache.end())
//        return it->second;
//    if(_loadTexture(szName))
//        return _mapTextureCache.find(szName)->second;
//    return textureTuple{0,0,0,0,0,0};
//}
//bool textureMgr::_loadTexture(const char *szFileName){
//    struct_texture st={0};
//    if(pngUtil::isPng(szFileName)){
//        if(!pngUtil::loadFile(szFileName,&st))
//            return false;
//    }else if(jpgUtil::isJpg(szFileName)){
//        if(!jpgUtil::loadFile(szFileName, &st))
//            return false;
//    }
//    _mapTextureCache[szFileName]=textureTuple{st.width,st.height,st.format,st.internalFormat,st.id,st.buf};
//    return true;
//}
