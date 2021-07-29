//
//  rotateTo.h
//  flyEngine
//
//  Created by joe on 20/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef rotateTo_h
#define rotateTo_h

#include <stdio.h>
#include <functional>

#include "defines.h"
#include "action.h"


NS_FLYENGINE_BEGIN

class rotateTo : public action {
    
public:
    rotateTo(float sec,glm::vec3 moveDis){
        m_fSec=sec;
        m_vec3MoveDis=moveDis;
    };
    
    ~rotateTo(){};
    
    void start(node* nodeObj);
    void start(node* nodeObj,std::function<void(void)> cb);
   
private:
    float m_fSec;
    int m_intLoopCount=0;
    int m_intLoopIndex=0;
    glm::vec3 m_vec3MoveDis;
};

NS_FLYENGINE_END

#endif /* rotateTo_h */
