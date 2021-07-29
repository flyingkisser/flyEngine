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
#include <vector>
#include <string>
#include <map>
//#include <condition_variable>
//#include <atomic>
//#include <mutex>

class timerMgr
{
public:
    static timerMgr* getInstance();
    
    timerMgr(const std::string sTimerName = "");   //构造定时器，附带名称
    ~timerMgr();
    
    /**
     开始运行定时器

     @param secTime 延迟运行(单位s)
     @param task 任务函数接口
     @param loopCount 执行次数，给0则无限循环
     @return int:timer id
     */
    int start(float secTime, std::function<void()> task, int loopCount);
    
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
    int execOnceDelay(float secTime, callable&& fun, arguments&&... args) {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(fun), std::forward<arguments>(args)...));
        return start(secTime, task, 1);
    }
    
    /**
     异步执行一次任务(默认延迟10毫秒后执行)
     
     @param fun 函数接口或lambda代码块
     @param args 参数
     @return true:已准备执行，否则失败
     */
    template<typename callable, typename... arguments>
    int execOnce(callable&& fun, arguments&&... args) {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(fun), std::forward<arguments>(args)...));
        return start(0, task, 1);
    }
    
    /**
     异步循环执行任务

     @param msTime 延迟及间隔时间（毫秒）
     @param fun 函数接口或lambda代码块
     @param args 参数
     @return true:已准备执行，否则失败
     */
    template<typename callable, typename... arguments>
    int exec(float secTime, callable&& fun, arguments&&... args) {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(fun), std::forward<arguments>(args)...));
        return start(secTime, task, 0);
    }
    
    template<typename callable, typename... arguments>
       int execWithCount(float secTime, int count, callable&& fun, arguments&&... args) {
           std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(fun), std::forward<arguments>(args)...));
           if(count<=0) count=1;
           return start(secTime, task, count);
       }
    
    void stop(int i);


public:
    int m_nCount = 0;   //循环次数
    int m_nTag = 0;     //定时器标签
    
private:
    std::string m_sName;   //定时器名称
    std::atomic_bool m_bStoped;       //装载的任务是否已经过期
    //std::vector<std::thread*> m_vectorThread;
    std::map<int,std::thread*> m_mapThread;
  
    int m_intKey=1;
};

#endif /* timerMgr_h */
