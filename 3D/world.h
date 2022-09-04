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
#include <functional>
#include "defines.h"


NS_FLYENGINE_BEGIN

class node;
class control;
class camera;
class directionLight;
class light;
class pointLight;
class spotLight;

class world{
private:
    std::vector<node*> _vector_child;
    std::vector<light*> _vector_light;
    std::vector<pointLight*> _vector_point_light;
    std::vector<spotLight*> _vector_spot_light;
    std::function <void()> _cb_before_draw_call=nullptr;
    std::function <void()> _cb_after_draw_call=nullptr;
    std::function <void()> _cb_before_render=nullptr;
    camera* _camera=NULL;
    directionLight* m_directionLight=NULL;
    
public:
    world();
    ~world();
    static world* getInstance();
    
    void addChild(node* node);
    void removeChild(node* node);
    void addLight(light* node);
    void addPointLight(pointLight* node);
    void addSpotLight(spotLight* node);
    void setCamera(camera* c);
    void setDirectiontLight(directionLight* directionLight);
    void start_rendering();
    void pause();
    void end();
    void renderOnce();
    
    void main_loop();
    
    void draw();
    control* getControl();
    
    directionLight* getDirectiontLight(){return m_directionLight;};
    std::vector<light*> getLightVector(){return _vector_light;};
    std::vector<pointLight*> getPointLightVector(){return _vector_point_light;};
    std::vector<spotLight*> getSpotLightVector(){return _vector_spot_light;};
    
    camera* getCamera(){return _camera;};
    
    float getFrameRate(){return CONST_FRAME_RATE;};
    void setCBBeforeRender(std::function<void()> f){_cb_before_render=f;};
    void setCBBeforeDrawCall(std::function<void()> f){_cb_before_draw_call=f;};
    void setCBAfterDrawCall(std::function<void()> f){_cb_after_draw_call=f;};
  

   
};

NS_FLYENGINE_END


#endif /* world_h */
