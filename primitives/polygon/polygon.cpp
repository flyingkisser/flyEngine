//
//  pologon.cpp
//  lines
//
//  Created by Joe on 2017/5/5.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "polygon.h"
#include <GLUT/glut.h>
#include "error.h"
void drawPolygon(){
    GLint p1[]={1,1};
    GLint p2[]={200,1};
    GLint p3[]={200,200};
    GLint p4[]={220,240};
    GLint p5[]={100,240};
    GLint p6[]={50,230};
    glColor3f (1, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2iv(p1);
    glVertex2iv(p2);
    glVertex2iv(p3);
    glVertex2iv(p4);
    glVertex2iv(p5);
    glVertex2iv(p6);
    glEnd();
    
    glColor3ub(0,255,0);
    //只画出边线，不填充颜色
    glPolygonMode(GL_FRONT,GL_LINE);
    glBegin(GL_POLYGON);
      glVertex2iv(p1);
      glVertex2iv(p2);
      glVertex2iv(p3);
      glVertex2iv(p4);
      glVertex2iv(p5);
      glVertex2iv(p6);
    glEnd();
    
    glFlush();
    errorCheck();
}
