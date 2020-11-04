//
//  testCurve.cpp
//  flyEngine
//
//  Created by joe on 29/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "testCurve.h"
#include "flyEngine.h"
#include "testWindow.h"

static void beginDrawLineByEvalMesh(){
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(-400,400,-400,400);
    glMatrixMode(GL_MODELVIEW);
    
    GLfloat ctrlPts[4][3]={{-40,40,0},{-10,200,0},{10,-200,0},{40,40,0}};
    glMap1f(GL_MAP1_VERTEX_3,0,1,3,4,*ctrlPts);
    glEnable(GL_MAP1_VERTEX_3);
    glColor3f(1,0,0);
    glMapGrid1f(50,0,1);
    glEvalMesh1(GL_LINE,0,50);  //等同于glBegin(GL_LINE_STRIP);glEvalCoord1f();glEnd()

    glColor3f(1,0,0);
    glPointSize(5);
    glBegin(GL_POINTS);
    for (int k=0; k<=4; k++) {
       glVertex3fv(&ctrlPts[k][0]);
    }
    glPointSize(7);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glEnd();
    glFlush();
}

static void beginDrawLineByEvalCoord(){
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(-400,400,-400,400);
    glMatrixMode(GL_MODELVIEW);
    
    GLfloat ctrlPts[4][3]={{-40,40,0},{-10,200,0},{10,-200,0},{40,40,0}};
    glMap1f(GL_MAP1_VERTEX_3,0,1,3,4,*ctrlPts);
    glEnable(GL_MAP1_VERTEX_3);
    glColor3f(1,0,0);
    glBegin(GL_LINE_STRIP);
    for(int k=0;k<=50;k++)
       glEvalCoord1f(GLfloat(k)/50.0);  //生成glVertex3f的函数
    glEnd();

    glColor3f(1,0,0);
    glPointSize(5);
    glBegin(GL_POINTS);
    for (int k=0; k<=4; k++) {
       glVertex3fv(&ctrlPts[k][0]);
    }
    glPointSize(7);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glEnd();
    glFlush();
}

//not work!
static void beginDrawMeshByEvalCoord(){
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(-400,400,-400,400);
    glMatrixMode(GL_MODELVIEW);
    
    GLfloat ctrlPts[4][4][3]={
        {{-1.5,-1.5,4.0},{-0.5,-1.5,2.0},{-0.5,-1.5,-1.0},{1.5,-1.5,2.0}},
         {{-1.5,-0.5,1.0},{-0.5,-0.5,3.0},{0.5,-0.5,0},{1.5,-0.5,-1.0}},
         {{-1.5,0.5,4.0},{-0.5,0.5,0.0},{0.5,0.5,3.0},{1.5,0.5,4.0}},
         {{-1.5,1.5,-2.0},{-0.5,1.5,-2.0},{0.5,1.5,0},{1.5,1.5,-1.0}}
    };
    glMap2f(GL_MAP1_VERTEX_3,0,1,3,4,0,1,12,4,&ctrlPts[0][0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glColor3f(1,0,0);
   
    for(int k=0;k<=8;k++){
        glBegin(GL_LINE_STRIP);
        for(int j=0;j<=40;j++)
            glEvalCoord2f(GLfloat(j)/40.0,GLfloat(k)/8.0);  //生成glVertex3f的函数
        glEnd();
        
        glBegin(GL_LINE_STRIP);
        for(int j=0;j<=40;j++)
           glEvalCoord2f(GLfloat(k)/8.0,GLfloat(j)/40.0);  //生成glVertex3f的函数
        glEnd();
    }
       
    glFlush();
}

static void beginDrawLineByNurbs(){
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(-400,400,-400,400);
    glMatrixMode(GL_MODELVIEW);
    
    GLfloat knotVector[8]={0,0,0,0,1,1,1,1};
    GLfloat ctrlPts[4][3]={{-40,40,0},{-10,200,0},{10,-200,0},{40,40,0}};
//    GLUnurbsObj* curbicBezCurve;
//    curbicBezCurve=gluNewNurbsRenderer();
    glColor3f(1, 0, 0);
//    gluBeginCurve(curbicBezCurve);
//    gluNurbsCurve(curbicBezCurve, 8, knotVector, 3, &ctrlPts[0][0], 4, GL_MAP1_VERTEX_3);
//    gluEndCurve(curbicBezCurve);
    glFlush();
}

void testCurve() {
    testInitWindow2D("openGL 2D Bezier test", beginDrawLineByNurbs);
    windowLoop();
}
