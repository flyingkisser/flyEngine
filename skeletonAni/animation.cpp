//
//  animation.cpp
//  flyEngine
//
//  Created by joe on 30/01/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#include "animation.h"
#include "logUtil.h"
#include "mathUtil.h"

USE_NS_FLYENGINE

animation::animation(char* path){
    Assimp::Importer importer;
    //aiProcess_GenNormals 创建法向量信息
    //aiProcess_SplitLargeMeshes 如果同时绘制的顶点数有限制，则拆分成子meash
    //aiProcess_OptimizeMeshes 和以上操作相反，合并子mesh，以减少draw call数
    //aiProcess_Triangulate 如果有不是三角形的部分，会把基本的开关转换成三角形
    //aiProcess_FlipUVs 处理纹理坐标时把y进行翻转
#ifdef BUILD_IOS
    path=ios_dirUtil::getFileFullPathName(path.c_str());
#endif
//    const aiScene* scene=importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs|aiProcess_CalcTangentSpace);
    const aiScene* scene=importer.ReadFile(path,aiProcess_Triangulate);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        flylog("model::loadModel load %s failed : %s",path,importer.GetErrorString());
        return;
    }
    auto ani=scene->mAnimations[0];
    _duration=ani->mDuration;
    _ticksPerSecond=ani->mTicksPerSecond;
    _model=new model(path);
    _model->init();
    _readHeirarchyData(&_rootNode, scene->mRootNode);
    _readMissingBones(ani,_model);
}

bone* animation::findBone(std::string& name){
    auto iter=std::find_if(_bones.begin(),_bones.end(),[&](bone& boneObj){
        return boneObj.getBoneName()==name;
    });
    if(iter==_bones.end())
        return nullptr;
    return &(*iter);
}

void animation::_readMissingBones(aiAnimation* aniObj,model* modelObj){
    auto& boneInfoMap=modelObj->getBoneInfoMap();
    int boneCount=modelObj->getBoneCount();
    for(int i=0;i<boneCount;i++){
        auto channel=aniObj->mChannels[i];
        std::string boneName=channel->mNodeName.data;
        if(boneInfoMap.find(boneName)==boneInfoMap.end()){
            boneInfoMap[boneName].id=boneCount;
            boneCount++;
            flylog("animation:find missing bone %s",boneName.c_str());
        }
        _bones.push_back(bone(channel->mNodeName.data,boneInfoMap[channel->mNodeName.data].id,channel));
    }
    _boneInfoMap=boneInfoMap;
}

void animation::_readHeirarchyData(AssimpNodeData* dest,aiNode* src){
    if(src==nullptr){
        flylog("animation::_readHeirarchyData src is null,return!");
        return;
    }
    dest->name=src->mName.data;
    dest->transformation=mathUtil::convertMatrixToGLMFormat(src->mTransformation);
    dest->childrenCount=src->mNumChildren;
    for(int i=0;i<src->mNumChildren;i++){
        AssimpNodeData newData;
        _readHeirarchyData(&newData, src->mChildren[i]);
        dest->childrens.push_back(newData);
    }
//    flylog("animation:%s have %d children",dest->name.c_str(),dest->childrenCount);
}
