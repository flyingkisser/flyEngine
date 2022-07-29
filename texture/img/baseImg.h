//
//  base.h
//  flyEngine
//
//  Created by joe on 05/08/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef base_h
#define base_h
#include <tuple>

 //width,height,format,internalFormat,id,buf
using textureTuple=std::tuple<int,int,unsigned int,int,unsigned int,unsigned char*>;

struct struct_texture
{
    int width;
    int height;
    int format;
    int internalFormat;
    unsigned int id;
    unsigned char* buf;
};


#endif /* base_h */
