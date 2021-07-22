//
//  lines.hpp
//  lines
//
//  Created by Joe on 2017/4/25.
//  Copyright © 2017年 joe. All rights reserved.
//

#ifndef lines_hpp
#define lines_hpp

#include <stdio.h>
#include "defines.h"

void drawLineByPosArrImm(flyEngine::structPos2* pointArr,int num,flyEngine::structColor* color);


void drawLineStripImm();
void drawLineLoopImm();

std::function<void(void)> drawLines();


#endif /* lines_hpp */
