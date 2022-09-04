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
        static std::string getFileExtName(std::string& fileName);
        static char* utoa(unsigned x, char* dest, const unsigned base);
        static std::string float2Str(float s);
        static std::string getStrBefore(std::string str1,std::string strFind);
        static std::string getStrAfter(std::string str1,std::string strFind);
        static std::string getStrBeforeFromLast(std::string str1,std::string strFind);
        static std::string getStrAfterFromLast(std::string str1,std::string strFind);
        static std::string replace(std::string str1,std::string strFind,std::string strReplace);
    
        static const char* replace(const char* str1,const char* strFind,const char* strReplace);
    
        static void test();
        
};


#endif /* stringUtil_h */
