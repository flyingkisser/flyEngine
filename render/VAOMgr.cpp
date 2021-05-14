//
//  VAOMgr.cpp
//  flyEngine
//
//  Created by joe on 04/09/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "VAOMgr.h"
#include "error.h"

//VAORender::VAORender(float* verticeBuf,unsigned int sizeOfVertice,){
//    unsigned int vao,vbo;
//    glGenVertexArrays(1,&vao);
//    glBindVertexArray(vao);
//
//    glGenBuffers(1,&vbo);
//    glBindBuffer(GL_ARRAY_BUFFER,vbo);
//    glBufferData(GL_ARRAY_BUFFER,sizeOfVertice,verticeBuf,GL_STATIC_DRAW);
//    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER,0);
//    glBindVertexArray(0);
//
//    return vao;
//}

//参数1：顶点数组缓冲区的地址
//参数2：顶点数组缓冲区的大小
//参数3：描述一个图元，需要几个顶点 如画一个三角形，给3，画四边形，给4
unsigned int VAOMgr::createVAO(void* verticeBuf,unsigned int sizeOfVertice,int numPerVertex,int stride,bool transPos){
    unsigned int vao,vbo;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeOfVertice,verticeBuf,GL_STATIC_DRAW);
    //参数1：在此vao里的vbo的序号，从0开始，也是在shader中指定的location的那个数
    //参数2：描述一个图元，需要几个顶点
    //参数3：顶点的数据类型
    //参数4：是否规范化到0-1范围内
    //参数5：从一组顶点的开始位置，到下一组顶点的开始位置，的偏移
    //参数6：第一组顶点在缓存中的起始位置
    if(transPos){
        glVertexAttribPointer(0,numPerVertex,GL_BYTE,GL_TRUE,stride,(void*)0);
    }
    else{
         glVertexAttribPointer(0,numPerVertex,GL_FLOAT,GL_FALSE,stride,(void*)0);
    }
       
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    return vao;
}

unsigned int VAOMgr::createVAO(void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerColor,int numPerTexture,
                               int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos){
    unsigned int vao,vbo;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeOfVertice,verticeBuf,GL_STATIC_DRAW);
    if(transPos){
        glVertexAttribPointer(0,numPerVertex,GL_BYTE,GL_TRUE,stride,(void*)0);
        glEnableVertexAttribArray(0);
    }
    else{
        glVertexAttribPointer(0,numPerVertex,GL_FLOAT,GL_FALSE,stride,(void*)offsetVertex);
        glVertexAttribPointer(1,numPerColor,GL_FLOAT,GL_FALSE,stride,(void*)offsetColor);
        glVertexAttribPointer(2,numPerTexture,GL_FLOAT,GL_FALSE,stride,(void*)offsetTexture);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    return vao;
}

unsigned int VAOMgr::createVAO(void* verticeBuf,int sizeOfVertice,int numPerVertex,int numPerTexture,
                               int stride,int offsetVertex,int offsetTexture){
    unsigned int vao,vbo;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeOfVertice,verticeBuf,GL_STATIC_DRAW);
    glVertexAttribPointer(0,numPerVertex,GL_FLOAT,GL_FALSE,stride,(void*)offsetVertex);
    glVertexAttribPointer(1,numPerTexture,GL_FLOAT,GL_FALSE,stride,(void*)offsetTexture);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    return vao;
}

unsigned int VAOMgr::createVAO(void* verticeBuf,int sizeOfVertice,void* verticeIndexBuf,int sizeOfVerticeIndex,int numPerVertex,int numPerColor,int numPerTexture,int stride,int offsetVertex,int offsetColor,int offsetTexture,bool transPos){
    unsigned int vao,vbo,ebo;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1,&vbo);
    glGenBuffers(1,&ebo);
    
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeOfVertice,verticeBuf,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeOfVerticeIndex,verticeIndexBuf,GL_STATIC_DRAW);
    
    if(transPos){
//        glVertexAttribPointer(0,numPerVertex,GL_BYTE,GL_TRUE,stride,(void*)0);
//        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,numPerVertex,GL_BYTE,GL_TRUE,stride,(void*)offsetVertex);
        glVertexAttribPointer(1,numPerColor,GL_BYTE,GL_TRUE,stride,(void*)offsetColor);
        glVertexAttribPointer(2,numPerTexture,GL_BYTE,GL_TRUE,stride,(void*)offsetTexture);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
    else{
        glVertexAttribPointer(0,numPerVertex,GL_FLOAT,GL_FALSE,stride,(void*)offsetVertex);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1,numPerColor,GL_FLOAT,GL_FALSE,stride,(void*)offsetColor);
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2,numPerTexture,GL_FLOAT,GL_FALSE,stride,(void*)offsetTexture);
        glEnableVertexAttribArray(2);
    }
    return vao;
}
unsigned int VAOMgr::createVAO(void* verticeBuf,int sizeOfVertice,void* verticeIndexBuf,int sizeOfVerticeIndex,int numPerVertex,int numPerTexture,int stride,int offsetVertex,int offsetTexture){
    unsigned int vao,vbo,ebo;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1,&vbo);
    glGenBuffers(1,&ebo);
    
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeOfVertice,verticeBuf,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeOfVerticeIndex,verticeIndexBuf,GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,numPerVertex,GL_FLOAT,GL_FALSE,stride,(void*)offsetVertex);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,numPerTexture,GL_FLOAT,GL_FALSE,stride,(void*)offsetTexture);
    glEnableVertexAttribArray(1);
    return vao;
}

//参数1:VAO对象id
//参数2:要绘制的图元类型，如GL_TRIANGLES
//参数3:要绘制的顶点总数，如要绘制一个三角形，这里给3，要绘制二个三角形，这里给6
void VAOMgr::drawPrimitive(unsigned int vao,unsigned int primitiveType,unsigned int vertexNumTotal){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
//    shaderObj->use();
    glBindVertexArray(vao);
    //参数1:要绘制的图元类型，如GL_TRIANGLES
    //参数2:顶点在vbo顶点数组中的偏移
    //参数3:要绘制的顶点总数，如要绘制一个三角形，这里给3，要绘制二个三角形，这里给6
    glDrawArrays(primitiveType, 0, vertexNumTotal);
}

void VAOMgr::drawPrimitive(unsigned int vao,unsigned int textureID,unsigned int primitiveType,unsigned int vertexNumTotal){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D,textureID);
    glBindVertexArray(vao);
    glDrawElements(primitiveType,vertexNumTotal,GL_UNSIGNED_INT,0);
}
