//
//  scaleBy.h
//  flyEngine
//
//  Created by joe on 26/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef scaleBy_h
#define scaleBy_h

#include <stdio.h>
#include <functional>

#include "defines.h"
#include "action.h"


NS_FLYENGINE_BEGIN

class scaleBy : public action {
    
public:
    scaleBy(float sec,glm::vec3 scaleDis){
        m_fSec=sec;
        m_vec3ScaleDis=scaleDis;
    };
    
    ~scaleBy(){};
    
    void start(node* nodeObj);
    void start(node* nodeObj,std::function<void(void)> cb);
   
private:
    float m_fSec;
    int m_intLoopCount=0;
    int m_intLoopIndex=0;
    glm::vec3 m_vec3ScaleDis;
};

NS_FLYENGINE_END

#endif /* scaleBy_h */
