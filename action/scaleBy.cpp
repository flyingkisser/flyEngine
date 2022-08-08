//
//  scaleBy.cpp
//  flyEngine
//
//  Created by joe on 26/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "scaleBy.h"

#include "world.h"
#include "node.h"

#include "mathUtil.h"
#include "timerUtil.h"
#include "logUtil.h"

USE_NS_FLYENGINE

void scaleBy::start(node* nodeObj){
    //计算出几帧完成这个动画
    float secRate=world::getInstance()->getFrameRate();
    m_intLoopCount=mathUtil::ceil(m_fSec/secRate);
    //计算出每一帧要缩放的系数
    glm::vec3 changeInner=glm::vec3(m_vec3ScaleDis.x/m_intLoopCount,m_vec3ScaleDis.y/m_intLoopCount,m_vec3ScaleDis.z/m_intLoopCount);
    if(m_funcCB!=NULL){
        m_intLoopCount++;
    }
    m_intLoopIndex=0;
    // flylog("scaleBy:action begin exec timer!");
    m_intTimerKey=timerUtil::getInstance()->execWithCount(secRate,m_intLoopCount, [&](action* act, node* nodeObj,glm::vec3& changeInner){
        //如果有回调，且已经到了最后一帧率，则调用回调，返回，这时最后一帧是额外加的
        if(this->m_funcCB!=NULL && ++this->m_intLoopIndex>=this->m_intLoopCount){
            // flylog("scaleBy:action end,call cb!");
            act->m_funcCB();
            return;
        }
        nodeObj->scaleBy(changeInner);
    }, this,nodeObj,changeInner);
}

void scaleBy::start(node* nodeObj,std::function<void(void)> cb){
    m_funcCB=cb;
    start(nodeObj);
}
