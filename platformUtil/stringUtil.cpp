//
//  stringUtil.cpp
//  flyEngine
//
//  Created by joe on 22/09/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "stringUtil.h"

std::string stringUtil::getFileExtName(std::string fileName){
    std::size_t pos=fileName.find_last_of(".");
    if(pos!=std::string::npos)
        return fileName.substr(pos,fileName.length()-pos);
    return "";
}

char* stringUtil::utoa(unsigned x, char* dest, const unsigned base)
{
    char *p = dest;
    do{
        *p++ = "0123456789abcdef"[x % base];
        x /= base;
    }
    while(x);
    *p = 0;
    std::reverse(dest, p);
    return dest;
}
