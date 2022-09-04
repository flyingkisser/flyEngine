//
//  fingerEvent.h
//  flyEngine_ios
//
//  Created by joe on 19/08/2022.
//

#ifndef fingerEvent_h
#define fingerEvent_h

#include <stdio.h>
#include <vector>
#include <functional>

#include "defines.h"

NS_FLYENGINE_BEGIN

class fingerEvent{
private:
    std::vector<std::function<void(float,float)>> _vecCBOnMove;
    std::vector<std::function<void(float)>> _vecCBOnZoom;
    std::vector<std::function<void(float)>> _vecCBOnRotate;
    std::vector<std::function<void(int)>> _vecCBOnTap;
 
public:
    static fingerEvent* getInstance();
    fingerEvent(){};
    ~fingerEvent(){};
    
    void regOnMove(std::function<void(float move_x,float move_y)> cb);
    void regOnZoom(std::function<void(float scale)> cb);
    void regOnRotate(std::function<void(float r)> cb);
    void regOnTap(std::function<void(int count)> cb);
    void onMove(float move_x,float move_y);
    void onZoom(float move);
    void onRotate(float r);
    void onTap(int c);
  
};

NS_FLYENGINE_END

#endif /* fingerEvent_h */
