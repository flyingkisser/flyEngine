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
#include <functional>
#include "types.h"
using namespace flyEngine;

void drawTriangleImm();
void drawTriangleImm(p2* v1, p2* v2, p2* v3);
void drawTriangleImm(p2* v1, p2* v2, p2* v3,structColor* color);
void drawTriangleImm(structPos2* v1,structPos2* v2,structPos2* v3,structColor* color);

std::function<void(void)> drawTriangle(float* v1,float* v2,float* v3,structColor* color);
std::function<void(void)> drawTriangle(structPos2* v1,structPos2* v2,structPos2* v3,structColor* color);
std::function<void(void)> drawTriangle();

#endif /* triangle_hpp */
