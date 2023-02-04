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
#include "textureMgr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "action.h"
#include "camera.h"
#include "world.h"
#include "directionLight.h"
#include "mathUtil.h"
#ifdef BUILD_IOS
#include "ios_dirUtil.h"
#endif

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
#ifdef BUILD_IOS
    path=ios_dirUtil::getFileFullPathName(path.c_str());
#endif
    const aiScene* scene=importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs|aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        flylog("model::loadModel load %s failed : %s",path.c_str(),importer.GetErrorString());
        return false;
    }
    mRootDirectory = path.substr(0, path.find_last_of('/'));
    processNode2(scene->mRootNode, scene);
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
        
        vertex.tangent.x=ai_mesh->mTangents[i].x;
        vertex.tangent.y=ai_mesh->mTangents[i].y;
        vertex.tangent.z=ai_mesh->mTangents[i].z;
        
        vertex.bitangent.x=ai_mesh->mBitangents[i].x;
        vertex.bitangent.y=ai_mesh->mBitangents[i].y;
        vertex.bitangent.z=ai_mesh->mBitangents[i].z;

        if(ai_mesh->mTextureCoords[0]){
            vertex.pos_texcoord.x=ai_mesh->mTextureCoords[0][i].x;
            vertex.pos_texcoord.y=ai_mesh->mTextureCoords[0][i].y;
        }else{
            vertex.pos_texcoord.x=0;
            vertex.pos_texcoord.y=0;
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
            std::vector<Texture> normalMaps=loadMaterialTextures(aiMT, aiTextureType_HEIGHT);
            textures.insert(textures.end(), normalMaps.begin(),normalMaps.end());
            m_mapTexture[ai_mesh->mMaterialIndex]=textures;
            flylog("material index %d processed!",ai_mesh->mMaterialIndex);
        }
    }
    return mesh(vertices,indices,textures);
}

void model::processNode2(aiNode* node,const aiScene *scene){
    for(int i=0;i<node->mNumMeshes;i++){
        int c=processMesh2(scene->mMeshes[node->mMeshes[i]], scene);
        flylog("mesh index %d processed![vertices %d]",node->mMeshes[i],c);
    }
    for(int i=0;i<node->mNumChildren;i++)
        processNode2(node->mChildren[i], scene);
    _meshObj=new mesh(_vertices,_indices,_textures);
    if(_cb_before_draw!=NULL)
        _meshObj->setCBBeforeDraw(_cb_before_draw);
}

