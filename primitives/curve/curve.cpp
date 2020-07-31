//
//  curve.cpp
//  flyEngine
//
//  Created by joe on 29/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "curve.h"
#include <GLUT/glut.h>

void drawBezierLine(){
    GLfloat ctrlPts[4][3]={{-40,40,0},{-10,200,0},{10,-200,0},{40,40,0}};
    glMap1f(GL_MAP1_VERTEX_3,0,1,3,4,*ctrlPts);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_LINE_STRIP);
    for(int k=0;k<=50;k++)
        glEvalCoord1f(GLfloat(k)/50.0);
    glEnd();
    
    glColor3f(1,0,0);
    glPointSize(5);
    glBegin(GL_POINTS);
    for (int k=0; k<=4; k++) {
        glVertex3fv(&ctrlPts[k][0]);
    }
    
    glFlush();
}
