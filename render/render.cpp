//
//  render.cpp
//  flyEngine
//
//  Created by joe on 01/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "render.h"
#include <thread>
#include "threadUtil.h"

static int s_frame_rate=60;

static render* s_instance;

render* render::getInstance(){
    if(s_instance)
        return s_instance;
    s_instance=new render();
    return s_instance;
}

void render::loopThread(){
    float s_renderSleep=1/s_frame_rate;
    while(true){
        threadUtil::sleep(s_renderSleep);
//        std::cout<<"render thread"<<std::endl;
    }
}

void render::start(){
    std::thread threadRender(std::bind(&render::loopThread,render::getInstance()));
    threadRender.detach();
}
