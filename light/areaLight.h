//
//  areaLight.h
//  flyEngine
//
//  Created by joe on 22/03/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef areaLight_h
#define areaLight_h

#include <stdio.h>
#include "defines.h"
#include "light.h"

NS_FLYENGINE_BEGIN

class areaLight:public light{
private:
    unsigned int _texForMat1;
    unsigned int _texForMat2;
    glm::vec3 _normal;
    bool _twoSided=false;
    float _intensity=4.0f;
    float _roughness=0.5f;
    int _ubo=0;
public:
    areaLight(glm::vec3 color,glm::vec3 normal=glm::vec3(0,1,0),material2* mt=NULL);

    bool init();
    void glUpdate(int light_index);
    void update(int light_index);
    glm::vec3 getNormal(){return _normal;};
    void setNormal(glm::vec3 d){_normal=d;};
    
    void setRotation(glm::vec3 v);
    void rotateBy(glm::vec3 v);
    
    void draw();
    glm::vec3 getVerticePosByIndex(int index);
    unsigned int getTex1(){return _texForMat1;};
    unsigned int getTex2(){return _texForMat2;};
    void setTowSided(bool v){_twoSided=v; setDirtyUBO(true);};
    bool getTowSided(){return _twoSided;};
    
    void setIntensity(float v){_intensity=v; setDirtyUBO(true);};
    float getIntensity(){return _intensity;};
    
    void setRoughness(float v){_roughness=v; setDirtyUBO(true);};
    float getRoughness(){return _roughness;};
};

NS_FLYENGINE_END
#endif /* areaLight_h */
