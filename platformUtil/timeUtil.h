//
//  timeUtil.h
//  flyEngine
//
//  Created by joe on 19/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef timeUtil_h
#define timeUtil_h

#include <stdio.h>
#include <time.h>
#include <string>
#include <sys/time.h>

class timeUtil{
public:
    static long long getTime();
    static long long getTimeMS();
    static struct tm* getDate();
    static struct tm* getDateBySec(long long secTime);
    static int getTimeZoneDiff();
    static std::string getTimeStrLong();
};

#endif /* timeUtil_h */
