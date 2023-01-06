//
//  jpgUtil.cpp
//  flyEngine
//
//  Created by joe on 05/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdexcept>
#include "jpeglib.h"
#include "jerror.h"
#include "defines.h"
#include "jpgUtil.h"
using namespace flyEngine;

static void getJPGTextureInfo(int num_components,  struct_texture *texinfo){
    switch (num_components){
        case 1:
            texinfo->format = GL_LUMINANCE;
            break;
        case 2:
            texinfo->format = GL_LUMINANCE_ALPHA;
            break;
        case 3:
            texinfo->format = GL_RGB;
            break;
        default:
            /* Badness */
            break;
    }
}

void on_jpg_error(jpeg_common_struct* st){
    fprintf(stderr, "on_jpg_error,do nothing,return!\n");
//    my_error_ptr myerr=(my_error_ptr)st->err;
    (*st->err->output_message)(st);
    throw std::runtime_error("on_jpg_error");
//    longjmp(myerr->setjmp_buffer,1);
//    jpeg_destroy_decompress(st);
}

bool jpgUtil::isJpg(const char *filename)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *fp = NULL;
    fp = fopen(filename, "rb");
    if (!fp){
        fprintf(stderr, "jpgUtil::isJpg error: couldn't open \"%s\"!\n", filename);
        return false;
    }
    //绑定标准错误处理结构
    cinfo.err = jpeg_std_error(&jerr);
    jerr.error_exit=on_jpg_error;
    //读取图像信息
    try{
        //初始化JPEG对象
        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, fp);
        jpeg_read_header(&cinfo, TRUE);
        if(cinfo.image_width==0 || cinfo.image_height==0)
          return false;
        return true;
    }catch(std::runtime_error error){
        //fprintf(stderr, "jpgUtil::isJpg error:%s\n", error.what());
        return false;
    }
}

bool jpgUtil::loadFile(const char *filename,struct_texture* texinfo,bool bFlipY)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *fp = NULL;
    
    /* Open image file */
    fp = fopen(filename, "rb");
    if (!fp){
        fprintf(stderr, "jpgUtil::loadFile error: couldn't open \"%s\"!\n", filename);
        return false;
    }
    //绑定标准错误处理结构
    cinfo.err = jpeg_std_error(&jerr);
    //初始化JPEG对象
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    //读取图像信息
    jpeg_read_header(&cinfo, TRUE);
    if(cinfo.image_width==0 || cinfo.image_height==0)
        return false;
    
    //设定解压缩参数，此处我们将图像长宽缩小为原图的1/1
    cinfo.scale_num=1;
    cinfo.scale_denom=1;
    //开始解压缩图像
    jpeg_start_decompress(&cinfo);
    //设置texinfo结构
//    texinfo = (struct_texture *)malloc(sizeof(struct_texture));
    texinfo->width=cinfo.image_width;
    texinfo->height=cinfo.image_height;
    texinfo->internalFormat=cinfo.num_components;
    getJPGTextureInfo(cinfo.num_components,texinfo);
    
    //分配缓冲区空间
    int bufSize=sizeof(GLubyte) * texinfo->width * texinfo->height * cinfo.num_components;
    texinfo->buf = (GLubyte *)malloc(bufSize);
    memset(texinfo->buf,0,bufSize);
//    printf("jpg at 0x%llx size 0x%x %d",texinfo->buf,bufSize,bufSize);
    
    //创建每一行数据的指针
    unsigned char** rowPtr = new unsigned char*[texinfo->height];
    if(bFlipY)
        for (int i=0;i<texinfo->height;i++)
            rowPtr[i] = &(texinfo->buf[i*texinfo->width*cinfo.num_components]);
    else
        //从jpg读取的第一行，放进缓存的最后一行
        for (int i=0;i<texinfo->height;i++)
            rowPtr[i] = &(texinfo->buf[(texinfo->height-1-i)*texinfo->width*cinfo.num_components]);
    //读取数据
    int rowsRead = 0;
    while(cinfo.output_scanline < cinfo.output_height)
        rowsRead+=jpeg_read_scanlines(&cinfo,&rowPtr[rowsRead],cinfo.output_height-rowsRead);
    delete [] rowPtr;
    //结束解压缩操作
    jpeg_finish_decompress(&cinfo);
    //释放资源
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);
//    fprintf(stdout, "jpgUtil::alloct buf %d(%x) at %llu for %s\n", bufSize,bufSize,texinfo->buf,filename);
    return true;
}
