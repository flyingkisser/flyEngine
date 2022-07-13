//
//  uiText.cpp
//  flyEngine
//
//  Created by Joe on 2022/7/12.
//  Copyright © 2022 joe. All rights reserved.
//

#include "uiText.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "logUtil.h"
#include "shader.h"
#include "shaderMgr.h"
#include "window.h"
#include "fontMgr.h"

using namespace flyEngine;
using namespace std;

uiText::uiText(const char* strFont,int fontSize,const char* strText){
    _fontSize=fontSize;
    _strText=(char*)strText;
    
    _shader2dObj=shaderMgr::getShader("res/shader/font_2d.vs", "res/shader/font.fs");
    if(_shader2dObj->getProgramID()==0){
        flylog("uiText: get shader failed!");
        return;
    }
    
    fontTTF* _fontObj=fontMgr::getInstance()->getFontTTF(strFont);
    if (_fontObj==NULL){
        flylog("uiText: get font obj failed!");
        return;
    }
    _fontObj->setFontSize(fontSize);
}

bool uiText::init(){
    if (_fontObj==NULL){
        flylog("uiText::init _fontObj is null,failed!");
        return false;
    }
    glInit();
    return true;
}

void uiText::glInit(){
    glGenVertexArrays(1,&_vao);
    glBindVertexArray(_vao);
    glGenBuffers(1,&_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,_vbo);
    glBufferData(GL_ARRAY_BUFFER,4*6*sizeof(float),NULL,GL_DYNAMIC_DRAW);//矩形有2个三角形即6个顶点，每个顶点有2个世界坐标和2个纹理坐标
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void uiText::draw(camera* cameraObj){
    int strLen=(int)strlen(_strText);    
    _shader2dObj->use();
    glm::mat4 proj=glm::ortho(0,g_winWidth,0,g_winHigh);
    _shader2dObj->setMat4("matProj", (float*)glm::value_ptr(proj));
    _shader2dObj->setMat4("matModel", (float*)glm::value_ptr(getPosition()));
    _shader2dObj->setInt("texture1", 0);
    glm::vec3 color=glm::vec3(_fontColor.r/255,_fontColor.g/255,_fontColor.b/255);
    _shader2dObj->setVec3("textColor", (float*)glm::value_ptr(color));
    
    glBindVertexArray(_vao);
    glm::vec3 nodePos=getPosition();
    glm::vec3 scale=getScale();
    glm::vec3 pos=nodePos;
   
    for(int i=0;i<strLen;i++){
        int k=(int)_strText[i];
        texFontStruct st=_fontObj->getTexStruct(k);
        float x=pos.x+st.bearingX*scale.x;
        float y=pos.y-(st.height-st.bearingY)*scale.y;
        float w=st.width*scale.x;
        float h=st.height*scale.y;
        float vertices[6][4]={
            {x,y+h,0,0},
            {x,y,0,1},
            {x+w,y,1,1},
            {x,y+h,0,0},
            {x+w,y,1,1},
            {x+w,y+h,1,0}
        };
        glBindTexture(GL_TEXTURE_2D,st.texID);
        glBindBuffer(GL_ARRAY_BUFFER,_vbo);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
        
        glDrawArrays(GL_TRIANGLES,0,6);
        pos.x+=(st.advance/6)*scale.x;
    }
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindTexture(GL_TEXTURE_2D,0);
}


