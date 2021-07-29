//
//  forever.h
//  flyEngine
//
//  Created by joe on 28/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef forever_h
#define forever_h

#include <stdio.h>

#include <vector>
#include "defines.h"
#include "action.h"


NS_FLYENGINE_BEGIN

class forever : public action{
public:
    forever(int count,...);
    ~forever();
     
    void start(node* nodeObj);
    void start(node* nodeObj,std::function<void(void)> cb);
    void stop();

private:
    std::vector<action*> m_vectorActionArr;
    action* m_actionCur=NULL;
    node* m_objNode=NULL;
    int m_intRunIndex=0;
    int m_intRepeatCur=0;
};

NS_FLYENGINE_END

#endif /* forever_h */
