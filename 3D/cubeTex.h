//
//  cubeTex.h
//  flyEngine
//
//  Created by wu mingzhou on 2021/8/1.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef cubeTex_h
#define cubeTex_h

#include <stdio.h>
#include <string>
#include <functional>

#include "defines.h"
#include "node.h"

using namespace std;

NS_FLYENGINE_BEGIN
class shader;

class cubeTex: public node
{
private:
    unsigned int _gl_texture0=0;
    texture* _texObj=NULL;
    const char* _texPath;
    
    std::function <void(int programID)> m_cb_before_draw_call=nullptr;

public:
    cubeTex(const char* texPath);
    ~cubeTex(){};
    
//    bool init(float* arr=g_verticeArrWithTexCoord,int arrSize=sizeof(g_verticeArrWithTexCoord),bool useNormal=false);
//    void glInit(float* arr=g_verticeArrWithTexCoord,int arrSize=sizeof(g_verticeArrWithTexCoord),bool useNormal=false);

    bool init();
    void glInit();
    
    cubeTex* clone();
    
    bool initByVerticeArr(float* arr=g_verticeArrWithTexCoord,int arrSize=sizeof(g_verticeArrWithTexCoord),bool useNormal=false);
    void glInitByVerticeArr(float* arr=g_verticeArrWithTexCoord,int arrSize=sizeof(g_verticeArrWithTexCoord),bool useNormal=false);

    void resetPos();
    void draw(camera* cameraObj);
    void setCBDrawCall(std::function<void(int programID)> f){m_cb_before_draw_call=f;};
};

NS_FLYENGINE_END

#endif /* cubeTex_h */
