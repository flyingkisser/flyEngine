//
//  glRef.h
//  flyEngine
//
//  Created by joe on 08/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef glRef_h
#define glRef_h

#include <stdio.h>

namespace flyEngine {

class glRef{
private:
    bool _gl_inited=false;
    bool _gl_pos_dirty=false;
public:
    virtual void glInit(){_gl_inited=true;};
};

}


#endif /* glRef_h */
