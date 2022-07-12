//
//  fontAscii.h
//  flyEngine
//
//  Created by joe on 12/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef fontAscii_h
#define fontAscii_h

#include <stdio.h>
#include "defines.h"
#include <vector>

namespace flyEngine{

struct AscStruct{
    int ascii;
    int texID;
    int width;
    int height;
    int bearingX;
    int bearingY;
    int advance;
};

class fontAscii{
public:
    fontAscii(){};
    ~fontAscii(){};
//    fontAscii* getInstance();
    bool init(const char* ttfFileName);
private:
    std::vector<AscStruct> m_vecAscStruct;
    
};
}
#endif /* fontAscii_h */
