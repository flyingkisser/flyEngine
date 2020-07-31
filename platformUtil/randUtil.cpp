//
//  randUtil.cpp
//  lines
//
//  Created by joe on 19/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "randUtil.h"

int getRand(int begin,int end){
    if(begin>end)
        return 0;
    if(begin==end)
        return begin;
    srand((unsigned int)time(0));
    
    int inner=end-begin+1;
    int r=rand()%inner;
    return begin+r;
}
