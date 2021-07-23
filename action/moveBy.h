//
//  moveBy.h
//  flyEngine
//
//  Created by joe on 20/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef moveBy_h
#define moveBy_h

#include <stdio.h>
#include <functional>

#include "defines.h"
#include "action.h"


NS_FLYENGINE_BEGIN

class moveBy : public flyEngine::action {

public:    
    moveBy(float sec,glm::vec3 targPos){
        m_fSec=sec;
        m_vec3TargetPos=targPos;
    };
    
    ~moveBy(){};
    
    void start(node* nodeObj);
    void start(node* nodeObj,std::function<void(void)> cb);
    
private:
    float m_fSec;
    int m_intLoopCount=0;
    int m_intLoopIndex=0;
    glm::vec3 m_vec3TargetPos;
   
    
};

NS_FLYENGINE_END

#endif /* moveBy_h */
