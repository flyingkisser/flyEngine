//
//  windowUtil.h
//  flyEngine_ios
//
//  Created by joe on 15/08/2022.
//

#ifndef windowUtil_h
#define windowUtil_h

#include <stdio.h>
#include "defines.h"

NS_FLYENGINE_BEGIN

class windowUtil{
public:
    static flyEngine::size getWinSize();
    static flyEngine::size getResolutionSize();
};

NS_FLYENGINE_END

#endif /* windowUtil_h */
