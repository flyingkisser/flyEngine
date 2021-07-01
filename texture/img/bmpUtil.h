//
//  bmpUtil.h
//  flyEngine
//
//  Created by joe on 03/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef bmpUtil_h
#define bmpUtil_h

#include <stdio.h>

namespace flyEngine{

class bmpUtil{
public:
    static void drawBMP(float x,float y,unsigned char* szBuf);
};

}

#endif /* bmpUtil_h */

