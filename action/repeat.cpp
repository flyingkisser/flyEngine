//
//  repeat.cpp
//  flyEngine
//
//  Created by joe on 28/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "repeat.h"
#include "action.h"
#include "logUtil.h"

USE_NS_FLYENGINE;

repeat::repeat(int r,int c,...){
    va_list ap;
    va_start(ap,c);
    for(int i=0;i<c;i++){
        action* act=va_arg(ap, action*);
        if(act==NULL)
            break;
        m_vectorActionArr.push_back(act);
    }
    if(r<=0)
        r=1;
    m_intRepeatLimit=r;
}

repeat::~repeat(){
    for(auto a:m_vectorActionArr){
       a->stop();
       delete a;
    }
    m_vectorActionArr.clear();
}

void repeat::start(node* nodeObj){
    if(m_objNode==NULL)
        m_objNode=nodeObj;
    action* act=m_vectorActionArr[m_intRunIndex++];
    act->start(m_objNode,[&](){
        if(m_bStop){
           flylog("repeat:stopped!return!");
           return;
       }
        if(m_intRunIndex>=m_vectorActionArr.size()){
            // flylog("repeat:[round %d] end!",m_intRepeatCur++);
            if(++m_intRepeatCur>=m_intRepeatLimit){
                flylog("repeat:all end!");
                if(m_funcCB!=NULL){
                    m_funcCB();
                }
                return;
            }
            m_intRunIndex=0;
        }
        // flylog("repeat:one loop end,start next loop!");
        start(m_objNode);
    });
    m_actionCur=act;
}

void repeat::start(node* nodeObj,std::function<void(void)> cb){
    m_funcCB=cb;
    m_objNode=nodeObj;
    start(nodeObj);
}

void repeat::stop(){
    action::stop();
    if(m_actionCur!=NULL)
        m_actionCur->stop();
}
