//
//  mesh.cpp
//  flyEngine
//
//  Created by joe on 19/10/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "mesh.h"
#include "state.h"

USE_NS_FLYENGINE;


mesh::mesh(std::vector<Vertex> vecVertices,std::vector<unsigned int> vecIndices,std::vector<Texture> vecTextures){
    m_vecVertices=vecVertices;
    m_vecIndices=vecIndices;
    m_vecTextures=vecTextures;
    setupMesh();
}

void mesh::setupMesh(){
    glGenVertexArrays(1,&m_int_vao);
    glGenBuffers(1,&m_int_vbo);
    glGenBuffers(1,&m_int_ebo);
    glBindVertexArray(m_int_vao);
    glBindBuffer(GL_ARRAY_BUFFER,m_int_vbo);
    glBufferData(GL_ARRAY_BUFFER,m_vecVertices.size()*sizeof(Vertex),&m_vecVertices[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_int_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_vecIndices.size()*sizeof(int),&m_vecIndices[0],GL_STATIC_DRAW);
    //顶点数组
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
    //法向量
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,normal));
    //纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,pos_tex));

    glBindVertexArray(0);
}

void mesh::draw(shader* s){
    unsigned int diffuseIndex=0;
    unsigned int specularIndex=0;
    for(int i=0;i<m_vecTextures.size();i++){
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, m_vecTextures[i].id);
        std::string strIndex;
        TextureType texType=m_vecTextures[i].type;
        if(texType==TYPE_Diffuse){
            strIndex=std::to_string(diffuseIndex++);
            s->setInt(("texture_diffuse_"+strIndex).c_str(),i);
        }
        else if(texType==TYPE_Specular){
            strIndex=std::to_string(specularIndex++);
            s->setInt(("texture_specular_"+strIndex).c_str(),i);
        }
    }
    if(_cb_before_draw)
        _cb_before_draw();
    //draw mesh
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(m_int_vao);
    glDrawElements(GL_TRIANGLES,(int)m_vecIndices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
    state::log((int)m_vecIndices.size());
}
