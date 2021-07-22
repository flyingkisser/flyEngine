//
//  mouseEventMgr.cpp
//  flyEngine
//
//  Created by joe on 10/11/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "mouseEventMgr.h"

USE_NS_FLYENGINE;

static map<std::string,mouseEvent*> s_eventChainMap;
static GLFWwindow* s_window;
static float s_width;
static float s_high;

void mouseEventMgr::init(GLFWwindow* window){
    s_window=window;
    int w,h;
    glfwGetWindowSize(window, &w, &h);
    s_width=(float)w;
    s_high=(float)h;
    
    //点击回调
    glfwSetMouseButtonCallback(window,[](GLFWwindow* window, int button, int action, int mods){
        if(action!=GLFW_PRESS)
            return;
        if(button==GLFW_MOUSE_BUTTON_LEFT){
            for(auto it : s_eventChainMap){
              it.second->onLeftClick();
            }
        }else if(button==GLFW_MOUSE_BUTTON_RIGHT){
            for(auto it : s_eventChainMap){
                it.second->onRightClick();
            }
        }
        else if(button==GLFW_MOUSE_BUTTON_MIDDLE){
            for(auto it : s_eventChainMap){
                it.second->onMiddleClick();
            }
        }
    });

    //移动回调
    glfwSetCursorPosCallback(window,[](GLFWwindow* window, double xpos, double ypos){
        for(auto it : s_eventChainMap){
            it.second->onMove((float)xpos,s_high-(float)ypos);
        }
        if(glfwGetMouseButton(s_window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS){
            for(auto it : s_eventChainMap){
                 it.second->onMoveWithLeftHold((float)xpos,s_high-(float)ypos);
            }
        }
        if(glfwGetMouseButton(s_window, GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS){
            for(auto it : s_eventChainMap){
                 it.second->onMoveWithRightHold((float)xpos,s_high-(float)ypos);
            }
        }
        if(glfwGetMouseButton(s_window, GLFW_MOUSE_BUTTON_MIDDLE)==GLFW_PRESS){
            for(auto it : s_eventChainMap){
                 it.second->onMoveWithMiddleHold((float)xpos,s_high-(float)ypos);
            }
        }
        
    });

    //滚轮回调
    glfwSetScrollCallback(window,[](GLFWwindow* window, double xoffset, double yoffset){
         for(auto it : s_eventChainMap){
               it.second->onScroll((float)xoffset,(float)yoffset);
          }
    });
};

void mouseEventMgr::addEvent(std::string name,mouseEvent* eventObj){
    s_eventChainMap.insert(pair<string,mouseEvent*>(name,eventObj));
    cout<<"s_eventChainMap size is "<<s_eventChainMap.size()<<endl;
};

void mouseEventMgr::printInfo(){
    cout<<"printInfo s_eventChainMap size is "<<s_eventChainMap.size()<<endl;
};
