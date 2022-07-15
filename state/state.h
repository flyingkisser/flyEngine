//
//  state.h
//  flyEngine
//
//  Created by Joe on 2022/7/14.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef state_h
#define state_h

#include <stdio.h>
#include "defines.h"

NS_FLYENGINE_BEGIN
class uiText;

static long long s_lasttime=0;
static int s_draw_call=0;
static int s_vertices=0;


static bool s_bShowFrameRate=true;
static bool s_bShowDrawCall=true;
static bool s_bShowVertices=true;


static uiText* s_textRate=NULL;
static uiText* s_textDrawCall=NULL;
static uiText* s_textVertices=NULL;

static std::string s_strRate;
static std::string s_strDrawCall;
static std::string s_strVertices;

class state{
    
public:
    
    static void setShowFrameRate(bool s);
    static bool isShowFrameRate();
    static void displayFrameRate();
    
    static void setShowDrawCall(bool s);
    static bool isShowDrawCall();
    static void displayDrawCall();
    
    
    static void setShowVertices(bool s);
    static bool isShowVertices();
    static void displayVertices();
    
    static void reset();
    
    static void incDrawCall();
    static void addVertices(int v);
    static void log(int v);
    
};

NS_FLYENGINE_END
#endif /* state_h */
