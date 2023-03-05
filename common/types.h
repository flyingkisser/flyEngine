
//
//  types.h
//  flyEngine
//
//  Created by joe on 30/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#ifndef types_h
#define types_h

#ifdef BUILD_IOS
#define GLFWwindow int
#endif

NS_FLYENGINE_BEGIN

struct size{
    float width;
    float height;
};


/*
class vec3{
public:
    GLfloat _row[3];
    vec3(){
        _row[0]=0;
        _row[1]=0;
        _row[2]=0;
    }
    vec3(float v1,float v2,float v3){
        _row[0]=v1;
        _row[1]=v2;
        _row[2]=v3;
    }
};

class vec4{
public:
    GLfloat _row[4];
    vec4(){
        _row[0]=0;
        _row[1]=0;
        _row[2]=0;
        _row[3]=0;
    }
    vec4(float v1,float v2,float v3,float v4){
        _row[0]=v1;
        _row[1]=v2;
        _row[2]=v3;
        _row[3]=v4;
    }
};


class mat3{
public:
    GLfloat _mat[3][3];
    mat3(){
        _mat[0][0]=0;
        _mat[0][1]=0;
        _mat[0][2]=0;
        _mat[1][0]=0;
        _mat[1][1]=0;
        _mat[1][2]=0;
        _mat[2][0]=0;
        _mat[2][1]=0;
        _mat[2][2]=0;
    }
    mat3(float v1,float v2,float v3,float v4,float v5,float v6,float v7,float v8,float v9){
        _mat[0][0]=v1;
        _mat[0][1]=v2;
        _mat[0][2]=v3;
        _mat[1][0]=v4;
        _mat[1][1]=v5;
        _mat[1][2]=v6;
        _mat[2][0]=v7;
        _mat[2][1]=v8;
        _mat[2][2]=v9;
    }
};

class mat4{
public:
    GLfloat _mat[4][4];
    mat4(){
        _mat[0][0]=0;
        _mat[0][1]=0;
        _mat[0][2]=0;
        _mat[0][3]=0;
        _mat[1][0]=0;
        _mat[1][1]=0;
        _mat[1][2]=0;
        _mat[1][3]=0;
        _mat[2][0]=0;
        _mat[2][1]=0;
        _mat[2][2]=0;
        _mat[2][3]=0;
        _mat[3][0]=0;
        _mat[3][1]=0;
        _mat[3][2]=0;
        _mat[3][3]=0;
    }
    mat4(float v1,float v2,float v3,float v4,float v5,float v6,float v7,float v8,float v9,float v10,float v11,float v12,
         float v13,float v14,float v15,float v16){
        _mat[0][0]=v1;
        _mat[0][1]=v2;
        _mat[0][2]=v3;
        _mat[0][3]=v4;
        _mat[1][0]=v5;
        _mat[1][1]=v6;
        _mat[1][2]=v7;
        _mat[1][3]=v8;
        _mat[2][0]=v9;
        _mat[2][1]=v10;
        _mat[2][2]=v11;
        _mat[2][3]=v12;
        _mat[3][0]=v13;
        _mat[3][1]=v14;
        _mat[3][2]=v15;
        _mat[3][3]=v16;
    }
};

class p2{
public:
    GLfloat x,y;
    vec3* v3;
    p2(){
        x=0;
        y=0;
    }
    p2(float v1,float v2){
        x=v1;
        y=v2;
    }
    vec3* getVec3(){
      return new vec3(x,y,1);
    }
};

class p3{
public:
    GLfloat x,y,z;
    p3(){
        x=0;
        y=0;
        z=0;
    }
    p3(float v1,float v2,float v3){
        x=v1;
        y=v2;
        z=v3;
    }
    vec4* getVec4(){
         return new vec4(x,y,z,1);
    }
};

class p4{
public:
    GLfloat x,y,z,w;
    p4(){
        x=0;
        y=0;
        z=0;
        w=0;
    }
    p4(float v1,float v2,float v3,float v4){
        x=v1;
        y=v2;
        z=v3;
        w=v4;
    }
};

struct structColor{
    int r;
    int g;
    int b;
    int a;
};

struct structPos2{
    float x;
    float y;
};


struct structPos3{
    float x;
    float y;
    float z;
};

struct structPos4{
    float x;
    float y;
    float z;
    float w;
};

typedef float Matrix4x4 [4][4];
*/

NS_FLYENGINE_END

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif


#endif  /* types_h */
