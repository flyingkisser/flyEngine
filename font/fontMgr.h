//
//  fontMgr.h
//  flyEngine
//
//  Created by Joe on 2022/7/12.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef fontMgr_h
#define fontMgr_h

#include <stdio.h>
#include "defines.h"
#include "fontTTF.h"
#include <map>
#include <string>

NS_FLYENGINE_BEGIN;

class fontMgr;
static fontMgr* s_ins=NULL;

class fontMgr{
public:
    static fontMgr* getInstance(){
        if(s_ins==NULL)
            s_ins=new fontMgr();
        return s_ins;
    };
    fontTTF* getFontTTF(const char* fontPath,int fontSize);
    
private:
    std::map<std::string,std::map<int,fontTTF*>> _mapTTF;
};


NS_FLYENGINE_END;


#endif /* fontMgr_h */
