//
//  texture.cpp
//  flyEngine
//
//  Created by joe on 24/06/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "texture.h"
#include "baseImg.h"
#include "pngUtil.h"
#include "jpgUtil.h"

using namespace flyEngine;

texture::~texture(){
    if(_dataBuf)
        free(_dataBuf);
}

texture::texture(const char* szPath){
    _strPath=szPath;
    struct_texture st={0};
   if(pngUtil::isPng(szPath)){
       if(!pngUtil::loadFile(szPath,&st))
           return;
   }else if(jpgUtil::isJpg(szPath)){
       if(!jpgUtil::loadFile(szPath, &st))
           return;
   }
    _width=st.width;
    _height=st.height;
    _format=st.format;
    _width=st.width;
    _dataBuf=st.buf;
}


bool texture::init(){
    char* szPath=(char*)_strPath.c_str();
    struct_texture st={0};
   if(pngUtil::isPng(szPath)){
       if(!pngUtil::loadFile(szPath,&st))
           return false;
       _colorType=GL_RGBA;
   }else if(jpgUtil::isJpg(szPath)){
       if(!jpgUtil::loadFile(szPath, &st))
           return false;
       _colorType=GL_RGB;
   }
    _width=st.width;
    _height=st.height;
    _format=st.format;
    _width=st.width;
    _dataBuf=st.buf;
    _glInit();
    return true;
}

void texture::_glInit(){
    glGenTextures(1,&_textureID);
    glBindTexture(GL_TEXTURE_2D,_textureID);
    //GL_TEXTURE_WRAP_S表示纹理坐标的横向方向
    //GL_TEXTURE_WRAP_T表示纹理坐标的纵向方向
    //表示当坐标超过纹理坐标范围时，应如何使用贴图
    //GL_REPEAT             重复贴图（默认值）
    //GL_MIRRORED_REPEAT    重复贴图的镜像
    //GL_CLAMP_TO_EDGE      从纹理的边界取出色值平铺
    //GL_CLAMP_TO_BORDER    用指定的颜色平铺
    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    //GL_NEAREST    计算离坐标点最近的一个纹理像素，取这个像素的色值（即计算纹理坐标点与纹理像素中心点的距离，找到最近的纹理像素）
    //GL_LINEAR     找到坐标点最近的四个纹理像素，取这四个像素色值的混合，距离越近，像素占比越大
    //GL_NEAREST_MIPMAP_NEAREST    找到尺寸最接近的mipmap，然后使用NEAREST的方式取样
    //GL_LINEAR_MIPMAP_NEAREST     找到尺寸最接近的mipmap，然后使用LINEAR的方式取样
    //GL_NEAREST_MIPMAP_LINEAR     找到尺寸最近的两个mipmap混合，然后使用NEAREST的方式取样
    //GL_LINEAR_MIPMAP_LINEAR      找到尺寸最近的两个mipmap混合，然后使用LINEAR的方式取样
    //如果在大几何体上使用小贴图，GL_NEAREST会有块状锯齿感，GL_LINEAR则更为平滑
    //GL_TEXTURE_MIN_FILTER表示当贴图需要缩小时使用哪种模式
    //GL_TEXTURE_MAG_FILTER表示当贴图需要放大时使用哪种模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glEnable(GL_TEXTURE_2D);
    //参数1:GL_TEXTURE_2D
    //参数2:指定为0，表示不是某一个大纹理数组的缩减
    //参数3:纹理的每一个颜色的格式
    //参数4:此纹理宽度
    //参数5:此纹理高度
    //参数6:指定为0，表示纹理周围没有边界，指定为1，表示纹理周围会有一个单像素宽的边界，用于与相邻图案融合
    //参数7:纹理图案中的颜色值类型，GL_RGBA
    //参数8:数据格式
    //参数9:纹理内存
    glTexImage2D(GL_TEXTURE_2D,0,0,_width,_height,0,_colorType,GL_UNSIGNED_BYTE,_dataBuf);
       
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, structTex.width, structTex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, structTex.buf);
}