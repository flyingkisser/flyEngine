//
//  testShader.cpp
//  flyEngine
//
//  Created by joe on 24/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "testShader.h"
#include "testWindow.h"
#include "3DTrans.h"
#include "shader.h"
#include "baseImg.h"
#include "pngUtil.h"

USE_NS_FLYENGINE

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

static bool s_bLeftDown;
static bool s_bRightDown;
static bool s_bMiddleDown;

static int s_intMouseLeftOriginX;
static int s_intMouseLeftOriginY;

static int s_intMouseRightOriginX;
static int s_intMouseRightOriginY;

static int s_intMouseMiddleOriginX;
static int s_intMouseMiddleOriginY;

static void (*drawCall)(float,float);
static shader* s_shader;

static void beginDraw3DByPng(float rotateX,float rotateY){
    if(!rotateX)
       rotateX=0;
    if(!rotateY)
       rotateY=0;
    struct_texture structTex={0};

    if(!pngUtil::loadFile("./res/fire.png",&structTex))
        return;
    
    s_shader->use();
    
    glClearColor(0.3, 0.3, 0.3, 0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glShadeModel(GL_FLAT);
    //    glShadeModel(GL_SMOOTH);
//    gluLookAt(s_cameraPos.x, s_cameraPos.y, s_cameraPos.z,
//                       s_lookatPos.x, s_lookatPos.y, s_lookatPos.z,
//                        s_direction.x, s_direction.y, s_direction.z);
    
    glTranslatef(0, 0, -10);
    s_rx=s_rx+rotateX;
    s_ry=s_ry+rotateY;
    glRotatef(s_rx, 1, 0, 0);
    glRotatef(s_ry, 0, 1, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //当纹理需要放大时，使用线性混合纹理颜色
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //当纹理需要缩小时，使用线性混合纹理颜色
    glEnable(GL_TEXTURE_2D);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, structTex.width, structTex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, structTex.buf);
    glBegin(GL_QUADS);
    
    //前面
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);    // 纹理和四边形的左下
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);    // 纹理和四边形的右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);    // 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);    // 纹理和四边形的左上
    //后面
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);    // 纹理和四边形的右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);    // 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);    // 纹理和四边形的左上
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);    // 纹理和四边形的左下
    //顶面
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);    // 纹理和四边形的左上
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);    // 纹理和四边形的左下
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);    // 纹理和四边形的右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);    // 纹理和四边形的右上
    //底面
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);    // 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);    // 纹理和四边形的左上
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);    // 纹理和四边形的左下
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);    // 纹理和四边形的右下
    //右面
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);    // 纹理和四边形的右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);    // 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);    // 纹理和四边形的左上
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);    // 纹理和四边形的左下
    //左面
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);    // 纹理和四边形的左下
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);    // 纹理和四边形的右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);    // 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);    // 纹理和四边形的左上

    glEnd();
    glFlush();
    
    free(structTex.buf);
}

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
        drawCall(0,0);
        s_intMouseLeftOriginX=x;
        s_intMouseLeftOriginY=y;
    }else if(s_bMiddleDown){
       printf("onMouseClickWithMove middle:%d %d\n",x,y);
       float dx=x-s_intMouseMiddleOriginX;
       float dy=y-s_intMouseMiddleOriginY;
       float rotateY=360*(dy/width2PI);
       float rotateX=360*(dx/height2PI);
       printf("middle mouse rotate %f %f\n",rotateX,rotateY);
       drawCall(rotateY, rotateX);
       s_intMouseMiddleOriginX=x;
       s_intMouseMiddleOriginY=y;
    }else if(s_bRightDown){
    }
}

static void move_front_back(float d){
    s_cameraPos.z+=d;
    s_lookatPos.z+=d;
    drawCall(0,0);
}

static void move_left_right(float d){
    s_cameraPos.x+=d;
    s_lookatPos.x+=d;
    drawCall(0,0);
}

static void move_top_down(float d){
    s_cameraPos.y+=d;
    s_lookatPos.y+=d;
    drawCall(0,0);
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
            drawCall(0,0);
            glDisable(GL_CLIP_PLANE0);
            break;
        default:
            break;
    }
}

static void draw3DCubePng(){
    beginDraw3DByPng(0, 0);
}

void testShaderNormal()
{
//    testInitWindow3D("openGL shader 3d quad test", draw3DCubePng);
    drawCall=beginDraw3DByPng;
    std::function<void(int,int,int,int)> mouseClick=onMouseClick;
    testRegMouseClick(mouseClick);
    std::function<void(int,int)> mouseClickWithMove=onMouseClickWithMove;
    testRegMouseClick(mouseClick);
    testRegMouseClickWithMove(mouseClickWithMove);
    testRegKeyboard(onKeyboard);
    s_shader=new shader("./res/shader/basic.vs","./res/shader/basic.fs");
    windowLoop();
}
