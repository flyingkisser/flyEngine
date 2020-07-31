//
//  triangle.hpp
//  lines
//
//  Created by Joe on 2017/7/11.
//  Copyright © 2017年 joe. All rights reserved.
//

#ifndef triangle_hpp
#define triangle_hpp

#include <stdio.h>
#include "types.h"
using namespace flyEngine;

void drawTriangle();

void drawTriangle(p2* v1, p2* v2, p2* v3);
void drawTriangle(p2* v1, p2* v2, p2* v3,structColor* color);

void drawTriangle(structPos2* v1,structPos2* v2,structPos2* v3,structColor* color);

#endif /* triangle_hpp */
