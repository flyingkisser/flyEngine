//
//  threadUtil.h
//  flyEngine
//
//  Created by joe on 03/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef threadUtil_h
#define threadUtil_h

#include <stdio.h>
#include <functional>

//void sleep(float sec);

class threadUtil{
public:
    static void sleep(float sec);
    static void createThread(std::function<void(void)> cb);
};

#endif /* threadUtil_h */
