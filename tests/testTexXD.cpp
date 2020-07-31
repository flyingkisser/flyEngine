//
//  testTex1D.cpp
//  flyEngine
//
//  Created by joe on 30/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "testTexXD.h"
#include "flyEngine.h"
#include "testWindow.h"

static void beginDrawTex1D(){
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400,400,-400,400);
    glMatrixMode(GL_MODELVIEW);
    
    GLubyte texArr1[4][4]={{0,255,0,255},{255,0,0,255},{0,255,0,255},{255,0,0,255}};
    GLubyte texArr2[3][4]={{255,0,0,255},{0,255,0,255},{0,0,255,255}};
//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //当纹理需要放大时，使用最近的纹理颜色
//    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //当纹理需要缩小时，使用最近的纹理颜色
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //当纹理需要放大时，使用线性混合纹理颜色
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //当纹理需要缩小时，使用线性混合纹理颜色
    glEnable(GL_TEXTURE_1D);
    
    //参数1:GL_TEXTURE_1D
    //参数2:指定为0，表示不是某一个大纹理数组的缩减
    //参数3:纹理的每一个颜色的格式
    //参数4:指出此纹理图案的颜色数量
    //参数5:指定为0，表示纹理周围没有边界，指定为1，表示纹理周围会有一个单像素宽的边界，用于与相邻图案融合
    //参数6:纹理图案中的颜色值类型，GL_RGBA
    //参数7:数据格式
    //参数8:纹理内存
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, texArr1);
    glBegin(GL_LINES);
    glTexCoord1d(0);
    glVertex3f(-200,0,0);
    glTexCoord1d(1);
    glVertex3f(200, 200,-1.3);
    glEnd();
    
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //当纹理需要放大时，使用最近的纹理颜色
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //当纹理需要缩小时，使用最近的纹理颜色
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 6, 0, GL_RGBA, GL_UNSIGNED_BYTE, texArr2);
    glBegin(GL_LINES);
    glTexCoord1d(0);
    glVertex3f(-200,100,0);
    glTexCoord1d(2);
    glVertex3f(200, 300,0);
    glEnd();
    
    glFlush();
}


void testTex1D() {
    testInitWindow3D("openGL TexImage1D TexImage2D TexImage3D test", beginDrawTex1D);
    windowLoop();
}
