//
//  fontMgr.cpp
//  flyEngine
//
//  Created by Joe on 2022/7/12.
//  Copyright © 2022 joe. All rights reserved.
//

#include "fontMgr.h"
using namespace flyEngine;

/*
 //使用以下结构描述
 {
 "fontPath":{size:fontObj}
 }
 */
fontTTF* fontMgr::getFontTTF(const char* fontPath,int fontSize){
    std::map<std::string,std::map<int,fontTTF*>>::iterator it=_mapTTF.find(fontPath);
    if(it==_mapTTF.end()){
        fontTTF* fontObj=new fontTTF(fontPath,fontSize);
        if(!fontObj->init())
            return NULL;
        std::map<int,fontTTF*> sizeFontMap;
        sizeFontMap[fontSize]=fontObj;
        _mapTTF[fontPath]=sizeFontMap;
        return fontObj;
    }
    std::map<int,fontTTF*> sizeFontMap=it->second;
    std::map<int,fontTTF*>::iterator it2=sizeFontMap.find(fontSize);
    if(it2==sizeFontMap.end()){
        fontTTF* fontObj=new fontTTF(fontPath,fontSize);
        if(!fontObj->init())
            return NULL;
        sizeFontMap[fontSize]=fontObj;
        _mapTTF[fontPath]=sizeFontMap;
        return fontObj;
    }
    return it2->second;
}
