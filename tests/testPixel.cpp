//
//  testPixel.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//
#include "flyEngine.h"

#include "testPixel.h"
#include "testWindow.h"

static void beginDraw(){
//    unsigned char* szBuf=g_textureMgr->getTexture("./res/test.bmp");
    ui::image* imgObj=new ui::image("./res/test.bmp");
    imgObj->draw();
}

static void xorPixel(float x,float y,float width,float height){
      //指定源缓存
        glReadBuffer(GL_FRONT);
        //指定目标缓存
        glDrawBuffer(GL_FRONT);
        //颜色进行操作
        glEnable(GL_COLOR_LOGIC_OP);
        //GL_AND
        //GL_OR
        //GL_XOR
        //GL_COPY   复制(默认值)
        //GL_COPY_INVERTED 取反后复制
        //GL_CLEAR  目标位清0
        glLogicOp(GL_XOR);
        //x,y,width,height,GL_DEPTH(GL_DEPTH_COMPONENT)或者GL_STENCIL(GL_STENCIL_INDEX)或者GL_COLOR(GL_RGBA或GL_COLOR_INDEX)
        glRasterPos2i(x,y);
        glCopyPixels(0,0,width,height,GL_COLOR);
    //    glutSwapBuffers();
        glFlush();
}

static void cppyPixel(float x,float y,float width,float height){
        glRasterPos2i(x,y);
        glCopyPixels(0,0,width,height,GL_COLOR);
        glFlush();
}

void testBMP(){
    testInitWindow2D("openGL pixel test",beginDraw);
    testRegMenu("show second picture",[](){
//        node* imgObj=(node*)new ui::image("./res/test.bmp");
//        imgObj->setPosition(30, 30);
//        imgObj->draw();
    });
    testRegMenu("copy pixel",[](){
//           node* imgObj=(node*)new ui::image("./res/test.bmp");
//           size imgSize=imgObj->getSize();
//           cppyPixel(30,30,imgSize.width,imgSize.height);
          });
    testRegMenu("remove second picture",[](){
//        node* imgObj=(node*)new ui::image("./res/test.bmp");
//        size imgSize=imgObj->getSize();
//        xorPixel(30,30,imgSize.width,imgSize.height);
       });
    windowLoop();
}

