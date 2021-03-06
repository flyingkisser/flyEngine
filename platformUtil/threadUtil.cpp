//
//  threadUtil.cpp
//  flyEngine
//
//  Created by joe on 03/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "threadUtil.h"
#include <thread>
#include <chrono>
#include <math.h>

void sleep(float s){
    int ms=floor(s*1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

//线程在自己的线程空间立即执行，当前线程不阻塞
void runThread(std::function<void(void)> cb){
    std::thread newThread(cb);
    newThread.detach();
}
