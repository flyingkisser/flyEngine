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

timerMgr::timerMgr(const std::string sTimerName):m_bStoped(false){
    m_sName = sTimerName;
}

timerMgr::~timerMgr(){
    clear();
}

bool timerMgr::start(unsigned int msTime, std::function<void()> task, bool bLoop){
    if(m_bStoped)
        return false;  //任务未过期(即内部仍在存在或正在运行任务)
    bool isLoop = bLoop;
    m_arrThread[m_intArrIndex++] = new std::thread([this, msTime, task, isLoop]() {
        if (!m_sName.empty()) {
        #if (defined(__ANDROID__) || defined(ANDROID))      //兼容Android
            pthread_setname_np(pthread_self(), m_sName.c_str());
        #elif defined(__APPLE__)                            //兼容苹果系统
            pthread_setname_np(m_sName.c_str());    //设置线程(定时器)名称
        #endif
        }
        if(!isLoop){
            std::this_thread::sleep_for(std::chrono::milliseconds(msTime));
            if(m_bStoped)
                return;
            task();
            return;
        }
        while(!m_bStoped) {
            std::this_thread::sleep_for(std::chrono::milliseconds(msTime));
            if(m_bStoped)
                return;
            task();     //执行任务
        }
    });
    return true;
}

void timerMgr::clear(){
    m_bStoped = true;
    for(int i=0;i<m_intArrIndex;i++){
        std::thread* threadObj=m_arrThread[i];
        threadObj->join();
        delete threadObj;
        m_arrThread[i]=NULL;
    }
    m_intArrIndex=0;
}





//void timerUtil::DeleteThread()
//{
//    if (m_Thread) {
//        m_ThreadCon.notify_all();   //休眠唤醒
//        m_Thread->join();           //等待线程退出
//        delete m_Thread;
//        m_Thread = nullptr;
//    }
//}

//bool timerUtil::Start(unsigned int msTime, std::function<void()> task, bool bLoop, bool async)
//{
//    if (!m_bExpired || m_bTryExpired) return false;  //任务未过期(即内部仍在存在或正在运行任务)
//    m_bExpired = false;
//    m_bLoop = bLoop;
//    m_nCount = 0;
//
//    if (async) {
//        DeleteThread();
//        m_Thread = new std::thread([this, msTime, task]() {
//            if (!m_sName.empty()) {
//#if (defined(__ANDROID__) || defined(ANDROID))      //兼容Android
//                pthread_setname_np(pthread_self(), m_sName.c_str());
//#elif defined(__APPLE__)                            //兼容苹果系统
//                pthread_setname_np(m_sName.c_str());    //设置线程(定时器)名称
//#endif
//            }
//
//            while (!m_bTryExpired) {
//                m_ThreadCon.wait_for(m_ThreadLock, std::chrono::milliseconds(msTime));  //休眠
//                if (!m_bTryExpired) {
//                    task();     //执行任务
//
//                    m_nCount ++;
//                    if (!m_bLoop) {
//                        break;
//                    }
//                }
//            }
//
//            m_bExpired = true;      //任务执行完成(表示已有任务已过期)
//            m_bTryExpired = false;  //为了下次再次装载任务
//        });
//    } else {
//        std::this_thread::sleep_for(std::chrono::milliseconds(msTime));
//        if (!m_bTryExpired) {
//            task();
//        }
//        m_bExpired = true;
//        m_bTryExpired = false;
//    }
//
//    return true;
//}
