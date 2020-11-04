//
//  2dUtil.cpp
//  lines
//
//  Created by joe on 30/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "2dTrans.h"
#include "defines.h"



//void move2dBy(p2* p1,float x,float y){
//    p1->x+=x;
//    p1->y+=y;
//}
//
//void move2dTo(p2* p1,float x,float y){
//    p1->x=x;
//    p1->y=y;
//}
//
//
//void scale2d(p2* p1,float sx,float sy){
//
//}
//
//void rotate2d(p2* p1,float theta){
//
//}


void _2dTrans::move(float x,float y){
    glTranslatef(x, y, 0);
}

void _2dTrans::rotate(float theta){
    //rotate as z axiel
    //theta,vx,vy,vz
    glRotatef(theta,0,0,1);
}

void _2dTrans::scale(float sx,float sy){
    glScalef(sx,sy,1);
}

void _2dTrans::reset(){
    glLoadIdentity();
}