int model::processMesh2(aiMesh* ai_mesh,const aiScene *scene){
    //处理顶点坐标，法向量，纹理坐标，构造Vertex结构体
    for(int i=0;i<ai_mesh->mNumVertices;i++){
        Vertex vertex;
        vertex.pos_vetex.x=ai_mesh->mVertices[i].x;
        vertex.pos_vetex.y=ai_mesh->mVertices[i].y;
        vertex.pos_vetex.z=ai_mesh->mVertices[i].z;

        vertex.normal.x=ai_mesh->mNormals[i].x;
        vertex.normal.y=ai_mesh->mNormals[i].y;
        vertex.normal.z=ai_mesh->mNormals[i].z;

        vertex.tangent.x=ai_mesh->mTangents[i].x;
        vertex.tangent.y=ai_mesh->mTangents[i].y;
        vertex.tangent.z=ai_mesh->mTangents[i].z;
        
        vertex.bitangent.x=ai_mesh->mBitangents[i].x;
        vertex.bitangent.y=ai_mesh->mBitangents[i].y;
        vertex.bitangent.z=ai_mesh->mBitangents[i].z;

        if(ai_mesh->mTextureCoords[0]){
            vertex.pos_texcoord.x=ai_mesh->mTextureCoords[0][i].x;
            vertex.pos_texcoord.y=ai_mesh->mTextureCoords[0][i].y;
        }else{
            vertex.pos_texcoord.x=0;
            vertex.pos_texcoord.y=0;
        }
        _vertices.push_back(vertex);
        m_totalVertics++;
    }
    //处理顶点坐标的索引
    int offset=_indices.size();
    for(int i=0;i<ai_mesh->mNumFaces;i++){
        aiFace face=ai_mesh->mFaces[i];
        for(int j=0;j<face.mNumIndices;j++)
            _indices.push_back(face.mIndices[j]+offset);
    }
    //处理纹理对象
    if(ai_mesh->mMaterialIndex>=0){
        aiMaterial* aiMT=scene->mMaterials[ai_mesh->mMaterialIndex];
        std::map<int,std::vector<Texture>>::iterator it=m_mapTexture.find(ai_mesh->mMaterialIndex);
        if(it!=m_mapTexture.end()){
            _textures=it->second;
        }else{
            std::vector<Texture> diffuseMaps=loadMaterialTextures(aiMT, aiTextureType_DIFFUSE);
            _textures.insert(_textures.end(), diffuseMaps.begin(),diffuseMaps.end());
            std::vector<Texture> specularMaps=loadMaterialTextures(aiMT, aiTextureType_SPECULAR);
            _textures.insert(_textures.end(), specularMaps.begin(),specularMaps.end());
            std::vector<Texture> normalMaps=loadMaterialTextures(aiMT, aiTextureType_HEIGHT);
            _textures.insert(_textures.end(), normalMaps.begin(),normalMaps.end());
            m_mapTexture[ai_mesh->mMaterialIndex]=_textures;
            flylog("material index %d processed!",ai_mesh->mMaterialIndex);
        }
    }
    extractBoneWeightForVertices(_vertices,ai_mesh,scene);
    return ai_mesh->mNumVertices;
}
void model::extractBoneWeightForVertices(std::vector<Vertex>& vertices,aiMesh* mesh,const aiScene* scene){
    for(int i=0;i<mesh->mNumBones;i++){
        int boneID=-1;
        std::string boneName=mesh->mBones[i]->mName.C_Str();
        if(m_boneInfoMap.find(boneName)==m_boneInfoMap.end()){
            BoneInfo newBoneInfo;
            newBoneInfo.id=m_boneCounter++;
            newBoneInfo.offset=mathUtil::convertMatrixToGLMFormat(mesh->mBones[i]->mOffsetMatrix);
            m_boneInfoMap[boneName]=newBoneInfo;
            boneID=newBoneInfo.id;
        }
        else
            boneID=m_boneInfoMap[boneName].id;
        if(boneID==-1){
            flylog("extractBoneWeightForVertices:find boneID -1!return!");
            return;
        }
        auto weights=mesh->mBones[i]->mWeights;
        int numWeight=mesh->mBones[i]->mNumWeights;
        for(int j=0;j<numWeight;j++){
            int vertexID=weights[j].mVertexId;
            auto weight=weights[j].mWeight;
            if(vertexID>=vertices.size()){
                flylog("extractBoneWeightForVertices:find vertexID %d >= vetices.size()",vertexID,vertices.size());
                continue;
            }
            setVertexBoneData(vertices[vertexID], boneID, weight);
//            flylog("set vertexID %d boneID %d weight %f",vertexID,boneID,weight);
        }
//        flylog("extractBoneWeightForVertices:bone %s set %d num of weight",boneName.c_str(),numWeight);
    }
    flylog("extractBoneWeightForVertices:mesh %s have bones %d",mesh->mName.C_Str(),mesh->mNumBones);
}
void model::setVertexBoneData(Vertex &stVertex, int boneID, float weight){
    for(int i=0;i<MAX_BONE_INFLUENCE;i++){
        if(stVertex.boneIDArr[i]>=0)
            continue;
        stVertex.boneIDArr[i]=boneID;
        stVertex.weightArr[i]=weight;
        break;
    }
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
        else if(aiTexType==aiTextureType_AMBIENT)
             texture.type=TYPE_Ambient;
        else if(aiTexType==aiTextureType_HEIGHT)
             texture.type=TYPE_Normal;
        //texture.type= aiTexType==aiTextureType_DIFFUSE? TYPE_Diffuse : TYPE_Specular;
        textures.push_back(texture);
        flylog("load texture %s as type %d id %d",filename.c_str(),texture.type,texture.id);
    }
    return textures;
}

void model::setVertexBoneDataToDefault(Vertex &stVertex){
    for(int i=0;i<MAX_BONE_INFLUENCE;i++){
        stVertex.boneIDArr[i]=-1;
        stVertex.weightArr[i]=0;
    }
}




bool model::init(){
//    setPosition(glm::vec3(0,0,-5));
    setDirtyPos(true);
    if(_shaderObj==NULL){
        _shaderObj=shaderMgr::getModelShader();
//        _shaderObj=shaderMgr::get3d1texPongShader();
    }
    if(_shaderObj==NULL){
        flylog("model::init shaderObj is null,return!");
        return false;
    }
    _gl_program=_shaderObj->getProgramID();
    glInit();
    return true;
}

void model::draw(){
    _shaderObj->use();
//    if(isDirtyAll()){
        node::updateModel();
        node::glUpdateLight();
//        setDirtyAll(false);
//    }
//    for(int i=0;i<m_vecMeshes.size();i++){
//        m_vecMeshes[i].draw(_shaderObj);
//    }
    _meshObj->draw(_shaderObj);
}

void model::drawSimple(){
    _shaderObj->use();
    node::updateModel();
    node::glUpdateLight();
    _meshObj->drawSimple(_shaderObj);
}
