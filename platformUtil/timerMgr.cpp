//
//  timerUtil.cpp
//  flyEngine
//
//  Created by joe on 19/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "timerMgr.h"

#include <future>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

static timerMgr* s_instance;

timerMgr* timerMgr::getInstance(){
    if(s_instance!=NULL)
        return s_instance;
    s_instance=new timerMgr("default_timer_mgr");
    return s_instance;
}


timerMgr::timerMgr(const std::string sTimerName):m_bStoped(false){
    m_sName = sTimerName;
}

timerMgr::~timerMgr(){
    clear();
}

//loopCount 0 表示无限循环
//loopCount 1 只执行一次
//loopCount n 只执行n次
int timerMgr::start(float secTime, std::function<void()> task, int loopCount){
    if(m_bStoped)
        return 0;
    unsigned int msTime=(int)(secTime*1000);
    int currentKey=m_intKey;
    std::thread* workThread = new std::thread([this, msTime, task, loopCount,currentKey]() {
        if (!m_sName.empty()) {
        #if (defined(__ANDROID__) || defined(ANDROID))      //兼容Android
            pthread_setname_np(pthread_self(), m_sName.c_str());
        #elif defined(__APPLE__)                            //兼容苹果系统
            pthread_setname_np(m_sName.c_str());    //设置线程(定时器)名称
        #endif
        }
        int count=0;
        while(!m_bStoped) {
            std::this_thread::sleep_for(std::chrono::milliseconds(msTime));
            if(m_bStoped)
                return;
            if(!m_mapThreadState[currentKey])
                return;
            task();     //执行任务
            count++;    //执行次数
            if(loopCount && count>=loopCount){
                //已经到达执行次数
                return;
            }
        }
    });
    m_mapThread[m_intKey]=workThread;
    m_mapThreadState[m_intKey]=true;
    m_intKey++;
    return m_intKey-1;
}


void timerMgr::clear(){
    m_bStoped = true;
    for(auto obj:m_mapThread){
        std::thread* threadObj=(std::thread*)obj.second;
        threadObj->join();
        delete threadObj;
    }
    m_mapThread.clear();
    m_intKey=1;
}

void timerMgr::stop(int key){
    std::thread* threadObj=m_mapThread[key];
    if(threadObj==NULL)
        return;
    m_mapThreadState[key]=false;
    threadObj->join();
    delete threadObj;
    m_mapThread.erase(key);
//    m_mapThreadState.erase(key);
}
