//
//  model.cpp
//  flyEngine
//
//  Created by joe on 19/10/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "model.h"
#include "mesh.h"
#include "shader.h"
#include "logUtil.h"
#include "../external/stb_image/stb_image.h"

USE_NS_FLYENGINE

model::model(char* szPath){
    loadModel(szPath);
}

bool model::loadModel(std::string path){
    Assimp::Importer importer;
    const aiScene* scene=importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        flylog("model::loadModel load %s failed : %s",path.c_str(),importer.GetErrorString());
        return false;
    }
    mRootDirectory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
    return true;
}

void model::processNode(aiNode* node,const aiScene *scene){
    for(int i=0;i<node->mNumMeshes;i++){
        aiMesh* mesh=scene->mMeshes[node->mMeshes[i]];
        m_vecMeshes.push_back(processMesh(mesh, scene));
    }
    for(int i=0;i<node->mNumChildren;i++)
        processNode(node->mChildren[i], scene);
}

//构造Mesh对象
mesh model::processMesh(aiMesh* ai_mesh,const aiScene *scene){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    //处理顶点坐标，法向量，纹理坐标，构造Vertex结构体
    for(int i=0;i<ai_mesh->mNumVertices;i++){
        Vertex vertex;
        vertex.pos_vetex.x=ai_mesh->mVertices[i].x;
        vertex.pos_vetex.y=ai_mesh->mVertices[i].y;
        vertex.pos_vetex.z=ai_mesh->mVertices[i].z;
        
        vertex.normal.x=ai_mesh->mNormals[i].x;
        vertex.normal.y=ai_mesh->mNormals[i].y;
        vertex.normal.z=ai_mesh->mNormals[i].z;
        
        if(ai_mesh->mTextureCoords[0]){
            vertex.pos_tex.x=ai_mesh->mTextureCoords[0][i].x;
            vertex.pos_tex.y=ai_mesh->mTextureCoords[0][i].y;
        }else{
            vertex.pos_tex.x=0;
            vertex.pos_tex.y=0;
        }
        vertices.push_back(vertex);
    }
    //处理顶点坐标的索引
    for(int i=0;i<ai_mesh->mNumFaces;i++){
        aiFace face=ai_mesh->mFaces[i];
        for(int j=0;j<face.mNumIndices;j++)
            indices.push_back(face.mIndices[j]);
    }
    //处理纹理对象
    if(ai_mesh->mMaterialIndex>=0){
        aiMaterial* mt=scene->mMaterials[ai_mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps=loadMaterialTextures(mt, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(),diffuseMaps.end());
        std::vector<Texture> specularMaps=loadMaterialTextures(mt, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(),specularMaps.end());
    }
    
    return mesh(vertices,indices,textures);
}

std::vector<Texture> model::loadMaterialTextures(aiMaterial *mat, aiTextureType strType, std::string typeName){
    std::vector<Texture> textures;
    for(int i=0;i<mat->GetTextureCount(strType);i++){
        aiString str;
        Texture texture;
        mat->GetTexture(strType,i,&str);
        texture.id=TextureFromFile(str.C_Str(),mRootDirectory.c_str(),false);
        texture.type=typeName;
//        texture.path=str;
        textures.push_back(texture);
    }
    return textures;
}
    



void model::draw(shader *objShader){
    for(int i=0;i<m_vecMeshes.size();i++){
        m_vecMeshes[i].draw(objShader);
    }
}
