//
//  move.cpp
//  flyEngine
//
//  Created by joe on 20/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "move.h"
#include "world.h"
#include "mathUtil.h"
#include "timerMgr.h"

using namespace flyEngine;

void move::start(flyEngine::node* nodeObj){
    //计算出几帧完成这个动画
    float secRate=world::getInstance()->getFrameRate();
    int loopCount=mathUtil::ceil(m_fSec/secRate);
    //计算出每一帧要移动的距离
    glm::vec3& curPos=nodeObj->getPosition();
    glm::vec3 moveDis=m_vec3TargetPos-curPos;
    glm::vec3 moveInner=glm::vec3(moveDis.x/loopCount,moveDis.y/loopCount,moveDis.z/loopCount);
    timerMgr::getInstance()->execWithCount(secRate,loopCount, [](flyEngine::node* nodeObj,glm::vec3& moveInner){
       nodeObj->moveBy(moveInner);
    }, nodeObj,moveInner);
}


//std::function<void(node*,float,glm::vec3&)> move::getCB(){
//    return [](node* nodeObj,float timeSec,glm::vec3& targetPos){
//        //计算出几帧完成这个动画
//        float secRate=world::getInstance()->getFrameRate();
//        int loopCount=mathUtil::ceil(timeSec/secRate);
//        //计算出每一帧要移动的距离
//        glm::vec3& curPos=nodeObj->getPosition();
//        glm::vec3 moveDis=targetPos-curPos;
//        glm::vec3 moveInner=glm::vec3(moveDis.x/loopCount,moveDis.y/loopCount,moveDis.z/loopCount);
//        timerMgr::getInstance()->execWithCount(secRate,loopCount, [](node* nodeObj,glm::vec3& moveInner){
//            nodeObj->moveBy(moveInner);
//        }, nodeObj,moveInner);
//    };
//}
