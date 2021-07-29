//
//  action.h
//  flyEngine
//
//  Created by joe on 20/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef action_h
#define action_h

#include <stdio.h>
#include "defines.h"
#include "timerMgr.h"


NS_FLYENGINE_BEGIN

class node;
class action{
public:
//    virtual ~action()=0;
    virtual void start(node* nodeObj)=0;
    virtual void start(node* nodeObj,std::function<void(void)> cb)=0;
    virtual void stop(){
        m_bStop=true;
        if(m_intTimerKey>0)
            timerMgr::getInstance()->stop(m_intTimerKey);
    };
public:
     bool m_bStop=false;
     int m_intTimerKey=0;
     std::function<void(void)> m_funcCB;
};

NS_FLYENGINE_END

#endif /* action_h */
