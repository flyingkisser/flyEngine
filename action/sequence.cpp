//
//  sequence.cpp
//  flyEngine
//
//  Created by joe on 23/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "sequence.h"
#include "action.h"
#include "logUtil.h"

USE_NS_FLYENGINE;

sequence::sequence(int c,...){
    va_list ap;
    va_start(ap,c);
    for(int i=0;i<c;i++){
        action* act=va_arg(ap, action*);
        m_vectorActionArr.push_back(act);
    }
}

sequence::~sequence(){
    for(auto a:m_vectorActionArr){
        a->stop();
        delete a;
    }
    m_vectorActionArr.clear();
}

void sequence::start(node* nodeObj){
    if(m_objNode==NULL)
        m_objNode=nodeObj;
    action* act=m_vectorActionArr[m_intRunIndex++];
    act->start(m_objNode,[&](){
        if(m_bStop){
            flylog("sequence:stopped!return!");
            return;
        }
        if(m_vectorActionArr.size()<=0 || m_intRunIndex>=m_vectorActionArr.size()){
            flylog("sequence:all end!");
            if(m_funcCB!=NULL){
                m_funcCB();
            }
            return;
        }
        flylog("sequence:one action end,start next!");
        start(m_objNode);
    });
    m_objActionCur=act;
}

void sequence::start(node* nodeObj,std::function<void(void)> cb){
    m_funcCB=cb;
    m_objNode=nodeObj;
    start(nodeObj);
}

void sequence::stop(){
    action::stop();
    if(m_objActionCur!=NULL)
        m_objActionCur->stop();
}
