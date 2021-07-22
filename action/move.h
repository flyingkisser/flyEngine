//
//  move.h
//  flyEngine
//
//  Created by joe on 20/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef move_h
#define move_h

#include <stdio.h>
#include "defines.h"
#include "node.h"
#include "action.h"


NS_FLYENGINE_BEGIN

class move  : public flyEngine::action {

public:    
    move(float sec,glm::vec3 targPos){
        m_fSec=sec;
        m_vec3TargetPos=targPos;
    };
    
    void start(flyEngine::node* nodeObj);
    
private:
    float m_fSec;
    glm::vec3 m_vec3TargetPos;
    
};

NS_FLYENGINE_END

#endif /* move_h */
