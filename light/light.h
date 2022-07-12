//
//  light.h
//  flyEngine
//
//  Created by joe on 10/08/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef light_h
#define light_h

#include <stdio.h>

#include <stdio.h>
#include <string>
#include <functional>

#include "defines.h"
#include "cubeColor.h"

using namespace std;

NS_FLYENGINE_BEGIN

class light: public cubeColor
{
private:

public:
    light(glm::vec3 color,material* mt);
    light(glm::vec3 color);
    ~light(){};
    
    bool init();
    //void glUpdateForCube(int program_id,int ligth_index);
    
   
    void setColor(glm::vec3 color);

    void draw(camera* cameraObj);
};

NS_FLYENGINE_END


#endif /* light_h */
