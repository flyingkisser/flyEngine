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

class mouseEvent{
private:
    std::vector<std::function<void(void)>> _vectorOnLeftClick;
    std::vector<std::function<void(void)>> _vectorOnRightClick;
    std::vector<std::function<void(void)>> _vectorOnMiddleClick;
    
    std::vector<std::function<void(float,float)>> _vectorOnMove;
    
    std::vector<std::function<void(float,float)>> _vectorOnMoveWithLefClick;
    std::vector<std::function<void(float,float)>> _vectorOnMoveWithRightClick;
    std::vector<std::function<void(float,float)>> _vectorOnMoveWithMiddleClick;
    
    std::vector<std::function<void(float,float)>> _vectorOnScroll;
    
public:
    mouseEvent(){};
    ~mouseEvent(){
        _vectorOnLeftClick.clear();
        _vectorOnRightClick.clear();
        _vectorOnMiddleClick.clear();
        
        _vectorOnMove.clear();
        
        _vectorOnMoveWithLefClick.clear();
        _vectorOnMoveWithRightClick.clear();
        _vectorOnMoveWithMiddleClick.clear();
        
        _vectorOnScroll.clear();
        
    };
    void regOnLeftClick(std::function<void(void)> cb){
        _vectorOnLeftClick.push_back(cb);
    };
    void regOnRightClick(std::function<void(void)> cb){
         _vectorOnRightClick.push_back(cb);
    };
    void regOnMiddleClick(std::function<void(void)> cb){
         _vectorOnMiddleClick.push_back(cb);
    };
    void regOnMove(std::function<void(float,float)> cb){
         _vectorOnMove.push_back(cb);
    }
   void regOnScroll(std::function<void(float,float)> cb){
         _vectorOnScroll.push_back(cb);
    }
    void regOnMoveWithLeftClick(std::function<void(float,float)> cb){
        _vectorOnMoveWithLefClick.push_back(cb);
    }
    
    void regOnMoveWithRightClick(std::function<void(float,float)> cb){
        _vectorOnMoveWithRightClick.push_back(cb);
    }
    
    void regOnMoveWithMiddleClick(std::function<void(float,float)> cb){
         _vectorOnMoveWithMiddleClick.push_back(cb);
    }
    
    void onLeftClick(){
        for(std::function<void(void)> cb : _vectorOnLeftClick){
           cb();
        }
    }
    void onRightClick(){
         for(std::function<void(void)> cb : _vectorOnRightClick){
             cb();
         }
    }
    void onMiddleClick(){
         for(std::function<void(void)> cb : _vectorOnMiddleClick){
            cb();
        }
    }
    void onMove(float x,float y){
        for(std::function<void(float,float)> cb : _vectorOnMove){
           cb(x,y);
       }
    }
    void onMoveWithLeftClick(float x,float y){
        for(std::function<void(float,float)> cb : _vectorOnMoveWithLefClick){
           cb(x,y);
       }
    }
    void onMoveWithRightClick(float x,float y){
        for(std::function<void(float,float)> cb : _vectorOnMoveWithRightClick){
           cb(x,y);
       }
    }
    void onMoveWithMiddleClick(float x,float y){
        for(std::function<void(float,float)> cb : _vectorOnMoveWithMiddleClick){
           cb(x,y);
       }
    }
    void onScroll(float x,float y){
        for(std::function<void(float,float)> cb : _vectorOnScroll){
            cb(x,y);
        }
    }

};


#endif /* mouseEvent_h */
