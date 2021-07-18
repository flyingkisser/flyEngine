//
//  dirUtil.hpp
//  lines
//
//  Created by joe on 22/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef dirUtil_h
#define dirUtil_h

#include <stdio.h>



class dirUtil{
public:
    static void getCurrentWorkDir(char *szBuf,int bufSize);
    static void getHomeDir(char *szBuf,int bufSize);
    static void setCurrentWorkDir(const char* szBuf);
};
#endif /* dirUtil_h */
