//
//  timerMgr.h
//  flyEngine
//
//  Created by joe on 19/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef timerMgr_h
#define timerMgr_h

#include <stdio.h>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <string>
#include <condition_variable>

class timerMgr
{
public:
    timerMgr(const std::string sTimerName = "");   //构造定时器，附带名称
    ~timerMgr();
    
    /**
     开始运行定时器

     @param msTime 延迟运行(单位ms)
     @param task 任务函数接口
     @param bLoop 是否循环(默认执行1次)
     @param async 是否异步(默认异步)
     @return true:已准备执行，否则失败
     */
    bool start(unsigned int msTime, std::function<void()> task, bool bLoop = false);
    
    /**
     取消定时器，同步定时器无法取消(若任务代码已执行则取消无效)
     */
    void clear();
    
    /**
     异步执行一次任务
     
     @param msTime 延迟及间隔时间（毫秒）
     @param fun 函数接口或lambda代码块
     @param args 参数
     @return true:已准备执行，否则失败
     */
    template<typename callable, typename... arguments>
    bool execOnceDelay(int msTime, callable&& fun, arguments&&... args) {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(fun), std::forward<arguments>(args)...));
        return start(msTime, task, false);
    }
    
    /**
     异步执行一次任务(默认延迟10毫秒后执行)
     
     @param fun 函数接口或lambda代码块
     @param args 参数
     @return true:已准备执行，否则失败
     */
    template<typename callable, typename... arguments>
    bool execOnce(callable&& fun, arguments&&... args) {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(fun), std::forward<arguments>(args)...));
        return start(0, task, false);
    }
    
    
    /**
     异步循环执行任务

     @param msTime 延迟及间隔时间（毫秒）
     @param fun 函数接口或lambda代码块
     @param args 参数
     @return true:已准备执行，否则失败
     */
    template<typename callable, typename... arguments>
    bool exec(int msTime, callable&& fun, arguments&&... args) {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(fun), std::forward<arguments>(args)...));
        return start(msTime, task, true);
    }
    
public:
    /// 获取时间戳(毫秒)
    static uint64_t Timestamp();

    /// 获取格式化时间
    static std::string FormatTime(const std::string sFormat = "%Y-%m-%d %H:%M:%S");

    /// 获取UTC时间
    static struct tm *UTCTime(long long secTime = 0);

    /// 获取UTC时间(秒)
    static int64_t UTCTime();

    /// 获取与0时区的时差（以秒为单位）
    static int TimeDifFrimGMT();
    
private:
    void DeleteThread();    //删除任务线程

public:
    int m_nCount = 0;   //循环次数
    int m_nTag = 0;     //定时器标签
    
private:
    std::string m_sName;   //定时器名称
    
    std::atomic_bool m_bStoped;       //装载的任务是否已经过期
    std::thread *m_arrThread[128] = {NULL};
    int m_intArrIndex=0;
    
//    std::atomic_bool m_bTryExpired;    //装备让已装载的任务过期(标记)
//    std::atomic_bool m_bLoop;          //是否循环
    
//    std::thread *m_Thread = nullptr;
   
//    std::mutex m_ThreadLock;
//    std::condition_variable_any m_ThreadCon;
};

#endif /* timerMgr_h */
