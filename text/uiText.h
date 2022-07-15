//
//  uiText.h
//  flyEngine
//
//  Created by Joe on 2022/7/12.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef uiText_h
#define uiText_h

#include <stdio.h>
#include "defines.h"
#include "node.h"
#include "shader.h"
#include "fontTTF.h"

NS_FLYENGINE_BEGIN

class uiText:public node{
public:
    uiText(const char* strFontName,int fontSize,const char* strText);
    void setText(const char* str){_strText=(char*)str;};
    void setFontColor(glm::vec3 color){_fontColor=color;};
    void setFontSize(int s);
    void draw(camera* cameraObj);
    bool init();
    void glInit();
    
    const char* getText(){return _strText;};
    glm::vec3 getFontColor(){return _fontColor;};
    int getFontSize(){return _fontSize;};
    
private:
    char* _strText;
    const char* _fontName;
    int _fontSize=12;
    glm::vec3 _fontColor=glm::vec3(255,0,0);
    shader* _shader2dObj;
    fontTTF* _fontObj;
    GLuint _vao;
    GLuint _vbo;
};

NS_FLYENGINE_END

#endif /* uiText_h */
