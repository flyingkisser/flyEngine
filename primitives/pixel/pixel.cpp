//
//  pixel.cpp
//  lines
//
//  Created by joe on 12/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "pixel.h"
#include "flyEngine.h"

 GLubyte bitShape[20]={
       0x1c,0x00,0x1c,0x00,0x1c,0x00,0x1c,0x00,0x1c,0x00,
       0xff,0x80,0x7f,0x00,0x3e,0x00,0x1c,0x00,0x08,0x00
   };
GLubyte bitmapCharA[]={0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18};

GLubyte bitmapCharB[]=
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18, //a
0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, //b
0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e, //c
0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc, //d
0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, //e
0xc0, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff //f
};

void pixelDrawBitmap(){
     
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glColor3f(1,0,0);
    
    //设置光珊位置
    glRasterPos2i(20,60);
    
    //把bitShape指定的位图数据绘制出来
    //9列，10行，即宽9高10
    //阵列的原点指定为0,0，在网络的左下角
    //xOffset,yOffset 位图显示后更新帧缓存当前光栅位置的坐标位移
    //width,height,x0,y0,xOffset,yOffset,bitShape
    glBitmap(9,10,0,0,0,0,bitShape);
    
//  glBitmap(8,13,0,2,10,0,bitmapCharA);
    glFlush();
}

void pixelDrawPixels(){
    //初始化一段像素值
    int imgWidth=64;
    int imgHigh=64;
    GLubyte imgBuf[64][64][3]={0};
    for(int i=0;i<imgWidth;i++){
        for(int j=0;j<imgHigh;j++){
            int c=((((i&0x8)==0)^((j&0x8))==0))*255;
            imgBuf[i][j][0]=(GLubyte)c;
            imgBuf[i][j][1]=(GLubyte)c;
            imgBuf[i][j][2]=(GLubyte)c;
        }
    }
//    GLubyte pixelBuf[10*10]={0};
    //GL_FRONT_LEFT     前左缓存(默认）
    //GL_FRONT_RIGHT    前右缓存
    //GL_BACK_LEFT      后左缓存
    //GL_BACK_RIGHT     后右缓存
    //GL_FRONT           两个前缓存
    //GL_BACK           两个后缓存
    //GL_LEFT           一对左缓存
    //GL_RIGHT          一对右缓存
    //GL_FRONT_AND_BACK 所有可用的颜色缓存
    //GL_AUX0           辅助缓存0
    //GL_AUX1           辅助缓存1
    //GL_AUX2           辅助缓存2
    //GL_AUX3           辅助缓存3
    glColor3f(1,0,0);
    glRasterPos2i(20,60);
    glDrawBuffer(GL_FRONT);
    //width,height,dataFormat,dataType,pixMap
    glDrawPixels(imgWidth,imgHigh,GL_RGB,GL_UNSIGNED_BYTE,imgBuf);
    glFlush();
}

void drawBMP(const char* fileName){
    unsigned char* szBuf=(unsigned char*)readFile(fileName);
    if(!szBuf){
        printf("cannot read file %s",fileName);
        return;
    }
    
    int a=szBuf[0x12];
    int b=szBuf[0x13];
    int c=szBuf[0x14];
    int d=szBuf[0x15];
    int imgWidth=d<<24 ^ c<<16 ^ b <<8 ^ a;
    
    a=szBuf[0x16];
    b=szBuf[0x17];
    c=szBuf[0x18];
    d=szBuf[0x19];
    int imgHigh=d<<24 ^ c<<16 ^ b <<8 ^ a;
    
    unsigned char* bufBMP=(unsigned char*)(szBuf+54);
    
    glColor3f(1,0,0);
    glRasterPos2i(20,60);
    glDrawBuffer(GL_FRONT);
    //width,height,dataFormat,dataType,pixMap
    glDrawPixels(imgWidth,imgHigh,GL_BGR,GL_UNSIGNED_BYTE,bufBMP);
    glFlush();
    free(szBuf);
}

void snapsBMP(const char* fileNameSave){
    int width=640;
    int height=1136;
    int bufSize=width*height*3+54;
    unsigned char* szBuf=(unsigned char* )malloc(bufSize);
    memset(szBuf,0,bufSize);
    
    typedef struct {
        /* type : Magic identifier,一般为BM(0x42,0x4d) */
        unsigned short int type;
        unsigned int size;/* File size in bytes,全部的档案大小 */
        unsigned short reserved1, reserved2; /* 保留位 */
        unsigned int offset;/* Offset to image data, bytes */
    } __attribute__((packed)) FILEHEADER;
    
    typedef struct {
        unsigned int size;/* Info Header size in bytes */
        int width,height;/* Width and height of image */
        unsigned short planes;/* Number of colour planes */
        unsigned short bits; /* Bits per pixel */
        unsigned int compression; /* Compression type */
        unsigned int imagesize; /* Image size in bytes */
        int xresolution,yresolution; /* Pixels per meter */
        unsigned int ncolours; /* Number of colours */
        unsigned int importantcolours; /* Important colours */
    } __attribute__((packed)) INFOHEADER;
    
    FILEHEADER* imgHeader=(FILEHEADER*)szBuf;
    imgHeader->type=0x4d42;
    imgHeader->size=bufSize;
    imgHeader->reserved1=0;
    imgHeader->reserved2=0;
    imgHeader->offset=54;
    
    INFOHEADER* imgInfo=(INFOHEADER*)(szBuf+14);
    imgInfo->size=40;
    imgInfo->width=width;
    imgInfo->height=height;
    imgInfo->planes=1;
    imgInfo->bits=24;
    imgInfo->compression=0;
    imgInfo->imagesize=0;
    imgInfo->xresolution=0xb13;
    imgInfo->yresolution=0xb13;
    imgInfo->ncolours=0;
    imgInfo->importantcolours=0;
    
    glReadBuffer(GL_FRONT);

//    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    //bmp图的像素使用的是bgr排列
     //xmin,ymin,width,height
    glReadPixels(20,60,640,1136,GL_BGR,GL_UNSIGNED_BYTE,szBuf+54);
    writeFile(fileNameSave, szBuf, bufSize);
    free(szBuf);
}

void pixelCopyPixels(){
    //指定源缓存
    glReadBuffer(GL_FRONT);
    //指定目标缓存
    glDrawBuffer(GL_BACK);
    //颜色进行操作
    glEnable(GL_COLOR_LOGIC_OP);
    //GL_AND
    //GL_OR
    //GL_XOR
    //GL_COPY   复制(默认值)
    //GL_COPY_INVERTED 取反后复制
    //GL_CLEAR  目标位清0
    glLogicOp(GL_COPY_INVERTED);
    //x,y,width,height,GL_DEPTH(GL_DEPTH_COMPONENT)或者GL_STENCIL(GL_STENCIL_INDEX)或者GL_COLOR(GL_RGBA或GL_COLOR_INDEX)
    glRasterPos2i(50,90);
    glCopyPixels(20,60,640,1136,GL_COLOR);
//    glutSwapBuffers();
    glFlush();
}
