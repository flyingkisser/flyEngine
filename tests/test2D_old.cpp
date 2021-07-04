////
////  test2D.cpp
////  flyEngine
////
////  Created by joe on 30/06/2020.
////  Copyright © 2020 joe. All rights reserved.
////
//
//#include <math.h>
//
//#include "test2D.h"
//#include "defines.h"
//#include "line.h"
//#include "triangle.h"
//#include "2dTrans.h"
//
//#include "testWindow.h"
//#include "textureMgr.h"
//#include "VAOMgr.h"
//#include "shader.h"
//#include "keyboardEventMgr.h"
//
//using namespace flyEngine;
//
//static void beginBasicTrans(void) {
//    glClearColor(1.0,1.0,1.0,0);    //指定背影色为白色
//    glClear(GL_COLOR_BUFFER_BIT);   //指定颜色缓存，该缓存使用glClearColor指定的背影色
//    p2* v1=new p2(10,10);
//    p2* v2=new p2(30,10);
//    p2* v3=new p2(20,30);
//    structColor color1={0,255,0,100};
//    float triangleWidth=20;
//    float triangleHeight=pow(400-100,0.5);
//    float centerX=10+triangleWidth/2;
//    float centerY=10+triangleHeight/2;
//    drawTriangleImm(v1,v2,v3,&color1);
//    float moveX=100;
//    float moveY=200;
//
//
//    //移动
//    structColor color2={210,210,210,255};
//    glLoadIdentity();
//    glTranslatef(moveX, moveY, 0);
//    drawTriangleImm(v1,v2,v3,&color2);
//
//    //缩放
//    //先移动到-centerX,-centerY
//    //再缩放3,3
//    //再移动回centerX,centerY
//    //注意变换的矩阵是右乘的，所以实际是按照调用顺序从后往前左乘
//    structColor color3={255,0,0,80};
//    glLoadIdentity();
//    glTranslatef(centerX, centerY, 0);
//    glScalef(3, 3, 0);
//    glTranslatef(-centerX, -centerY, 0);
//    drawTriangleImm(v1,v2,v3,&color3);
//
//    //旋转
//    structColor color4={0,10,250,150};
//    glLoadIdentity();
//    glTranslatef(moveX, moveY, 0);
//    glTranslatef(centerX, centerY, 0);
//    glRotatef(180, 0, 0,1);    //M_PI,这里使用的度数，不是pi值，180表示旋转180度
//    glTranslatef(-centerX, -centerY, 0);
//    drawTriangleImm(v1,v2,v3,&color4);
//}
//
//static std::function<void(void)> beginTransView(){
//    float moveX=200;
//    float moveY=200;
//
//    glClearColor(1, 1, 1, 0);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    //已经在core模式下被废弃！
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glTranslatef(moveX, moveY, 0);
//
//    structPos2 p1={-50,-25};
//    structPos2 p2={50,-25};
//    structPos2 p3={0,25};
//    structColor color={255,0,0,255};
//
//    return drawTriangle(&p1, &p2, &p3, &color);
//}
//
//static std::function<void(void)> beginBasicViewJpg(){
//    float verticeArr[]={
//        0.5,0.5,0,  1,0,0,  1,1,
//        0.5,-0.5,0, 0,1,0,  1,0,
//        -0.5,-0.5,0,0,0,1,  0,0,
//        -0.5,0.5,0, 1,1,0,  0,1
//    };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID=textureMgr::getInstance()->getTextureID("res/wall.jpg",GL_RGB);
//    if(!texID)
//        return nullptr;
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,3,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
//    return [vao,texID](){
//        VAOMgr::drawPrimitive(vao, texID, GL_TRIANGLES, 6);
//    };
//}
//
//static std::function<void(void)> beginBasicViewPng(){
//    float verticeArr[]={
//        0.5,0.5,0,  1,0,0,  1,1,
//        0.5,-0.5,0, 0,1,0,  1,0,
//        -0.5,-0.5,0,0,0,1,  0,0,
//        -0.5,0.5,0, 1,1,0,  0,1
//    };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
//    if(!texID)
//        return nullptr;
//    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
//    size winSize=getWindowSize();
//    float winWith=winSize.width;
//    float winHeight=winSize.height;
//    float width=texSize.width;
//    float height=texSize.height;
//    verticeArr[0]=width/winWith;
//    verticeArr[1]=height/winHeight;
//    verticeArr[0+8*1]=width/winWith;
//    verticeArr[1+8*1]=-height/winHeight;
//    verticeArr[0+8*2]=-width/winWith;
//    verticeArr[1+8*2]=-height/winHeight;
//    verticeArr[0+8*3]=-width/winWith;
//    verticeArr[1+8*3]=height/winHeight;
//
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
//    return [vao,texID](){
//        VAOMgr::drawPrimitive(vao, texID, GL_TRIANGLES, 6);
//    };
//}
//
//static std::function<void(void)> beginBasicViewMixPng(){
//    float verticeArr[]={
//        0.5,0.5,0,  1,0,0,  1,1,
//        0.5,-0.5,0, 0,1,0,  1,0,
//        -0.5,-0.5,0,0,0,1,  0,0,
//        -0.5,0.5,0, 1,1,0,  0,1
//    };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
//    if(!texID1)
//        return nullptr;
//    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
//    if(!texID2)
//       return nullptr;
//    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
//    size winSize=getWindowSize();
//    float winWith=winSize.width;
//    float winHeight=winSize.height;
//    float width=texSize.width;
//    float height=texSize.height;
//    verticeArr[0]=width/winWith;
//    verticeArr[1]=height/winHeight;
//    verticeArr[0+8*1]=width/winWith;
//    verticeArr[1+8*1]=-height/winHeight;
//    verticeArr[0+8*2]=-width/winWith;
//    verticeArr[1+8*2]=-height/winHeight;
//    verticeArr[0+8*3]=-width/winWith;
//    verticeArr[1+8*3]=height/winHeight;
//
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
//    int shaderID=shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureAphaMix.fs");
//
//    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
//    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
//
//    return [vao,texID1,texID2](){
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,texID1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D,texID2);
//        glBindVertexArray(vao);
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//    };
//}
//
//static std::function<void(void)> beginBasicViewMixPng2(){
//    float verticeArr[]={
//        0.5,0.5,0,  1,0,0,  2,2,
//        0.5,-0.5,0, 0,1,0,  2,0,
//        -0.5,-0.5,0,0,0,1,  0,0,
//        -0.5,0.5,0, 1,1,0,  0,2
//    };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID1=textureMgr::getInstance()->getTextureID("res/wall.jpg",GL_RGB);
//    if(!texID1)
//        return nullptr;
//    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
//    if(!texID2)
//       return nullptr;
//
//    glBindTexture(GL_TEXTURE_2D,texID1);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
////    glBindTexture(GL_TEXTURE_2D,texID2);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
//
//    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
//    size winSize=getWindowSize();
//    float winWith=winSize.width;
//    float winHeight=winSize.height;
//    float width=texSize.width;
//    float height=texSize.height;
//    verticeArr[0]=width/winWith;
//    verticeArr[1]=height/winHeight;
//    verticeArr[0+8*1]=width/winWith;
//    verticeArr[1+8*1]=-height/winHeight;
//    verticeArr[0+8*2]=-width/winWith;
//    verticeArr[1+8*2]=-height/winHeight;
//    verticeArr[0+8*3]=-width/winWith;
//    verticeArr[1+8*3]=height/winHeight;
//
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
//    int shaderID=shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureAphaMix.fs");
//
//    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
//    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
//
//    return [vao,texID1,texID2](){
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,texID1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D,texID2);
//        glBindVertexArray(vao);
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//    };
//}
//
//static std::function<void(void)> beginBasicViewMixPng3(){
//    float verticeArr[]={
//        0.5,0.5,0,  1,0,0,  0.55,0.55,
//        0.5,-0.5,0, 0,1,0,  0.55,0.45,
//        -0.5,-0.5,0,0,0,1,  0.45,0.45,
//        -0.5,0.5,0, 1,1,0,  0.45,0.55
//    };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID1=textureMgr::getInstance()->getTextureID("res/wall.jpg",GL_RGB);
//    if(!texID1)
//        return nullptr;
//    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
//    if(!texID2)
//       return nullptr;
//
//    glBindTexture(GL_TEXTURE_2D,texID1);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    glBindTexture(GL_TEXTURE_2D,texID2);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
//    size winSize=getWindowSize();
//    float winWith=winSize.width;
//    float winHeight=winSize.height;
//    float width=texSize.width;
//    float height=texSize.height;
//    verticeArr[0]=width/winWith;
//    verticeArr[1]=height/winHeight;
//    verticeArr[0+8*1]=width/winWith;
//    verticeArr[1+8*1]=-height/winHeight;
//    verticeArr[0+8*2]=-width/winWith;
//    verticeArr[1+8*2]=-height/winHeight;
//    verticeArr[0+8*3]=-width/winWith;
//    verticeArr[1+8*3]=height/winHeight;
//
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
//    int shaderID=shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureAphaMixReverseY.fs");
//
//    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
//    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
//
//    return [vao,texID1,texID2](){
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,texID1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D,texID2);
//        glBindVertexArray(vao);
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//    };
//}
//
//static std::function<void(void)> beginBasicViewMixPng4(){
//    float verticeArr[]={
//        0.5,0.5,0,  1,0,0,  0.55,0.55,
//        0.5,-0.5,0, 0,1,0,  0.55,0.45,
//        -0.5,-0.5,0,0,0,1,  0.45,0.45,
//        -0.5,0.5,0, 1,1,0,  0.45,0.55
//    };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID1=textureMgr::getInstance()->getTextureID("res/wall.jpg",GL_RGB);
//    if(!texID1)
//        return nullptr;
//    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
//    if(!texID2)
//       return nullptr;
//
//    glBindTexture(GL_TEXTURE_2D,texID1);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    glBindTexture(GL_TEXTURE_2D,texID2);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
//    size winSize=getWindowSize();
//    float winWith=winSize.width;
//    float winHeight=winSize.height;
//    float width=texSize.width;
//    float height=texSize.height;
//    verticeArr[0]=width/winWith;
//    verticeArr[1]=height/winHeight;
//    verticeArr[0+8*1]=width/winWith;
//    verticeArr[1+8*1]=-height/winHeight;
//    verticeArr[0+8*2]=-width/winWith;
//    verticeArr[1+8*2]=-height/winHeight;
//    verticeArr[0+8*3]=-width/winWith;
//    verticeArr[1+8*3]=height/winHeight;
//
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
//    int shaderID=shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureAphaMixReverseX.fs");
//
//    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
//    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
//
//    return [vao,texID1,texID2](){
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,texID1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D,texID2);
//        glBindVertexArray(vao);
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//    };
//}
//
//static float s_mixValue;
//static float s_mixValueInner;
//
//static std::function<void(void)> beginBasicViewMixPng5(){
//    float verticeArr[]={
//           0.5,0.5,0,  1,0,0,  1,1,
//           0.5,-0.5,0, 0,1,0,  1,0,
//           -0.5,-0.5,0,0,0,1,  0,0,
//           -0.5,0.5,0, 1,1,0,  0,1
//       };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID1=textureMgr::getInstance()->getTextureID("res/wall.jpg",GL_RGB);
//    if(!texID1)
//        return nullptr;
//    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
//    if(!texID2)
//       return nullptr;
//
//    glBindTexture(GL_TEXTURE_2D,texID1);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    glBindTexture(GL_TEXTURE_2D,texID2);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
//    size winSize=getWindowSize();
//    float winWith=winSize.width;
//    float winHeight=winSize.height;
//    float width=texSize.width;
//    float height=texSize.height;
//    verticeArr[0]=width/winWith;
//    verticeArr[1]=height/winHeight;
//    verticeArr[0+8*1]=width/winWith;
//    verticeArr[1+8*1]=-height/winHeight;
//    verticeArr[0+8*2]=-width/winWith;
//    verticeArr[1+8*2]=-height/winHeight;
//    verticeArr[0+8*3]=-width/winWith;
//    verticeArr[1+8*3]=height/winHeight;
//
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
//    int shaderID=shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureAphaMixControl.fs");
//
//    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
//    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
//
//    s_mixValue=0.2;
//    s_mixValueInner=s_mixValue/10;
//    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), s_mixValue);
//    keyboardEvent* eventObj=new keyboardEvent();
//
//    eventObj->regEvent('w', [=](){
//        cout<<"w mixValue is "<<s_mixValue<<endl;
//        s_mixValue+=s_mixValueInner;
//        if(s_mixValue>1)
//            s_mixValue=1;
//        glUniform1f(glGetUniformLocation(shaderID, "mixValue"), s_mixValue);
//        cout<<"w set mixValue "<<s_mixValue<<endl;
//    });
//    eventObj->regEvent('s', [=](){
//          s_mixValue-=s_mixValueInner;
//          if(s_mixValue<0.001)
//              s_mixValue=0;
//          glUniform1f(glGetUniformLocation(shaderID, "mixValue"), s_mixValue);
//          cout<<"s set mixValue "<<s_mixValue<<endl;
//      });
//    keyboardEventMgr::addEvent("test2D_beginBasicViewMixPng5", eventObj);
//
//    //必须要把shaderID放进capture list中，否则栈变量shaderID已经为0
//    return [vao,texID1,texID2,shaderID](){
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,texID1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D,texID2);
//
//        glBindVertexArray(vao);
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//    };
//}
//
//static std::function<void(void)> beginBasicViewTransPng(int shaderID){
//    float verticeArr[]={
//        0.5,0.5,0,  1,0,0,  1,1,
//        0.5,-0.5,0, 0,1,0,  1,0,
//        -0.5,-0.5,0,0,0,1,  0,0,
//        -0.5,0.5,0, 1,1,0,  0,1
//    };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
//    if(!texID1)
//        return nullptr;
//    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
//    if(!texID2)
//       return nullptr;
//    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
//    size winSize=getWindowSize();
//    float winWith=winSize.width;
//    float winHeight=winSize.height;
//    float width=texSize.width;
//    float height=texSize.height;
//    verticeArr[0]=width/winWith;
//    verticeArr[1]=height/winHeight;
//    verticeArr[0+8*1]=width/winWith;
//    verticeArr[1+8*1]=-height/winHeight;
//    verticeArr[0+8*2]=-width/winWith;
//    verticeArr[1+8*2]=-height/winHeight;
//    verticeArr[0+8*3]=-width/winWith;
//    verticeArr[1+8*3]=height/winHeight;
//
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
//    glm::mat4 matTrans=glm::mat4(1.0);
//    matTrans=glm::rotate(matTrans,glm::radians(60.0f),glm::vec3(0,0,1));
//    matTrans=glm::scale(matTrans,glm::vec3(0.5,0.5,0.5));
//
//    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
//    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
//    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matrixTrans"), 1,GL_FALSE,glm::value_ptr(matTrans));
//
//    return [vao,texID1,texID2](){
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,texID1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D,texID2);
//        glBindVertexArray(vao);
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//    };
//}
//
//static std::function<void(void)> beginBasicViewTransPng2(int shaderID){
//    float verticeArr[]={
//        0.5,0.5,0,  1,0,0,  1,1,
//        0.5,-0.5,0, 0,1,0,  1,0,
//        -0.5,-0.5,0,0,0,1,  0,0,
//        -0.5,0.5,0, 1,1,0,  0,1
//    };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
//    if(!texID1)
//        return nullptr;
//    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
//    if(!texID2)
//       return nullptr;
//    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
//    size winSize=getWindowSize();
//    float winWith=winSize.width;
//    float winHeight=winSize.height;
//    float width=texSize.width;
//    float height=texSize.height;
//    verticeArr[0]=width/winWith;
//    verticeArr[1]=height/winHeight;
//    verticeArr[0+8*1]=width/winWith;
//    verticeArr[1+8*1]=-height/winHeight;
//    verticeArr[0+8*2]=-width/winWith;
//    verticeArr[1+8*2]=-height/winHeight;
//    verticeArr[0+8*3]=-width/winWith;
//    verticeArr[1+8*3]=height/winHeight;
//
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
//    glm::mat4 matTrans=glm::mat4(1.0);
//    matTrans=glm::translate(matTrans,glm::vec3(0.5,-0.5,0));
//    matTrans=glm::rotate(matTrans,(float)glfwGetTime(),glm::vec3(0,0,1));
//
//    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
//    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
//    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matrixTrans"), 1,GL_FALSE,glm::value_ptr(matTrans));
//
//    return [vao,texID1,texID2,shaderID](){
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,texID1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D,texID2);
//        glBindVertexArray(vao);
//
//        glm::mat4 matTrans=glm::mat4(1.0);
//        matTrans=glm::translate(matTrans,glm::vec3(0.5,-0.5,0));
//        matTrans=glm::rotate(matTrans,(float)glfwGetTime(),glm::vec3(0,0,1));
//        glUniformMatrix4fv(glGetUniformLocation(shaderID, "matrixTrans"), 1,GL_FALSE,glm::value_ptr(matTrans));
//
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//    };
//}
//
//static std::function<void(void)> beginBasicViewTransPng3(int shaderID){
//    float verticeArr[]={
//        0.5,0.5,0,  1,0,0,  1,1,
//        0.5,-0.5,0, 0,1,0,  1,0,
//        -0.5,-0.5,0,0,0,1,  0,0,
//        -0.5,0.5,0, 1,1,0,  0,1
//    };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
//    if(!texID1)
//        return nullptr;
//    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
//    if(!texID2)
//       return nullptr;
//    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
//    size winSize=getWindowSize();
//    float winWith=winSize.width;
//    float winHeight=winSize.height;
//    float width=texSize.width;
//    float height=texSize.height;
//    verticeArr[0]=width/winWith;
//    verticeArr[1]=height/winHeight;
//    verticeArr[0+8*1]=width/winWith;
//    verticeArr[1+8*1]=-height/winHeight;
//    verticeArr[0+8*2]=-width/winWith;
//    verticeArr[1+8*2]=-height/winHeight;
//    verticeArr[0+8*3]=-width/winWith;
//    verticeArr[1+8*3]=height/winHeight;
//
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
//    glm::mat4 matTrans=glm::mat4(1.0);
//
//   //先移动，再旋转
////    matTrans=glm::rotate(matTrans,glm::radians(45.0f),glm::vec3(0,0,1));
////    matTrans=glm::translate(matTrans,glm::vec3(0.5,-0.5,0));
//
//    //先旋转，再移动
//    matTrans=glm::translate(matTrans,glm::vec3(0.5,-0.5,0));
//    matTrans=glm::rotate(matTrans,glm::radians(45.0f),glm::vec3(0,0,1));
//
//    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
//    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);
//    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matrixTrans"), 1,GL_FALSE,glm::value_ptr(matTrans));
//
//    return [vao,texID1,texID2,shaderID](){
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,texID1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D,texID2);
//        glBindVertexArray(vao);
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//    };
//}
//
//static std::function<void(void)> beginBasicViewTransPng4(int shaderID1,int shaderID2){
//    float verticeArr[]={
//        0.5,0.5,0,  1,0,0,  1,1,
//        0.5,-0.5,0, 0,1,0,  1,0,
//        -0.5,-0.5,0,0,0,1,  0,0,
//        -0.5,0.5,0, 1,1,0,  0,1
//    };
//    unsigned int verticeIndexArr[]={
//        0,1,3,      //first triangle
//        1,2,3       //second triangle
//    };
//
//    int texID1=textureMgr::getInstance()->getTextureID("res/fire.png",GL_RGBA);
//    if(!texID1)
//        return nullptr;
//    int texID2=textureMgr::getInstance()->getTextureID("res/smile.png",GL_RGBA);
//    if(!texID2)
//       return nullptr;
//    size texSize=textureMgr::getInstance()->getTextureSize("res/fire.png");
//    size winSize=getWindowSize();
//    float winWith=winSize.width;
//    float winHeight=winSize.height;
//    float width=texSize.width;
//    float height=texSize.height;
//    verticeArr[0]=width/winWith;
//    verticeArr[1]=height/winHeight;
//    verticeArr[0+8*1]=width/winWith;
//    verticeArr[1+8*1]=-height/winHeight;
//    verticeArr[0+8*2]=-width/winWith;
//    verticeArr[1+8*2]=-height/winHeight;
//    verticeArr[0+8*3]=-width/winWith;
//    verticeArr[1+8*3]=height/winHeight;
//
//    //void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
//    //int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos
//    int vao=VAOMgr::createVAO(verticeArr,sizeof(verticeArr),verticeIndexArr,sizeof(verticeIndexArr),3,4,2,8*sizeof(float),0,3*sizeof(float),6*sizeof(float),false);
//
////    glm::mat4 matIdentity=glm::mat4(1.0);
//
////    glUniform1i(glGetUniformLocation(shaderID1, "texture1"), 0);
////    glUniform1i(glGetUniformLocation(shaderID1, "texture2"), 1);
////    glUniformMatrix4fv(glGetUniformLocation(shaderID1, "matrixTrans"), 1,GL_FALSE,glm::value_ptr(matIdentity));
//
////    glUniform1i(glGetUniformLocation(shaderID2, "texture1"), 0);
////    glUniform1i(glGetUniformLocation(shaderID2, "texture2"), 1);
////    glUniformMatrix4fv(glGetUniformLocation(shaderID2, "matrixTrans"), 1,GL_FALSE,glm::value_ptr(matIdentity));
//
//    return [vao,texID1,texID2,shaderID1,shaderID2](){
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,texID1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D,texID2);
//        glBindVertexArray(vao);
//
//        glm::mat4 matTrans=glm::mat4(1.0);
//        matTrans=glm::translate(matTrans,glm::vec3(0.5,-0.5,0));
//        matTrans=glm::rotate(matTrans,(float)glfwGetTime(),glm::vec3(0,0,1));
//        glUniform1i(glGetUniformLocation(shaderID1, "texture1"), 0);
//        glUniform1i(glGetUniformLocation(shaderID1, "texture2"), 1);
//        glUniformMatrix4fv(glGetUniformLocation(shaderID1, "matrixTrans"), 1,GL_FALSE,glm::value_ptr(matTrans));
//        glUseProgram(shaderID1);
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//
//        matTrans=glm::mat4(1.0);
//        matTrans=glm::translate(matTrans,glm::vec3(-0.5,0.5,0));
//        float scale=sin((float)glfwGetTime());
//        matTrans=glm::scale(matTrans,glm::vec3(scale,scale,0));
//        glUniformMatrix4fv(glGetUniformLocation(shaderID2, "matrixTrans"), 1,GL_FALSE,glm::value_ptr(matTrans));
//        glUseProgram(shaderID2);
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//    };
//}
//
//void test2dViewJpg() {
//    std::function<void(void)> drawCall=beginBasicViewJpg();
//    testInitWindow2D("openGL 2D view test",drawCall,shaderMgr::createShader("res/shader/2dTexture.vs", "res/shader/2dTexture.fs"));
//    windowLoop();
//}
//
//void test2dViewPng() {
//    std::function<void(void)> drawCall=beginBasicViewPng();
//    testInitWindow2D("openGL 2D png test",drawCall,shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureApha.fs"));
//    windowLoop();
//}
//
//void test2dViewMixPng() {
//    std::function<void(void)> drawCall=beginBasicViewMixPng();
//    testInitWindow2D("openGL 2D png mix test",drawCall,shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureAphaMix.fs"));
//    windowLoop();
//}
//
//void test2dViewMixPng2() {
//    std::function<void(void)> drawCall=beginBasicViewMixPng2();
//    testInitWindow2D("openGL 2D png mix test",drawCall,shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureAphaMix.fs"));
//    windowLoop();
//}
//
//void test2dViewMixPng3() {
//    std::function<void(void)> drawCall=beginBasicViewMixPng3();
//    testInitWindow2D("openGL 2D png mix test",drawCall,shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureAphaMixReverseY.fs"));
//    windowLoop();
//}
//
//void test2dViewMixPng4() {
//    std::function<void(void)> drawCall=beginBasicViewMixPng4();
//    testInitWindow2D("openGL 2D png mix test",drawCall,shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureAphaMixReverseX.fs"));
//    windowLoop();
//}
//
//void test2dViewMixPng5() {
//    std::function<void(void)> drawCall=beginBasicViewMixPng5();
//    testInitWindow2D("openGL 2D png mix test",drawCall,shaderMgr::createShader("res/shader/2dTextureApha.vs", "res/shader/2dTextureAphaMixControl.fs"));
//    windowLoop();
//}
//
//void test2dViewTransPng() {
//    int shaderID=shaderMgr::createShader("res/shader/2dTextureAphaTrans.vs","res/shader/2dTextureAphaMix.fs");
//    std::function<void(void)> drawCall=beginBasicViewTransPng(shaderID);
//    testInitWindow2D("openGL 2D png trans test",drawCall,shaderID);
//    windowLoop();
//}
//
//void test2dViewTransPng2() {
//    int shaderID=shaderMgr::createShader("res/shader/2dTextureAphaTrans.vs","res/shader/2dTextureAphaMix.fs");
//    std::function<void(void)> drawCall=beginBasicViewTransPng2(shaderID);
//    testInitWindow2D("openGL 2D png trans test",drawCall,shaderID);
//    windowLoop();
//}
//
//void test2dViewTransPng3() {
//    int shaderID=shaderMgr::createShader("res/shader/2dTextureAphaTrans.vs","res/shader/2dTextureAphaMix.fs");
//    std::function<void(void)> drawCall=beginBasicViewTransPng3(shaderID);
//    testInitWindow2D("openGL 2D png trans test",drawCall,shaderID);
//    windowLoop();
//}
//
//void test2dViewTransPng4() {
//    int shaderID1=shaderMgr::createShaderWithoutCache("res/shader/2dTextureAphaTrans.vs","res/shader/2dTextureAphaMix.fs");
//    int shaderID2=shaderMgr::createShaderWithoutCache("res/shader/2dTextureAphaTrans.vs","res/shader/2dTextureAphaMix.fs");
//    std::function<void(void)> drawCall=beginBasicViewTransPng4(shaderID1,shaderID2);
//    testInitWindow2D("openGL 2D png trans test",drawCall,shaderID1);
//    windowLoop();
//}
//
//
//void test2dTrans() {
//    std::function<void(void)> drawCall=beginTransView();
//    testInitWindow2D("openGL 2D trans test",drawCall,0);
//    windowLoop();
//}
