//
//  mathUtil.cpp
//  flyEngine
//
//  Created by joe on 21/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "mathUtil.h"

int mathUtil::ceil(float v){
    int high=(int)v;
    float left=v-high;
    if(left>0)
        return ++high;
    else if(left==0)
        return high;
    else
        return high;
}

int mathUtil::floor(float v){
    int high=(int)v;
    float left=v-high;
    if(left>0)
       return high;
    else if(left==0)
       return high;
    else
       return --high;
}

int mathUtil::round(float v){
    int high=(int)v;
    float left=v-high;
    if(v>0){
        if(left>=0.5)
           return ++high;
        else
           return high;
    }else if(v==0)
        return high;
    else{
        if(left<=-0.5)
            return --high;
        else
            return high;
    }
  
}

float mathUtil::max(float v1,float v2){
    return v1>=v2?v1:v2;
}

int mathUtil::max(int v1,int v2){
    return v1>=v2?v1:v2;
}

float mathUtil::min(float v1,float v2){
    return v1<=v2?v1:v2;
}

int mathUtil::min(int v1,int v2){
     return v1<=v2?v1:v2;
}

int mathUtil::abs(int v){
    return v>=0?v:-v;
}

float mathUtil::abs(float v){
    return v>=0?v:-v;
}

void mathUtil::test(){
//    int v=0;
//    float vf=0;
//    v=floor(1.3);
//    v=floor(-1.3);
//    v=floor(1.7);
//    v=floor(-1.7);
//    v=ceil(1.3);
//    v=ceil(-1.3);
//    v=ceil(1.7);
//    v=ceil(-1.7);
//    v=round(1.3);
//    v=round(-1.3);
//    v=round(1.7);
//    v=round(-1.7);
//    vf=abs(1.7f);
//    vf=abs(-1.7f);
//    v=abs(1);
//    v=abs(-1);
//    vf=min(1.7f,-1.7f);
//    vf=max(1.7f,-1.7f);
//    v=min(2,0);
//    v=max(3,10);
}
