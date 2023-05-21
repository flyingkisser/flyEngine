//
//  world.cpp
//  flyEngine
//
//  Created by wu mingzhou on 2021/6/27.
//  Copyright © 2021 joe. All rights reserved.
//

#include "world.h"
#include "node.h"
#include "control.h"
#include "camera.h"
#include "threadUtil.h"
#include "window.h"
#include "uiText.h"
#include "timeUtil.h"
#include "state.h"
#include "logUtil.h"

//#include "testWindow.h"

USE_NS_FLYENGINE

static world* s_instance;

world* world::getInstance(){
    if(s_instance==NULL)
        s_instance=new world();
    return s_instance;
}

world::world(){
    _camera=new flyEngine::camera();
    _camera->enableControl();
#ifdef BUILD_IOS
    _view=[ViewController getView];
#endif
}
void world::pause(){
    
}

void world::end(){
    
}

void world::addChild(node *node){
    _vector_child.push_back(node);
}

void world::addLight(light *node){
    _vector_light.push_back(node);
}
void world::addPointLight(pointLight *node){
    _vector_point_light.push_back(node);
}
void world::addSpotLight(spotLight *node){
    _vector_spot_light.push_back(node);
}
void world::addAreaLight(areaLight *node){
    _vector_area_light.push_back(node);
}


void world::setCamera(camera* c){
    _camera=c;
}

void world::setDirectiontLight(directionLight* directionLight){
    m_directionLight=directionLight;
}


control* world::getControl(){
    if(_camera==nullptr)
        return nullptr;
    return _camera->getControl();
}

void world::removeChild(node *nodeObj){
    std::vector<node*>::iterator it =std::find(_vector_child.begin(),_vector_child.end(),nodeObj);
    if(it==_vector_child.end())
        return;
    _vector_child.erase(it);
}

void world::_drawAllChild(shader* sh){
    for(auto c : _vector_child){
        node* nodeObj=c;
        if(!nodeObj->visible())
            continue;
        if(nodeObj->isCufOffSphereEnabled() && !nodeObj->isInFrustumBySphere())
            continue;
        if(nodeObj->isCufOffAABBEnabled() && !nodeObj->isInFrustumByAABB())
            continue;
        if(sh!=NULL)
            nodeObj->setShader(sh);
        nodeObj->draw();
    }
    for(auto c : _vector_point_light){
        node* nodeObj=(node*)c;
        if(!nodeObj->visible())
            continue;
        if(sh!=NULL)
            nodeObj->setShader(sh);
        nodeObj->draw();
    }
    for(auto c : _vector_spot_light){
        node* nodeObj=(node*)c;
        if(!nodeObj->visible())
            continue;
        if(sh!=NULL)
            nodeObj->setShader(sh);
        nodeObj->draw();
    }
    for(auto c : _vector_area_light){
        node* nodeObj=(node*)c;
        if(!nodeObj->visible())
            continue;
        if(sh!=NULL)
            nodeObj->setShader(sh);
        nodeObj->draw();
    }
}

void world::addPass(unsigned int fbo,shader* sh,std::function<void()> f){
    _vector_pass.push_back(std::make_tuple(fbo,sh,f));
}

void world::_draw(){
    if(_cb_before_any_gl_call!=nullptr)
        _cb_before_any_gl_call();
    _camera->update();

    if(_vector_pass.size()==0){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glEnable(GL_DEPTH_TEST);
        if(_cb_before_draw_call!=nullptr)
            _cb_before_draw_call();
        _drawAllChild(NULL);
    }else{
        for(auto p:_vector_pass){
            int fbo=get<0>(p);
            shader* sh=get<1>(p);
            std::function<void()> callback=get<2>(p);
            if(fbo!=0)
                glBindFramebuffer(GL_FRAMEBUFFER,fbo);
            else{
    #ifdef BUILD_MAC
            glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
    #elif BUILD_IOS
            [view bindDrawable];    //绑定GLKView创建的framebuffer
    #endif
            }
            if(callback!=NULL)
                callback();
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            _drawAllChild(sh);
        }
    }

    if(_cb_after_any_gl_call!=nullptr)
        _cb_after_any_gl_call();
}


void world::_renderOnce(){
    state::reset();
    _draw();
    if(state::isShowFrameRate()==true)
        state::displayFrameRate();
    if(state::isShowDrawCall())
        state::displayDrawCall();
    if(state::isShowVertices())
        state::displayVertices();
    if(state::isShowTime())
        state::displayTime();
}

void world::_main_loop(){
    unsigned int timeQuery;
    GLuint64 renderNS=0;
    glGenQueries(1,&timeQuery);
#ifdef BUILD_MAC
//    flyEngine::camera* cameraObj=new flyEngine::camera();
    flyEngine::world* worldObj=flyEngine::world::getInstance();
//    worldObj->setCamera(cameraObj);
//    cameraObj->enableControl();
    while(!glfwWindowShouldClose(g_window)){
//        if(glfwGetKey(g_window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
//            glfwSetWindowShouldClose(g_window,true);
        threadUtil::sleepMS(CONST_FRAME_RATE*1000);   //1000 means 1ms
        glBeginQuery(GL_TIME_ELAPSED,timeQuery);
        _renderOnce();
        glEndQuery(GL_TIME_ELAPSED);
        glGetQueryObjectui64v(timeQuery,GL_QUERY_RESULT,&renderNS);
        _totalRenderLoops++;
        _totalRenderNS+=renderNS;
        glfwSwapBuffers(g_window);
        glfwPollEvents();
   }
#elif BUILD_IOS
    renderOnce();
#endif
}

void world::start_rendering(){
//    threadUtil::createThread(world::_main_loop);
    _main_loop();
}

void world::printRenderTime(){
    double ns=(double)_totalRenderNS/(double)_totalRenderLoops;
    double ms=ns/double(1000000.0);
    flylog("total average render time ms %f",ms);
}

double world::getRenderTimeMS(){
    double ns=(double)_totalRenderNS/(double)_totalRenderLoops;
    double ms=ns/double(1000000.0);
    return ms;
}


//void world::drawPass(){
//    if(_cb_before_any_gl_call!=nullptr)
//        _cb_before_any_gl_call();
//    _camera->update();
//    for(auto p:_vector_pass){
//        if(p.first!=0)
//            glBindFramebuffer(GL_FRAMEBUFFER,p.first);
//        else{
//#ifdef BUILD_MAC
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
//#elif BUILD_IOS
//        [view bindDrawable];    //绑定GLKView创建的framebuffer
//#endif
//        }
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        loopAllObj(p.second);
//    }
//    if(_cb_after_any_gl_call!=nullptr)
//        _cb_after_any_gl_call();
//}

//void world::enableMultiPassMode(bool v){
//    _bMultiPassMode=v;
//};

//void world::renderToPass(unsigned int fbo,shader* sh){
//    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    _camera->update();
//    loopAllObj(sh);
//}
//
//void world::renderToDefault(shader* sh){
//#ifdef BUILD_MAC
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //绑定默认的framebuffer
//#elif BUILD_IOS
//        [view bindDrawable];    //绑定GLKView创建的framebuffer
//#endif
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    loopAllObj(sh);
//}
