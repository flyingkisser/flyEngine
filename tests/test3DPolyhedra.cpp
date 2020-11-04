//
//  test3DPolyhedra.cpp
//  flyEngine
//
//  Created by joe on 27/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "test3DPolyhedra.h"

#include "test3DPolyhedra.h"
#include "defines.h"
#include "line.h"
#include "triangle.h"
#include "2DTrans.h"
#include "3DTrans.h"
#include <math.h>

#include "testWindow.h"

using namespace flyEngine;
static float s_rx=0;
static float s_ry=0;
static float s_camera_x=0;
static float s_camera_y=0;
static float s_camera_z=0;
static float s_lookat_x=0;
static float s_lookat_y=0;
static float s_lookat_z=-1000;

static structPos3 s_cameraPos={0,0,0};
static structPos3 s_lookatPos={0,0,-1000};
static structPos3 s_direction={0,1,0};

static void drawPolyhedraWithRotate(float rotateX,float rotateY){
    if(!rotateX)
        rotateX=0;
    if(!rotateY)
        rotateY=0;
    float z=-30;
    glClearColor(0.3, 0.3, 0.3, 0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glShadeModel(GL_FLAT);
//    glShadeModel(GL_SMOOTH);
    
//    gluLookAt(s_cameraPos.x, s_cameraPos.y, s_cameraPos.z,
//                        s_lookatPos.x, s_lookatPos.y, s_lookatPos.z,
//                         s_direction.x, s_direction.y, s_direction.z);
    s_rx=s_rx+rotateX;
    s_ry=s_ry+rotateY;
    
    glPushMatrix();
    glTranslatef(0, 0, z);
    glRotatef(s_rx, 1, 0, 0);
    glRotatef(s_ry, 0, 1, 0);
//    glutWireCube(1);    //立方体
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-3, 0, z);
    glRotatef(s_rx, 1, 0, 0);
    glRotatef(s_ry, 0, 1, 0);
//    glutWireDodecahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3, 0, z);
    glRotatef(s_rx, 1, 0, 0);
       glRotatef(s_ry, 0, 1, 0);
//    glutWireTetrahedron();  //三棱锥
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-3, 3, z);
    glRotatef(s_rx, 1, 0, 0);
       glRotatef(s_ry, 0, 1, 0);
//    glutWireOctahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 3, z);
    glRotatef(s_rx, 1, 0, 0);
       glRotatef(s_ry, 0, 1, 0);
//    glutWireIcosahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3, 3,z);
    glRotatef(s_rx, 1, 0, 0);
       glRotatef(s_ry, 0, 1, 0);
//    glutWireTeapot(0.5);      //茶杯
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-8, 6,z);
    glRotatef(s_rx, 1, 0, 0);
       glRotatef(s_ry, 0, 1, 0);
//    glutWireSphere(1, 10, 10);      //球面
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-4, 6, z);
    glRotatef(s_rx, 1, 0, 0);
    glRotatef(s_ry, 0, 1, 0);
//    glutSolidSphere(1, 10, 10);      //球面
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 6, z);
    glRotatef(s_rx, 1, 0, 0);
    glRotatef(s_ry, 0, 1, 0);
//    glutWireCone(1,3, 10, 10);      //圆锥面
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(s_rx, 1, 0, 0);
    glRotatef(s_ry, 0, 1, 0);
    glTranslatef(4, 6, z);
//    glutSolidCone(1, 3,10, 10);      //圆锥面
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-6, -6,z);
    glRotatef(s_rx, 1, 0, 0);
    glRotatef(s_ry, 0, 1, 0);
//    GLUquadricObj* obj1;
//    obj1=gluNewQuadric();
//    gluQuadricDrawStyle(obj1, GLU_LINE);
//    gluSphere(obj1,1,10,10);            //二次曲面的球面线框图
    glPopMatrix();
 
    glPushMatrix();
    glTranslatef(-3, -6,z);
    glRotatef(s_rx, 1, 0, 0);
    glRotatef(s_ry, 0, 1, 0);
//    GLUquadricObj* obj2;
//    obj2=gluNewQuadric();
//    gluQuadricDrawStyle(obj2, GLU_LINE);
//    gluCylinder(obj2,1,1,3,10,10);            //二次曲面的圆柱体线框图
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, -6,z);
    glRotatef(s_rx, 1, 0, 0);
    glRotatef(s_ry, 0, 1, 0);
//    GLUquadricObj* obj3;
//    obj3=gluNewQuadric();
//    gluQuadricDrawStyle(obj3, GLU_LINE);
//    gluDisk(obj3,0.3,1,10,10);            //二次曲面的圆柱体线框图
    glPopMatrix();
    
    
    glFlush();
}

static void drawPolyhedra(){
    drawPolyhedraWithRotate(0, 0);
}

static bool s_bLeftDown;
static bool s_bRightDown;
static bool s_bMiddleDown;

static int s_intMouseLeftOriginX;
static int s_intMouseLeftOriginY;

static int s_intMouseRightOriginX;
static int s_intMouseRightOriginY;

static int s_intMouseMiddleOriginX;
static int s_intMouseMiddleOriginY;

