//
//  texture2.cpp
//  flyEngine
//
//  Created by joe on 04/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "texture2.h"
//#include "baseImg.h"
#include "pngUtil.h"
#include "jpgUtil.h"
#include "logUtil.h"
#include "stb_image.h"

#ifdef BUILD_IOS
#include "ios_dirUtil.h"
#endif

USE_NS_FLYENGINE

texture2::~texture2(){
    if(_dataBuf){
        free(_dataBuf);
        flylog("~texture:_dataBuf %llu freed!",_dataBuf);
    }
}

texture2::texture2(const char* szPath,bool bFlipY){
#ifdef BUILD_IOS
    const char* szfullPath=ios_dirUtil::getFileFullPathName(szPath);
    _strPath=szfullPath;
#else
    _strPath=szPath;
#endif
    _bFlipY=bFlipY;
}

bool texture2::init(){
    char* szPath=(char*)_strPath.c_str();
    struct_texture st={0};
    if(pngUtil::isPng(szPath)){
        if(!pngUtil::loadFile(szPath,&st,_bFlipY)){
            flylog("texture.init:png loadFile %s failed",szPath);
            return false;
        }
        _width=st.width;
        _height=st.height;
        _format=st.format;
        _dataBuf=st.buf;
        return true;
    }else if(jpgUtil::isJpg(szPath)){
        if(!jpgUtil::loadFile(szPath, &st,_bFlipY)){
            flylog("texture.init:jpg loadFile %s failed",szPath);
            return false;
        }
        _width=st.width;
        _height=st.height;
        _format=st.format;
        _dataBuf=st.buf;
        return true;
    }else{
        int width, height, nrComponents;
        // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
        if(_bFlipY)
            stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(szPath, &width, &height, &nrComponents, 0);
        _width=width;
        _height=height;
        if (nrComponents == 1)
           _format = GL_RED;
        else if (nrComponents == 3)
           _format = GL_RGB;
        else if (nrComponents == 4)
           _format = GL_RGBA;
        _dataBuf=data;
        return true;
    }
}

flyEngine::size texture2::getSize(){
  return flyEngine::size{(float)_width,(float)_height};
};


//texturePos from GL_TEXTURE0,GL_TEXTURE1
void texture2::glInit(int texturePos){
    glRef::glInit();
    glGenTextures(1,&_textureID);
    if(texturePos){
        glActiveTexture(texturePos);
        _texturePos=texturePos-GL_TEXTURE0;
    }
       
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
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _format == GL_RGBA ? GL_CLAMP_TO_EDGE :GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _format == GL_RGBA ? GL_CLAMP_TO_EDGE :GL_REPEAT);
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
    flylog("glTexImage2D from buf %llu begin %d*%d %d",_dataBuf,_width,_height,_width*_height);
    glTexImage2D(GL_TEXTURE_2D,0,_format,_width,_height,0,_format,GL_UNSIGNED_BYTE,_dataBuf);
    flylog("glTexImage2D from buf %llu end",_dataBuf);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, structTex.width, structTex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, structTex.buf);
}
