//
//  fontMgr.cpp
//  flyEngine
//
//  Created by Joe on 2022/7/12.
//  Copyright © 2022 joe. All rights reserved.
//

#include "fontMgr.h"
using namespace flyEngine;

fontTTF* fontMgr::getFontTTF(const char* fontPath){
    std::map<std::string,fontTTF*>::iterator it=_mapTTF.find(fontPath);
    if(it==_mapTTF.end()){
        fontTTF* fontObj=new fontTTF(fontPath,12);
        if(!fontObj->init())
            return NULL;
        _mapTTF[fontPath]=fontObj;
        return fontObj;
    }
    return it->second;
}
