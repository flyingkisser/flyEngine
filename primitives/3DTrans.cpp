////
////  3DTrans.cpp
////  flyEngine
////
////  Created by joe on 13/07/2020.
////  Copyright © 2020 joe. All rights reserved.
////
//
//#include "3DTrans.h"
//using namespace flyEngine;
//
//void _3dTrans::_setMoveMatrix(Matrix4x4 mat,float x,float y,float z){
//    matrix::setIdentity(mat);
//    mat[0][3]=x;
//    mat[1][3]=y;
//    mat[2][3]=z;
//}
//
//void _3dTrans::_setScaleMatrix(Matrix4x4 mat,float sx,float sy,float sz){
//    matrix::setIdentity(mat);
//    mat[0][0]=sx;
//    mat[1][1]=sy;
//    mat[2][2]=sz;
//}
//
//void _3dTrans::_setReflectMatrix(Matrix4x4 mat, bool x, bool y, bool z){
//    matrix::setIdentity(mat);
//    if(x)
//        mat[0][0]=-1;
//    if(y)
//        mat[1][1]=-1;
//    if(z)
//        mat[2][2]=-1;
//}
//
////theta是度数，取值0到360
//void _3dTrans::_setRotateMatrix(Matrix4x4 mat,structPos3* p1,structPos3* p2,float theta){
//    matrix::setIdentity(mat);
//    
//    float axisLen=sqrt(pow(p2->x-p1->x,2)+pow(p2->y-p1->y,2)+pow(p2->z-p1->z,2));
//    float ux=(p2->x-p1->x)/axisLen;
//    float uy=(p2->y-p1->y)/axisLen;
//    float uz=(p2->z-p1->z)/axisLen;
//    float t=(theta/360.0)*2.0*M_PI;
//    float cosTheta=cos(t);
//    float sinTheta=sin(t);
//    float oneC=1-cosTheta;
//
//    mat[0][0]=ux*ux*oneC+cosTheta;
//    mat[0][1]=ux*uy*oneC-uz*sinTheta;
//    mat[0][2]=ux*uz*oneC+uy*sinTheta;
//    
//    mat[1][0]=uy*ux*oneC+uz*sinTheta;
//    mat[1][1]=uy*uy*oneC+cosTheta;
//    mat[1][2]=uy*uz*oneC-ux*sinTheta;
//    
//    mat[2][0]=uz*ux*oneC-uy*sinTheta;
//    mat[2][1]=uz*uy*oneC+ux*sinTheta;
//    mat[2][2]=uz*uz*oneC+cosTheta;
//}
//
////void _3dTrans::_rotatess(structPos3* pos,structPos3* p1,structPos3* p2,float theta){
////    Matrix4x4 matMove1;
////    Matrix4x4 matMove2;
////    Matrix4x4 matRotate;
////    Matrix4x4 matRoot;
////    structPos4 pos4;
////
////    _setMoveMatrix(matMove1, -pos->x, -pos->y, -pos->z);
////    _setMoveMatrix(matMove2, pos->x, pos->y, pos->z);
////    _setRotateMatrix(matRotate, p1, p2, theta);
////    pos4.x=pos->x;
////    pos4.y=pos->y;
////    pos4.z=pos->z;
////    pos4.w=1;
////
////    matrix::setIdentity(matRoot);
////    matrix::preMultiply(matMove1, matRoot);
////    matrix::preMultiply(matRotate, matRoot);
////    matrix::preMultiply(matMove2, matRoot);
////    matrix::preMultiplyWithVector(matRoot,&pos4);
////    pos->x=pos4.x;
////    pos->y=pos4.y;
////    pos->z=pos4.z;
////}
//
//void _3dTrans::_rotate(structPos3* pos,structPos3* p1,structPos3* p2,float theta){
//    Matrix4x4 matMove1;
//    Matrix4x4 matMove2;
//    Matrix4x4 matRotate;
//    Matrix4x4 matRoot;
//    structPos4 pos4;
//   
//    _setMoveMatrix(matMove1, -p1->x, -p1->y, -p1->z);
//    _setMoveMatrix(matMove2, p1->x, p1->y, p1->z);
//    _setRotateMatrix(matRotate, p1, p2, theta);
//    pos4.x=pos->x;
//    pos4.y=pos->y;
//    pos4.z=pos->z;
//    pos4.w=1;
//
//    matrix::setIdentity(matRoot);
//    matrix::preMultiply(matMove1, matRoot);
//    matrix::preMultiply(matRotate, matRoot);
//    matrix::preMultiply(matMove2, matRoot);
//    matrix::preMultiplyWithVector(matRoot,&pos4);
//    pos->x=pos4.x;
//    pos->y=pos4.y;
//    pos->z=pos4.z;
//}
//
//void _3dTrans::rotate(structPos3* p,structPos3* axis,float theta){
//    structPos3 p1={0,0,0};
//    _3dTrans::_rotate(p,&p1,axis,theta);
//}
//
//void _3dTrans::scale(structPos3* pos,float sx,float sy,float sz){
//    Matrix4x4 matMove1;
//    Matrix4x4 matMove2;
//    Matrix4x4 matScale;
//    Matrix4x4 matRoot;
//    structPos4 pos4;
//   
//    _setMoveMatrix(matMove1, -pos->x, -pos->y, -pos->z);
//    _setMoveMatrix(matMove2, pos->x, pos->y, pos->z);
//    _setScaleMatrix(matScale,sx,sy,sz);
//    pos4.x=pos->x;
//    pos4.y=pos->y;
//    pos4.z=pos->z;
//    pos4.w=0;
//
//    matrix::setIdentity(matRoot);
//    matrix::preMultiply(matMove1, matRoot);
//    matrix::preMultiply(matScale, matRoot);
//    matrix::preMultiply(matMove2, matRoot);
//    matrix::preMultiplyWithVector(matRoot,&pos4);
//    pos->x=pos4.x;
//    pos->y=pos4.y;
//    pos->z=pos4.z;
//}
