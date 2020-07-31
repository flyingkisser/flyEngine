//
//  testPrimitive.cpp
//  flyEngine
//
//  Created by joe on 30/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "testPrimitive.h"
#include "defines.h"

#include "testWindow.h"

#include "points.h"
#include "line.h"
#include "rectangular.h"
#include "polygon.h"
#include "triangle.h"
#include "quad.h"
#include "vertexArray.h"
#include "pixel.h"
#include "listStr.h"
#include "dirUtil.h"
#include "character.h"

static void menuCallback(GLint menuID){
    switch (menuID) {
        case 1:
            snapsBMP("./save.bmp");
            break;
        default:
            break;
    }
}

static void beginDraw(void) {
//    initOtho2D();
    drawPoints();
//    drawLine();

//    drawLineLoop();
//    drawPolygon();
//    drawTriangle();
   
//    drawRectangular();
//    drawQuad();
//    drawCube();

//     drawElementsByVertexArr();
//     drawBitmapByListString();

//    pixelDrawPixels();
//    pixelReadPixels();
//    pixelCopyPixels();
    
    //pixcel的基本操作
//    drawBMP("/Users/joe/Documents/work_OpenGL/lines/res/test.bmp");
//    pixelDrawBitmap();
//    pixelCopyPixels();
    
    //字体的操作
//    drawStringByBitmap("hello,world! by joe:)");
//    drawStringByStroke("hello,world! by joe:)");
   
}

void testPrimitive() {
    testInitWindow2D("openGL primitive test", beginDraw);
    testRegMenu("snapshot", [](){
         snapsBMP("./save.bmp");
    });
    windowLoop();
}
