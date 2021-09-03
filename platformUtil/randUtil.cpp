//
//  randUtil.cpp
//  lines
//
//  Created by joe on 19/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "randUtil.h"
#include "logUtil.h"
#include "timeUtil.h"

unsigned int randUtil::getRand(unsigned int begin,unsigned int end){
    if(begin>end)
        return 0;
    if(begin==end)
        return begin;
//    srand((unsigned int)time(0));
    unsigned int inner=end-begin;
    unsigned int r=rand();
    unsigned int ret=r%inner;
    return begin+ret+1;
}

float randUtil::getRand(float begin,float end){
    if(begin>end)
        return 0;
    if(begin==end)
        return begin;
//  srand((unsigned int)time(0));
//    srand((unsigned int)timeUtil::getTimeMS());
    float inner=end-begin;
    float r=rand();
//    flylog("rand:%f",r);
    r=r/double(RAND_MAX);    //value [0,1]
    return begin+r*inner;
}

void randUtil::getRandStr(unsigned int len,char* buf){
    char charArr[]="ABCDEFGHJKMNPQRSTWXYZabcdefhijkmnprstwxyz2345678";
    unsigned int charSize=sizeof(charArr);
    if(len<=0)
        len=32;
    for(int i=0;i<charSize;i++){
        unsigned int index=randUtil::getRand((unsigned int)0, (unsigned int)(charSize-1));
        buf[i]=charArr[index];
    }
}
