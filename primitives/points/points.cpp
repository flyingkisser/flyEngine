//
//  points.cpp
//  lines
//
//  Created by Joe on 2017/4/28.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "points.h"
#include <GLUT/glut.h>

void drawPoints(){
    //glClearColor(0, 0, 0, 0);
    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 0, 0);
    glPointSize(5);
    glBegin (GL_POINTS);
    glVertex2f (10, 10);
    glVertex2f (100, 100);
    glVertex2i (200, 150);
     glVertex2i (220, 150);
     glVertex2i (240, 150);
    glEnd();
    glFlush();
}
