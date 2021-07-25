//
//  moveTo.h
//  flyEngine
//
//  Created by Joe on 2021/7/24.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef moveTo_h
#define moveTo_h

#include <stdio.h>
#include <functional>

#include "defines.h"
#include "action.h"


NS_FLYENGINE_BEGIN

class moveTo : public action {
    
public:
    moveTo(float sec,glm::vec3 targPos){
        m_fSec=sec;
        m_vec3TargetPos=targPos;
    };
    
    ~moveTo(){};
    
    void start(node* nodeObj);
    void start(node* nodeObj,std::function<void(void)> cb);
    
private:
    float m_fSec;
    int m_intLoopCount=0;
    int m_intLoopIndex=0;
    glm::vec3 m_vec3TargetPos;
    
    
};

NS_FLYENGINE_END

#endif /* moveTo_h */