static void onMouseClick(int button,int action,int x,int y){
    if(button==GLFW_MOUSE_BUTTON_LEFT){
        if(action==GLFW_PRESS){
            s_bLeftDown=true;
            s_intMouseLeftOriginX=x;
            s_intMouseLeftOriginY=y;
            printf("onMouseLeftClick:%d %d\n",x,y);
        }else{
            s_bLeftDown=false;
            s_intMouseLeftOriginX=0;
            s_intMouseLeftOriginY=0;
            printf("onMouseLeftClick:reset\n");
        }
    }else if(button==GLFW_MOUSE_BUTTON_RIGHT){
        if(action==GLFW_PRESS){
            s_bRightDown=true;
            s_intMouseRightOriginX=x;
            s_intMouseRightOriginY=y;
            printf("onMouseRightClick:%d %d\n",x,y);
        }else{
            s_bRightDown=false;
            s_intMouseRightOriginX=0;
            s_intMouseRightOriginY=0;
            printf("onMouseRightClick:reset\n");
        }
    }else if(button==GLFW_MOUSE_BUTTON_MIDDLE){
        if(action==GLFW_PRESS){
            s_bMiddleDown=true;
            s_intMouseMiddleOriginX=x;
            s_intMouseMiddleOriginY=y;
            printf("onMouseMiddleClick:%d %d\n",x,y);
        }else{
            s_bMiddleDown=false;
            s_intMouseMiddleOriginX=0;
            s_intMouseMiddleOriginY=0;
            printf("onMouseMiddleClick:reset\n");
        }
    }
}

static void rotate_camera(float dx,float dy){
    structPos3 axisX={1,0,0};
    structPos3 axisY={0,1,0};
    if(dx){
         _3dTrans::rotate(&s_cameraPos,&axisX,dx);
         _3dTrans::rotate(&s_lookatPos,&axisX,dx);
    }
    if(dy){
        _3dTrans::rotate(&s_cameraPos,&axisY,dy);
        _3dTrans::rotate(&s_lookatPos,&axisY,dy);
    }
};

static void onMouseClickWithMove(int x,int y){
    float width2PI=4000;   //认为横向移动100个坐标点，为360度
    float height2PI=4000;
    if(s_bLeftDown){
        printf("onMouseClickWithMove left:%d %d\n",x,y);
        float dx=x-s_intMouseLeftOriginX;
        float dy=y-s_intMouseLeftOriginY;
        float rotateY=360*(dy/width2PI);
        float rotateX=360*(dx/height2PI);
        printf("left mouse rotate %f %f\n",rotateX,rotateY);
        rotate_camera(rotateY, rotateX);
        drawPolyhedra();
        s_intMouseLeftOriginX=x;
        s_intMouseLeftOriginY=y;
    }else if(s_bMiddleDown){
       printf("onMouseClickWithMove middle:%d %d\n",x,y);
       float dx=x-s_intMouseMiddleOriginX;
       float dy=y-s_intMouseMiddleOriginY;
       float rotateY=360*(dy/width2PI);
       float rotateX=360*(dx/height2PI);
       printf("middle mouse rotate %f %f\n",rotateX,rotateY);
       drawPolyhedraWithRotate(rotateY, rotateX);
       s_intMouseMiddleOriginX=x;
       s_intMouseMiddleOriginY=y;
    }else if(s_bRightDown){
     
    }
}

static void move_front_back(float d){
    s_cameraPos.z+=d;
    s_lookatPos.z+=d;
    drawPolyhedra();
}

static void move_left_right(float d){
    s_cameraPos.x+=d;
    s_lookatPos.x+=d;
    drawPolyhedra();
}

static void move_top_down(float d){
    s_cameraPos.y+=d;
    s_lookatPos.y+=d;
    drawPolyhedra();
}


static void onKeyboard(unsigned char c,int x,int y){
    float move_d=0.2;
    switch (c) {
        case 'w':
            move_front_back(-move_d);
            break;
        case 's':
            move_front_back(move_d);
            break;
        case 'a':
             move_left_right(-move_d);
             break;
        case 'd':
             move_left_right(move_d);
             break;
        case 'z':   //向上移动
             move_top_down(-move_d);
             break;
        case 'x':   //向下移动
             move_top_down(move_d);
             break;
        case 'r':   //重围位置
            s_cameraPos.x=0;
             s_cameraPos.y=0;
             s_cameraPos.z=0;
            s_lookatPos.x=0;
             s_lookatPos.y=0;
             s_lookatPos.z=-1000;
            s_rx=0;
            s_ry=0;
            drawPolyhedra();
            break;
        default:
            break;
    }
}

void test3DPolyhedra()
{
    testInitWindow3D("openGL 3D polyhedra test", drawPolyhedra);
    std::function<void(int,int,int,int)> mouseClick=onMouseClick;
    testRegMouseClick(mouseClick);
    std::function<void(int,int)> mouseClickWithMove=onMouseClickWithMove;
    testRegMouseClick(mouseClick);
    testRegMouseClickWithMove(mouseClickWithMove);
    testRegKeyboard(onKeyboard);
    windowLoop();
}
