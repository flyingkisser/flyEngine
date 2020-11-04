//
//  quad.cpp
//  lines
//
//  Created by Joe on 2017/7/12.
//  Copyright © 2017年 joe. All rights reserved.
//

#include "quad.h"
#include "defines.h"
#include "error.h"
#include "flyEngine.h"

void drawQuadImm(){
    GLint p1[]={1,1};
    GLint p2[]={200,1};
    GLint p3[]={200,200};
    GLint p4[]={180,240};
    GLint p5[]={100,240};
    GLint p6[]={50,230};
    GLint p7[]={50,180};
    GLint p8[]={80,180};
    glColor3f (1, 0, 0);
    
    /*
    //每四个点画出一个四边形
    glBegin(GL_QUADS);
    glVertex2iv(p1);
    glVertex2iv(p2);
    glVertex2iv(p3);
    glVertex2iv(p4);
    glVertex2iv(p5);
    glVertex2iv(p6);
    glVertex2iv(p7);
    glVertex2iv(p8);
    glEnd();
    */
    
    //p1,p2,p4,p3组成第一个四边形
    //p3,p4,p6,p5组成第二个四边形
    //p5,p6,p8,p7组成第三个四边形
    glBegin(GL_QUAD_STRIP);
    glVertex2iv(p1);
    glVertex2iv(p2);
    glVertex2iv(p3);
    glVertex2iv(p4);
    glVertex2iv(p5);
    glVertex2iv(p6);
    glVertex2iv(p7);
    glVertex2iv(p8);
    glEnd();
    
    
    glFlush();
    checkGLError();
}

//GL_QUADS在3.0版本中标为过时，在3.1版本中已经完全移除
std::function<void(void)> drawQuad(){
        float vertices[]={
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
       };
    unsigned int vao=flyEngine::VAOMgr::createVAO(vertices, sizeof(vertices), 3, 3*sizeof(float),false);
    return [vao](){
        VAOMgr::drawPrimitive(vao, GL_QUADS, 4);
    };
}
