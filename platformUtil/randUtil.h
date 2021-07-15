//
//  randUtil.hpp
//  lines
//
//  Created by joe on 19/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef randUtil_h
#define randUtil_h

#include <stdio.h>
#include <iostream>
#include <ctime>

class randUtil{
    public :
        static unsigned int getRand(unsigned int begin,unsigned int end);
        static float getRand(float begin,float end);
        static void getRandStr(unsigned int strLen,char* buf);
};


#endif /* randUtil_h */
