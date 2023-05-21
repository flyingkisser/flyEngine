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
#include <vector>
#include <tuple>

#include "defines.h"

#ifdef BUILD_IOS
#import "ViewController.h"
#endif

NS_FLYENGINE_BEGIN

class node;
class control;
class camera;
class directionLight;
class light;
class pointLight;
class spotLight;
class areaLight;
class shader;

class world{
private:
    std::vector<node*> _vector_child;
    std::vector<light*> _vector_light;
    std::vector<pointLight*> _vector_point_light;
    std::vector<spotLight*> _vector_spot_light;
    std::vector<areaLight*> _vector_area_light;
    std::function <void()> _cb_before_any_gl_call=nullptr;
    std::function <void()> _cb_after_any_gl_call=nullptr;
    std::function <void()> _cb_before_draw_call=nullptr;
    camera* _camera=NULL;
    directionLight* m_directionLight=NULL;
    std::vector<std::tuple<unsigned int,shader*,std::function<void()>>> _vector_pass;
#ifdef BUILD_IOS
    GLKView* _view=NULL;
#endif
    long long _totalRenderNS=0;
    long long _totalRenderLoops=0;
    
    void _drawAllChild(shader* sh);
    void _main_loop();
    void _draw();
    void _renderOnce();
    
public:
    world();
    ~world();
    static world* getInstance();
    
    void addChild(node* node);
    void removeChild(node* node);
    void addLight(light* node);
    void addPointLight(pointLight* node);
    void addSpotLight(spotLight* node);
    void addAreaLight(areaLight* node);
    void setCamera(camera* c);
    void setDirectiontLight(directionLight* directionLight);
    void start_rendering();
    void pause();
    void end();
   
    control* getControl();
    
    directionLight* getDirectiontLight(){return m_directionLight;};
    std::vector<light*> getLightVector(){return _vector_light;};
    std::vector<pointLight*> getPointLightVector(){return _vector_point_light;};
    std::vector<spotLight*> getSpotLightVector(){return _vector_spot_light;};
    std::vector<areaLight*> getAreaLightVector(){return _vector_area_light;};
   
    camera* getCamera(){return _camera;};
    
    float getFrameRate(){return CONST_FRAME_RATE;};
    void setCBBeforeDrawCall(std::function<void()> f){_cb_before_draw_call=f;};
    void setCBBeforeAnyGLCall(std::function<void()> f){_cb_before_any_gl_call=f;};
    void setCBAfterAnyGLCall(std::function<void()> f){_cb_after_any_gl_call=f;};
  
    void addPass(unsigned int fbo,shader* sh,std::function<void()> f);
    
    void printRenderTime();
    double getRenderTimeMS();
   
};

NS_FLYENGINE_END


#endif /* world_h */
