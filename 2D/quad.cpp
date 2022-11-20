//
//  quad.cpp
//  flyEngine
//
//  Created by joe on 10/10/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "quad.h"
#include "logUtil.h"
#include "state.h"
#include "textureMgr.h"
#include "shaderMgr.h"
#include "camera.h"
#include "window.h"

USE_NS_FLYENGINE

quad::quad(const char* strFileName):sprite(strFileName){
}

quad::quad(unsigned int texID):sprite(texID){
}


void quad::glInit(){
   // glm::vec2 s=getScale();
    int descArr[]={3,2};
    initVAO(g_verticeArrWithTexCoord_quad, sizeof(g_verticeArrWithTexCoord_quad),descArr,2);
}

void quad::_reInitVertices(){
    glBindBuffer(GL_ARRAY_BUFFER,_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(g_verticeArrWithTexCoord_quad),g_verticeArrWithTexCoord_quad,GL_STATIC_DRAW);
}

void quad::draw(){
    drawByType(GL_TRIANGLE_STRIP,4);
}
