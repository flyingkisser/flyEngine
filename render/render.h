//
//  render.h
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef render_h
#define render_h

#include "defines.h"

namespace flyEngine {

class render{
public:
    void loopThread();
    
    static render* getInstance();
    
    static void start();
    
};

}

#endif /* render_h */
