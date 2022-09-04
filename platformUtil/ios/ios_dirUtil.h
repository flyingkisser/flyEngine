//
//  ios_dirUtil.h
//  flyEngine_ios
//
//  Created by joe on 15/08/2022.
//

#ifndef ios_dirUtil_h
#define ios_dirUtil_h

#include <stdio.h>
#include "defines.h"

NS_FLYENGINE_BEGIN

class ios_dirUtil {
public:
    static const char* getFileFullPathName(const char* fileName);
    static const char* getBundleRootPath();
    static const char* getDocumentPath();
    static const char* getCachePath();
    static const char* getApplicationSupportPath();
    static const char* getTmpPath();
    static const char* getHomePath();
    static void test();
};

NS_FLYENGINE_END

#endif /* ios_dirUtil_h */
