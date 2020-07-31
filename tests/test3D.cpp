//
//  test3D.cpp
//  flyEngine
//
//  Created by joe on 07/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "test3D.h"
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

static void drawCubeWithRotate(float rotateX,float rotateY){
    int x0,y0,z0,x1,y1,z1;
    x0=y0=z0=1;
    x1=y1=z1=0;
    GLint vertexArr[8][3]={
        {x0,y0,z0},
        {x1,y0,z0},
        {x1,y1,z0},
        {x0,y1,z0},
        {x0,y0,z1},
        {x1,y0,z1},
        {x1,y1,z1},
        {x0,y1,z1}
    };
    
    GLubyte vertexIndexArr[]={
           4,5,6,7,    //底
           2,3,7,6,    //后
           0,4,7,3,    //右
           0,1,5,4,    //前
           1,5,6,2,    //左
           0,3,2,1     //上
           };
   GLfloat colorArr[8][4]={
             {0,0,0,1},
             {1,0,0,1},
             {0,1,0,1},
             {1,1,0,1},
             {0,0,1,1},
             {1,0,1,1},
             {0,1,1,1},
             {1,1,1,1}
           };
    
    if(!rotateX)
        rotateX=0;
    if(!rotateY)
        rotateY=0;
    glClearColor(0.3, 0.3, 0.3, 0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glShadeModel(GL_FLAT);
//    glShadeModel(GL_SMOOTH);
    
    gluLookAt(s_cameraPos.x, s_cameraPos.y, s_cameraPos.z,
                        s_lookatPos.x, s_lookatPos.y, s_lookatPos.z,
                         s_direction.x, s_direction.y, s_direction.z);
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //修改绘制模式，只显示线框图，可见的和隐藏的边都显示
//    glEnable(GL_POLYGON_OFFSET_FILL);             //启用多边形绘制偏移
//    glPolygonOffset(50,50);                       //设定多边的偏移，默认是0，0
    
//    glEnable(GL_FOG);                             //启用按对象的深度改变对象的亮度
//    glFogi(GL_FOG_MODE,GL_LINEAR);
//    glFogf(GL_FOG_START,0);                         //最小深度值
//    glFogf(GL_FOG_END,200);                         //最大深度值
    
    glTranslatef(0, 0, -10);
    s_rx=s_rx+rotateX;
    s_ry=s_ry+rotateY;
    glRotatef(s_rx, 1, 0, 0);
    glRotatef(s_ry, 0, 1, 0);
//     glRotatef(30, 0, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_INT, 0, vertexArr);
    glColorPointer(4, GL_FLOAT, 0, colorArr);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, vertexIndexArr);
    glFlush();
    printf("glRotate s_rx %f s_ry %f camera %f %f %f lookat %f %f %f\n",s_rx,s_ry,s_cameraPos.x,s_cameraPos.y,s_cameraPos.z,
           s_lookatPos.x,s_lookatPos.y,s_lookatPos.z);
}

static void drawCube(){
    drawCubeWithRotate(0, 0);
}
    
void test3dTrans() {
//    testInitWindow("openGL 2D trans test", beginBasicTrans);
//    testRegMenu("click1", [](){
//        printf("btn1 clicked!");
//    });
//    windowLoop();
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
    if(button==GLUT_LEFT_BUTTON){
        if(action==GLUT_DOWN){
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
    }else if(button==GLUT_RIGHT_BUTTON){
        if(action==GLUT_DOWN){
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
    }else if(button==GLUT_MIDDLE_BUTTON){
        if(action==GLUT_DOWN){
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
        drawCube();
        s_intMouseLeftOriginX=x;
        s_intMouseLeftOriginY=y;
    }else if(s_bMiddleDown){
       printf("onMouseClickWithMove middle:%d %d\n",x,y);
       float dx=x-s_intMouseMiddleOriginX;
       float dy=y-s_intMouseMiddleOriginY;
       float rotateY=360*(dy/width2PI);
       float rotateX=360*(dx/height2PI);
       printf("middle mouse rotate %f %f\n",rotateX,rotateY);
       drawCubeWithRotate(rotateY, rotateX);
       s_intMouseMiddleOriginX=x;
       s_intMouseMiddleOriginY=y;
    }else if(s_bRightDown){
     
    }
}

static void move_front_back(float d){
    s_cameraPos.z+=d;
    s_lookatPos.z+=d;
    drawCube();
}

static void move_left_right(float d){
    s_cameraPos.x+=d;
    s_lookatPos.x+=d;
    drawCube();
}

static void move_top_down(float d){
    s_cameraPos.y+=d;
    s_lookatPos.y+=d;
    drawCube();
}

static void test_clip(){
    GLdouble plane[4]={0,0,-5,0};
    glClipPlane(GL_CLIP_PLANE0,plane);
    glEnable(GL_CLIP_PLANE0);
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
        case 'i':
            test_clip();
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
            drawCube();
            glDisable(GL_CLIP_PLANE0);
            break;
        default:
            break;
    }
}

void test3dView()
{
    testInitWindow3D("openGL 3D view test", drawCube);
    std::function<void(int,int,int,int)> mouseClick=onMouseClick;
    testRegMouseClick(mouseClick);
    std::function<void(int,int)> mouseClickWithMove=onMouseClickWithMove;
    testRegMouseClick(mouseClick);
    testRegMouseClickWithMove(mouseClickWithMove);
    testRegKeyboard(onKeyboard);
    windowLoop();
}
