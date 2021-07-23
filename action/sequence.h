//
//  sequence.h
//  flyEngine
//
//  Created by joe on 23/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef sequence_h
#define sequence_h

#include <stdio.h>
#include <vector>
#include "defines.h"
#include "action.h"


NS_FLYENGINE_BEGIN


class sequence : public action{
public:
    sequence(int count,...);
    ~sequence();
     
    void start(node* nodeObj);
    
    void start(node* nodeObj,std::function<void(void)> cb);

private:
    std::vector<action*> m_vectorActionArr;
    
};

NS_FLYENGINE_END

#endif /* sequence_h */
