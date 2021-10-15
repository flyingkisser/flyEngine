//
//  world.h
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/27.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef world_h
#define world_h

#include <stdio.h>
#include "defines.h"


NS_FLYENGINE_BEGIN

class node;
class control;
class camera;
class ambientLight;
class light;
class pointLight;
class spotLight;

class world{
private:
    std::vector<node*> _vector_child;
    std::vector<light*> _vector_light;
    std::vector<pointLight*> _vector_point_light;
    std::vector<spotLight*> _vector_spot_light;
    camera* _camera;
    ambientLight* m_amLight;
    
public:
    world();
    ~world();
    static world* getInstance();
    void addChild(node* node);
    void addLight(light* node);
    void addPointLight(pointLight* node);
    void addSpotLight(spotLight* node);
    void setCamera(camera* c);
    void setAmbientLight(ambientLight* amLight);
    void start_rendering();
    void pause();
    void end();
    
    static void _main_loop();
    
    void draw();
    control* getControl();
    
    ambientLight* getAmbientLight(){return m_amLight;};
    std::vector<light*> getLightVector(){return _vector_light;};
    std::vector<pointLight*> getPointLightVector(){return _vector_point_light;};
    std::vector<spotLight*> getSpotLightVector(){return _vector_spot_light;};
    
    camera* getCamera(){return _camera;};
    
    float getFrameRate(){return CONST_FRAME_RATE;};
};

NS_FLYENGINE_END


#endif /* world_h */
