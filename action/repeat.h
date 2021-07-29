//
//  repeat.h
//  flyEngine
//
//  Created by joe on 28/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef repeat_h
#define repeat_h

#include <stdio.h>

#include <vector>
#include "defines.h"
#include "action.h"


NS_FLYENGINE_BEGIN

class repeat : public action{
public:
    repeat(int actionCount,int repeatCount,...);
    ~repeat();
     
    void start(node* nodeObj);
    void start(node* nodeObj,std::function<void(void)> cb);
    void stop();

private:
    std::vector<action*> m_vectorActionArr;
    node* m_objNode=NULL;
    action* m_actionCur=NULL;
    int m_intRunIndex=0;
    int m_intRepeatLimit=0;
    int m_intRepeatCur=0;
    
};

NS_FLYENGINE_END

#endif /* repeat_h */
