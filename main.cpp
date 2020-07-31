//
//  main.cpp
//
//  Created by Joe on 2017/4/25.
//  Copyright © 2017年 joe. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>
#include "testMain.h"

//void initPerspective3D(void)
//{
//    glClearColor(1.0,1.0,1.0,0.0);
//    gluPerspective(30.0, 0.9, 1, 10000);
//    glMatrixMode(GL_MODELVIEW);
//    gluLookAt(500,300,800,0,0,0,0,1,0);
//}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    testMainBegin();
    return 0;
}

