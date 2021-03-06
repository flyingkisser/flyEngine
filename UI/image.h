//
//  image.h
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef image_h
#define image_h

#include "flyEngine.h"



namespace ui {

class image : node{
private:
    unsigned char* _buf;
    
    void _initBMP(unsigned char* szBuf);
public:
    image(const char* szFileName);
    void draw();
    
    size getSize();
};

}



#endif /* image_h */
