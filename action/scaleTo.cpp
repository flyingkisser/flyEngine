//
//  scaleTo.cpp
//  flyEngine
//
//  Created by joe on 20/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "scaleTo.h"
#include "world.h"
#include "node.h"

#include "mathUtil.h"
#include "timerMgr.h"
#include "logUtil.h"

USE_NS_FLYENGINE

void scaleTo::start(node* nodeObj){
    //计算出几帧完成这个动画
    float secRate=world::getInstance()->getFrameRate();
    m_intLoopCount=mathUtil::ceil(m_fSec/secRate);
    glm::vec3 currenScale=nodeObj->getScale();
    glm::vec3 changeScale=m_vec3ScaleDis-currenScale;
    
    //计算出每一帧要缩放的系数
    glm::vec3 changeInner=glm::vec3(changeScale.x/m_intLoopCount,changeScale.y/m_intLoopCount,changeScale.z/m_intLoopCount);
    if(m_funcCB!=NULL){
        m_intLoopCount++;
    }
    m_intLoopIndex=0;
    flylog("scaleTo:action begin exec timer!");
    timerMgr::getInstance()->execWithCount(secRate,m_intLoopCount, [&](action* act, node* nodeObj,glm::vec3& changeInner){
        //如果有回调，且已经到了最后一帧率，则调用回调，返回，这时最后一帧是额外加的
        if(this->m_funcCB!=NULL && ++this->m_intLoopIndex>=this->m_intLoopCount){
            flylog("scaleTo:action end,call cb!");
            act->m_funcCB();
            return;
        }
        nodeObj->scaleBy(changeInner);
    }, this,nodeObj,changeInner);
}

void scaleTo::start(node* nodeObj,std::function<void(void)> cb){
    m_funcCB=cb;
    start(nodeObj);
}