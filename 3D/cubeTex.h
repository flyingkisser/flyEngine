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
class texture2;

class cubeTex: public node
{
private:
    unsigned int _gl_texture0=0;
    texture2* _texObj=NULL;
    const char* _texPath=NULL;
    
    std::function <void(int programID)> m_cb_before_draw_call=nullptr;

public:
    cubeTex(const char* texPath);
    cubeTex(unsigned int texID);
    ~cubeTex(){};

    bool init();
    void glInit();
    
    cubeTex* clone();
    
    bool initByVerticeArr(float* arr,int arrSize,int descArr[],int descArrSize);
    void resetPos();
    void draw();
    void setCBDrawCall(std::function<void(int programID)> f){m_cb_before_draw_call=f;};
};

NS_FLYENGINE_END

#endif /* cubeTex_h */
