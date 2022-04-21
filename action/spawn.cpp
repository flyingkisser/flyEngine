//
//  spawn.cpp
//  flyEngine
//
//  Created by joe on 23/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "spawn.h"

#include "action.h"
#include "logUtil.h"

USE_NS_FLYENGINE;

spawn::spawn(int c,...){
    va_list ap;
    va_start(ap,c);
    for(int i=0;i<c;i++){
        action* act=va_arg(ap, action*);
        m_vectorActionArr.push_back(act);
    }
//    while(true){
//        action* act=va_arg(ap, action*);
//        if(act==NULL)
//            return;
//        m_vectorActionArr.push_back(act);
//    }
}

spawn::~spawn(){
    for(auto a:m_vectorActionArr){
        a->stop();
        delete a;
    }
    m_vectorActionArr.clear();
}

void spawn::start(node* nodeObj){
    if(m_objNode==NULL)
        m_objNode=nodeObj;
    for(action* act:m_vectorActionArr){
        act->start(m_objNode,[this](){
//          flylog("spawn:on cb %d %d",m_intRunIndex,m_vectorActionArr.size());
            if(++m_intRunIndex>=m_vectorActionArr.size()){
                if(m_funcCB!=NULL){
                     flylog("spawn:all end!call cb!");
                     m_funcCB();
                }else{
                     flylog("spawn:all end!return!");
                }
            }
        });
    }
}

void spawn::start(node* nodeObj,std::function<void(void)> cb){
    m_funcCB=cb;
    m_objNode=nodeObj;
    start(nodeObj);
}

void spawn::stop(){
    action::stop();
    for(action* act:m_vectorActionArr){
        act->stop();
    }
}
