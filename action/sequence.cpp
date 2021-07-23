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
//    for(auto act:m_vectorActionArr){
//        delete act;
//    }
//    m_vectorActionArr.clear();
}

void sequence::start(node* nodeObj){
    if(m_vectorActionArr.size()<=0){
        flylog("sequence:all end!");
        return;
    }
       
    action* act=m_vectorActionArr.front();
    act->start(nodeObj,[&](){
        flylog("sequence:one action end,start next!");
        start(nodeObj);
    });
}

void sequence::start(node* nodeObj,std::function<void(void)> cb){
    m_funcCB=cb;
    start(nodeObj);
}
