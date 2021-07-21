//
//  action.h
//  flyEngine
//
//  Created by joe on 20/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef action_h
#define action_h

#include <stdio.h>
#include "defines.h"
#include "node.h"

NS_FLYENGINE_BEGIN

class node;

class action{
public:
    virtual void start(node* nodeObj)=0;
};

NS_FLYENGINE_END

#endif /* action_h */
