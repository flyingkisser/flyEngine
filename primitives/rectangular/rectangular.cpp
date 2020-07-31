//
//  rectangular.cpp
//  lines
//
//  Created by Joe on 2017/5/5.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "rectangular.h"
#include <GLUT/glut.h>

void drawRectangular(){
    int v1[]={300,300};
    int v2[]={400,400};
    glColor3f(1.0,0.3,0.2);
    glRecti(1,1,200,200);
    glRectiv(v1,v2);
    glFlush();
}
