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
    glm::vec3 pos_tex;
};

enum TextureType{
    TYPE_Diffuse,TYPE_Specular,TYPE_Ambient
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
    void drawIns(shader* s,int count);
    int getVerticeCount(){return m_vecVertices.size();};
    void setCBBeforeDraw(std::function<void()> cb){_cb_before_draw=cb;};
private:
    unsigned int m_int_vao,m_int_vbo,m_int_ebo;
    std::function<void()> _cb_before_draw=NULL;
    void setupMesh();
};


NS_FLYENGINE_END

#endif
