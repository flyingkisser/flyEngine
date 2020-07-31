//
//  matrix.cpp
//  flyEngine
//
//  Created by joe on 13/07/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "matrix.h"

void matrix::setIdentity(Matrix4x4 mat){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++)
            mat[i][j]=(i==j);
    }
}

void matrix::preMultiply(Matrix4x4 m1,Matrix4x4 m2){
    Matrix4x4 matTemp;
    for(int i=0;i<4;i++){
      for(int j=0;j<4;j++)
          matTemp[i][j]=m1[i][0]*m2[0][j]+
                        m1[i][1]*m2[1][j]+
                        m1[i][2]*m2[2][j]+
          m1[i][3]*m2[3][j];
    }
    for(int i=0;i<4;i++){
       for(int j=0;j<4;j++)
           m2[i][j]=matTemp[i][j];
    }
}

void matrix::preMultiplyWithVector(Matrix4x4 m1,structPos4* pos){
    structPos4 posTemp;
    posTemp.x=m1[0][0]*pos->x+m1[0][1]*pos->y+m1[0][2]*pos->z+m1[0][3]*pos->w;
    posTemp.y=m1[1][0]*pos->x+m1[1][1]*pos->y+m1[1][2]*pos->z+m1[1][3]*pos->w;
    posTemp.z=m1[2][0]*pos->x+m1[2][1]*pos->y+m1[2][2]*pos->z+m1[2][3]*pos->w;
    posTemp.w=m1[3][0]*pos->x+m1[3][1]*pos->y+m1[3][2]*pos->z+m1[3][3]*pos->w;
    pos->x=posTemp.x;
    pos->y=posTemp.y;
    pos->z=posTemp.z;
    pos->w=posTemp.w;
}
