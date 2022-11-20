//
//  mesh.h
//  flyEngine
//
//  Created by joe on 19/10/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef mesh_h
#define mesh_h

#include <stdio.h>
#include "defines.h"
#include "shader.h"

NS_FLYENGINE_BEGIN

struct Vertex{
    glm::vec3 pos_vetex;
    glm::vec3 normal;
    glm::vec2 pos_texcoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

enum TextureType{
    TYPE_Diffuse,TYPE_Specular,TYPE_Ambient,TYPE_Normal
};

struct Texture{
    unsigned int id;
    TextureType type;
    std::string path;
};

class mesh{
public:
    std::vector<Vertex> m_vecVertices;
    std::vector<unsigned int> m_vecIndices;
    std::vector<Texture> m_vecTextures;

    mesh(std::vector<Vertex> vecVertices,std::vector<unsigned int> vecIndices,std::vector<Texture> vecTextures);
    void draw(shader* s);
    void drawInstanced(shader* s,int count);
    int getVerticeCount(){return m_vecVertices.size();};
    void setCBBeforeDraw(std::function<void()> cb){_cb_before_draw=cb;};
    unsigned int getVAO(){return m_int_vao;};
private:
    unsigned int m_int_vao,m_int_vbo,m_int_ebo;
    std::function<void()> _cb_before_draw=NULL;
    void setupMesh();
};


NS_FLYENGINE_END

#endif
