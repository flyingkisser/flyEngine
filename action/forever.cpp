//
//  forever.cpp
//  flyEngine
//
//  Created by joe on 28/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "forever.h"
#include "action.h"
#include "logUtil.h"

USE_NS_FLYENGINE;

forever::forever(int c,...){
    va_list ap;
    va_start(ap,c);
    for(int i=0;i<c;i++){
        action* act=va_arg(ap, action*);
        m_vectorActionArr.push_back(act);
    }
}

forever::~forever(){
    for(auto a:m_vectorActionArr){
        a->stop();
        delete a;
    }
    m_vectorActionArr.clear();
}

void forever::start(node* nodeObj){
    if(m_objNode==NULL)
        m_objNode=nodeObj;
    action* act=m_vectorActionArr[m_intRunIndex++];
    act->start(m_objNode,[&](){
        if(m_bStop){
            flylog("forever:stopped!return!");
            return;
        }
        if(m_intRunIndex>=m_vectorActionArr.size()){
            flylog("forever:[round %d] end!",m_intRepeatCur);
            m_intRunIndex=0;
        }
        flylog("forever:one loop end,start next loop!");
        start(m_objNode);
    });
    m_actionCur=act;
}

void forever::start(node* nodeObj,std::function<void(void)> cb){
//    m_funcCB=cb;
    start(nodeObj);
}

void forever::stop(){
    action::stop();
    if(m_actionCur!=NULL)
        m_actionCur->stop();
}
