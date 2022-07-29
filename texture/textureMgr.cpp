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
#include "texture.h"
#include "stb_image.h"

using namespace std;
using namespace flyEngine;

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
//        delete texObj;
//        free(std::get<5>(line));
    }
    _mapTextureCache.clear();
}

flyEngine::texture* textureMgr::getTexture(const char *szFileName,bool bFlipY){
    auto it=_mapTextureCache.find(szFileName);
    if(it!=_mapTextureCache.end())
        return it->second;
    texture* texObj=new texture(szFileName,bFlipY);
    if(!texObj->init())
        return NULL;
    texObj->glInit();
    _mapTextureCache[szFileName]=texObj;
    return texObj;
}

flyEngine::size textureMgr::getTextureSize(const char* szName){
    auto it=_mapTextureCache.find(szName);
    if(it!=_mapTextureCache.end())
        return it->second->getSize();
    return flyEngine::size{0,0};
}

unsigned int textureMgr::getTextureID(const char *szName,bool bFlipY){
    flyEngine::texture* texObj=getTexture(szName,bFlipY);
    if(texObj!=nullptr)
        return texObj->getTextureID();
    return 0;
}

struct_texture textureMgr::loadTexture(const char* szPath,bool bFlipY){
    struct_texture st={0};
    if(pngUtil::isPng(szPath)){
        if(!pngUtil::loadFile(szPath,&st,bFlipY)){
            flylog("texture.init:png loadFile %s failed",szPath);
        }
    }else if(jpgUtil::isJpg(szPath)){
        if(!jpgUtil::loadFile(szPath, &st,bFlipY)){
            flylog("texture.init:jpg loadFile %s failed",szPath);
        }
    }else{
        int width=0;
        int height=0;
        int nrComponents;
        // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
        if(bFlipY)
            stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(szPath, &width, &height, &nrComponents, 0);
        if (nrComponents == 1)
            st.format = GL_RED;
        else if (nrComponents == 3)
            st.format = GL_RGB;
        else if (nrComponents == 4)
            st.format = GL_RGBA;
        st.width=width;
        st.height=height;
        st.buf=data;
    }
    return st;
}
