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

void drawLineByPosArr(structPos2* pointArr,int num,structColor* color);

void drawLine();
void drawLineStrip();
void drawLineLoop();


#endif /* lines_hpp */
