//
//  stringUtil.h
//  flyEngine
//
//  Created by joe on 22/09/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef stringUtil_h
#define stringUtil_h

#include <stdio.h>
#include <string>

class stringUtil{
    public :
        static std::string getFileExtName(std::string fileName);
        static char* utoa(unsigned x, char* dest, const unsigned base);
};


#endif /* stringUtil_h */
