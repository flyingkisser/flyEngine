//
//  logUtil.h
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/6.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef logUtil_h
#define logUtil_h

#include <stdio.h>

class logUtil{
    static void log(const char* szFmt,...);
};

void flylog(const char* szFmt,...);

#endif /* logUtil_h */
