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
}

void world::addChild(node *node){
    _vector_child.push_back(node);
}

void world::addLight(node *node){
    _vector_light.push_back(node);
}

void world::start_rendering(){
//    threadUtil::createThread(world::_main_loop);
    world::_main_loop();
}

void world::setCamera(camera* c){
    _camera=c;
}

void world::setAmbientLight(ambientLight* amLight){
    m_amLight=amLight;
}


control* world::getControl(){
    if(_camera==nullptr)
        return nullptr;
    return _camera->getControl();
}

void world::draw(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    for(auto c : _vector_child){
        node* nodeObj=c;
        nodeObj->draw(_camera);
    }
    for(auto c : _vector_light){
       node* nodeObj=c;
       nodeObj->draw(_camera);
    }
}


void world::pause(){
    
}

void world::end(){
    
}


void world::_main_loop(){
//    flyEngine::camera* cameraObj=new flyEngine::camera();
    flyEngine::world* worldObj=flyEngine::world::getInstance();
//    worldObj->setCamera(cameraObj);
//    cameraObj->enableControl();
    
    while(!glfwWindowShouldClose(g_window)){
         threadUtil::sleepMS(CONST_FRAME_RATE*1000);   //1000 means 1ms

         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
         glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

         worldObj->draw();

         glfwSwapBuffers(g_window);
         glfwPollEvents();
   }

}
