//
//  model.cpp
//  flyEngine
//
//  Created by joe on 19/10/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "model.h"
#include "mesh.h"

#include "../external/stb_image/stb_image.h"

#include "logUtil.h"
#include "texture.h"
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "action.h"
#include "camera.h"
#include "world.h"
#include "directionLight.h"
#include "material.h"

USE_NS_FLYENGINE

model::model(char* szPath){
    loadModel(szPath);
}

bool model::loadModel(std::string path){
    Assimp::Importer importer;
    //aiProcess_GenNormals 创建法向量信息
    //aiProcess_SplitLargeMeshes 如果同时绘制的顶点数有限制，则拆分成子meash
    //aiProcess_OptimizeMeshes 和以上操作相反，合并子mesh，以减少draw call数
    //aiProcess_Triangulate 如果有不是三角形的部分，会把基本的开关转换成三角形
    //aiProcess_FlipUVs 处理纹理坐标时把y进行翻转
    const aiScene* scene=importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        flylog("model::loadModel load %s failed : %s",path.c_str(),importer.GetErrorString());
        return false;
    }
    mRootDirectory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
    flylog("model %s have total vertices %d",path.c_str(),m_totalVertics);
    return true;
}

void model::processNode(aiNode* node,const aiScene *scene){
    for(int i=0;i<node->mNumMeshes;i++){
        mesh meshObj=processMesh(scene->mMeshes[node->mMeshes[i]], scene);
        if(_cb_before_draw!=NULL)
            meshObj.setCBBeforeDraw(_cb_before_draw);
        m_vecMeshes.push_back(meshObj);
        flylog("mesh index %d processed![vertices %d]",node->mMeshes[i],meshObj.getVerticeCount());
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
        m_totalVertics++;
    }
    //处理顶点坐标的索引
    for(int i=0;i<ai_mesh->mNumFaces;i++){
        aiFace face=ai_mesh->mFaces[i];
        for(int j=0;j<face.mNumIndices;j++)
            indices.push_back(face.mIndices[j]);
    }
    //处理纹理对象
    if(ai_mesh->mMaterialIndex>=0){
        aiMaterial* aiMT=scene->mMaterials[ai_mesh->mMaterialIndex];
        std::map<int,std::vector<Texture>>::iterator it=m_mapTexture.find(ai_mesh->mMaterialIndex);
        if(it!=m_mapTexture.end()){
            textures=it->second;
        }else{
            std::vector<Texture> diffuseMaps=loadMaterialTextures(aiMT, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(),diffuseMaps.end());
            std::vector<Texture> specularMaps=loadMaterialTextures(aiMT, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(),specularMaps.end());
            m_mapTexture[ai_mesh->mMaterialIndex]=textures;
            flylog("material index %d processed!",ai_mesh->mMaterialIndex);
        }
    }
    return mesh(vertices,indices,textures);
}

std::vector<Texture> model::loadMaterialTextures(aiMaterial *aiMT, aiTextureType aiTexType){
    std::vector<Texture> textures;
    for(int i=0;i<aiMT->GetTextureCount(aiTexType);i++){
        aiString str;
        Texture texture;
        aiMT->GetTexture(aiTexType,i,&str);
        string filename = string(str.C_Str());
        filename = mRootDirectory + '/' + filename;
        texture.id=textureMgr::getInstance()->getTextureID(filename.c_str());
        if(aiTexType==aiTextureType_DIFFUSE)
            texture.type=TYPE_Diffuse;
        else if(aiTexType==aiTextureType_SPECULAR)
             texture.type=TYPE_Specular;
        else if(aiTexType==aiTextureType_SPECULAR)
             texture.type=TYPE_Specular;
        //texture.type= aiTexType==aiTextureType_DIFFUSE? TYPE_Diffuse : TYPE_Specular;
        textures.push_back(texture);
        flylog("load texture %s as type %d id %d",filename.c_str(),texture.type,texture.id);
    }
    return textures;
}


bool model::init(){
    setPosition(glm::vec3(0,0,-5));
    if(_shaderObj==NULL){
        _shaderObj=shaderMgr::getModelShader();
    }
    if(_shaderObj==NULL){
        flylog("model::init shaderObj is null,return!");
        return false;
    }
    _gl_program=_shaderObj->getProgramID();
    glInit();
    return true;
}

void model::glInit(){

}

void model::draw(camera* cameraObj){
    _shaderObj->use();
    cameraObj->update(_gl_program);
    node::updateModel();
    node::glUpdateLight();
    for(int i=0;i<m_vecMeshes.size();i++){
        m_vecMeshes[i].draw(_shaderObj);
    }
}
