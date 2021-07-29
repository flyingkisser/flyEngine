//
//  scaleTo.h
//  flyEngine
//
//  Created by joe on 20/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef scaleTo_h
#define scaleTo_h

#include <stdio.h>
#include <functional>

#include "defines.h"
#include "action.h"


NS_FLYENGINE_BEGIN

class scaleTo : public action {
    
public:
    scaleTo(float sec,glm::vec3 scaleDis){
        m_fSec=sec;
        m_vec3ScaleDis=scaleDis;
    };
    
    ~scaleTo(){};
    
    void start(node* nodeObj);
    void start(node* nodeObj,std::function<void(void)> cb);
 
    
private:
    float m_fSec;
    int m_intLoopCount=0;
    int m_intLoopIndex=0;
    glm::vec3 m_vec3ScaleDis;
};

NS_FLYENGINE_END

#endif /* scaleTo_h */
