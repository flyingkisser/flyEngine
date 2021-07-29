//
//  moveTo.cpp
//  flyEngine
//
//  Created by Joe on 2021/7/24.
//  Copyright © 2021 joe. All rights reserved.
//

#include "moveTo.h"
#include "world.h"
#include "node.h"

#include "mathUtil.h"
#include "timerMgr.h"
#include "logUtil.h"

USE_NS_FLYENGINE

void moveTo::start(node* nodeObj){
    //计算出几帧完成这个动画
    float secRate=world::getInstance()->getFrameRate();
    m_intLoopCount=mathUtil::ceil(m_fSec/secRate);
    //计算出每一帧要移动的距离
    glm::vec3& curPos=nodeObj->getPosition();
    glm::vec3 moveDis=m_vec3TargetPos-curPos;
    glm::vec3 moveInner=glm::vec3(moveDis.x/m_intLoopCount,moveDis.y/m_intLoopCount,moveDis.z/m_intLoopCount);
    if(m_funcCB!=NULL){
        m_intLoopCount++;
    }
    m_intLoopIndex=0;
    flylog("moveTo:action begin exec timer!");
    m_intTimerKey=timerMgr::getInstance()->execWithCount(secRate,m_intLoopCount, [&](action* act, node* nodeObj,glm::vec3& moveInner){
        //如果有回调，且已经到了最后一帧率，则调用回调，返回，这时最后一帧是额外加的
        if(this->m_funcCB!=NULL && ++this->m_intLoopIndex>=this->m_intLoopCount){
            flylog("moveTo:action end,call cb!");
            act->m_funcCB();
            return;
        }
        nodeObj->moveBy(moveInner);
    }, this,nodeObj,moveInner);
}

void moveTo::start(node* nodeObj,std::function<void(void)> cb){
    m_funcCB=cb;
    start(nodeObj);
}
