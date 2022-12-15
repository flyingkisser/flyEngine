//
//  mathUtil.cpp
//  flyEngine
//
//  Created by joe on 21/07/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "mathUtil.h"
#include <cmath>

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

float mathUtil::sqrt(float v){
    if(v<=0)
        return 0;
    return std::sqrt(v);
}

float mathUtil::pow(float v,float s){
    return std::pow(v,s);
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


void mathUtil::extendPosRange(float* arr,int rows,int columns,float v){
    if(v==0)
        return;
    float s=v/arr[0];
    if(s<0)
        s=-s;
    for(int i=0;i<rows;i++){
        for(int j=0;j<3;j++){
            arr[i*columns+j]*=s;
        }
    }
}


//y1起点，y2终点，weight是[0,1]区间内的实数
//在y1和y2之间取一个值 ，取多少，由权重控制
//当权重为0.5时，正好取到中间
float mathUtil::lerp(float y1,float y2,float weight){
    return y1+weight*(y2-y1);
}
