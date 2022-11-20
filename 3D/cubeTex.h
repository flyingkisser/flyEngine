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
class texture2;

class cubeTex: public node
{
private:
    texture2* _texObj=NULL;
    const char* _texPath=NULL;
    
    

public:
    unsigned int _gl_texture0=0;
    
    cubeTex(const char* texPath);
    cubeTex(unsigned int texID);
    ~cubeTex(){};

    bool init();

    
    cubeTex* clone();
    
    bool initByVerticeArr(float* arr,int arrSize,int descArr[],int descArrSize);
    void resetPos();
    void draw();
    void setPipelineValue();
    void drawCall();
    
};

NS_FLYENGINE_END

#endif /* cubeTex_h */
