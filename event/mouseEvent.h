//
//  mouseEvent.h
//  flyEngine
//
//  Created by joe on 10/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef mouseEvent_h
#define mouseEvent_h
#include <stdio.h>
#include <vector>
#include "defines.h"

NS_FLYENGINE_BEGIN

class mouseEvent{
private:
    std::vector<std::function<void(void)>> _vectorOnLeftClick;
    std::vector<std::function<void(void)>> _vectorOnLeftClickRelease;
    std::vector<std::function<void(void)>> _vectorOnRightClick;
    std::vector<std::function<void(void)>> _vectorOnRightClickRelease;
    std::vector<std::function<void(void)>> _vectorOnMiddleClick;
    std::vector<std::function<void(void)>> _vectorOnMiddleClickRelease;
    
    std::vector<std::function<void(float,float)>> _vectorOnMove;
    
    std::vector<std::function<void(float,float)>> _vectorOnMoveWithLefHold;
    std::vector<std::function<void(float,float)>> _vectorOnMoveWithRightHold;
    std::vector<std::function<void(float,float)>> _vectorOnMoveWithMiddleHold;
    
    std::vector<std::function<void(float,float)>> _vectorOnScroll;
    
public:
    mouseEvent(){};
    ~mouseEvent(){
        _vectorOnLeftClick.clear();
        _vectorOnRightClick.clear();
        _vectorOnMiddleClick.clear();
        
        _vectorOnMove.clear();
        
        _vectorOnMoveWithLefHold.clear();
        _vectorOnMoveWithRightHold.clear();
        _vectorOnMoveWithMiddleHold.clear();
        
        _vectorOnScroll.clear();
        
    };
    void regOnLeftClick(std::function<void(void)> cb){
        _vectorOnLeftClick.push_back(cb);
    };
    void regOnLeftClickRelease(std::function<void(void)> cb){
        _vectorOnLeftClickRelease.push_back(cb);
    };
    void regOnRightClick(std::function<void(void)> cb){
         _vectorOnRightClick.push_back(cb);
    };
    void regOnRightClickRelease(std::function<void(void)> cb){
        _vectorOnRightClickRelease.push_back(cb);
    };
    void regOnMiddleClick(std::function<void(void)> cb){
         _vectorOnMiddleClick.push_back(cb);
    };
    void regOnMiddleClickRelease(std::function<void(void)> cb){
        _vectorOnMiddleClickRelease.push_back(cb);
    };
    void regOnMove(std::function<void(float,float)> cb){
         _vectorOnMove.push_back(cb);
    }
   void regOnScroll(std::function<void(float,float)> cb){
         _vectorOnScroll.push_back(cb);
    }
    void regOnMoveWithLeftHold(std::function<void(float,float)> cb){
        _vectorOnMoveWithLefHold.push_back(cb);
    }
    
    void regOnMoveWithRightHold(std::function<void(float,float)> cb){
        _vectorOnMoveWithRightHold.push_back(cb);
    }
    
    void regOnMoveWithMiddleHold(std::function<void(float,float)> cb){
         _vectorOnMoveWithMiddleHold.push_back(cb);
    }
    
    void onLeftClick(){
        for(std::function<void(void)> cb : _vectorOnLeftClick){
           cb();
        }
    }
    void onLeftClickRelease(){
        for(std::function<void(void)> cb : _vectorOnLeftClickRelease){
            cb();
        }
    }
    void onRightClick(){
         for(std::function<void(void)> cb : _vectorOnRightClick){
             cb();
         }
    }
    void onRightClickRelease(){
        for(std::function<void(void)> cb : _vectorOnRightClickRelease){
            cb();
        }
    }
    void onMiddleClick(){
         for(std::function<void(void)> cb : _vectorOnMiddleClick){
            cb();
        }
    }
    void onMiddleClickRelease(){
        for(std::function<void(void)> cb : _vectorOnMiddleClickRelease){
            cb();
        }
    }
    void onMove(float x,float y){
        for(std::function<void(float,float)> cb : _vectorOnMove){
           cb(x,y);
       }
    }
    void onMoveWithLeftHold(float x,float y){
        for(std::function<void(float,float)> cb : _vectorOnMoveWithLefHold){
           cb(x,y);
       }
    }
    void onMoveWithRightHold(float x,float y){
        for(std::function<void(float,float)> cb : _vectorOnMoveWithRightHold){
           cb(x,y);
       }
    }
    void onMoveWithMiddleHold(float x,float y){
        for(std::function<void(float,float)> cb : _vectorOnMoveWithMiddleHold){
           cb(x,y);
       }
    }
    void onScroll(float x,float y){
        for(std::function<void(float,float)> cb : _vectorOnScroll){
            cb(x,y);
        }
    }

};

NS_FLYENGINE_END
#endif /* mouseEvent_h */
