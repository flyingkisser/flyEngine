//
//  animation.h
//  flyEngine
//
//  Created by joe on 30/01/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef animation_h
#define animation_h

#include <stdio.h>
#include <map>
#include "defines.h"
#include "cubeColor.h"

#include "assimp/anim.h"
#include "bone.h"
#include "model.h"

using namespace std;

NS_FLYENGINE_BEGIN

struct AssimpNodeData{
    std::string name;
    glm::mat4 transformation;
    int childrenCount;
    std::vector<AssimpNodeData> childrens;
};


class animation
{
private:
    float _duration;
    int _ticksPerSecond;
    std::vector<bone> _bones;
    AssimpNodeData _rootNode;
    std::map<std::string,BoneInfo> _boneInfoMap;

public:
    animation(std::string& aniPath,model* modelObj);
    bone* findBone(std::string& name);
    inline float getTicksPersecond(){return _ticksPerSecond;};
    inline float getDuration(){return _duration;};
    inline AssimpNodeData& getRootNode(){return _rootNode;};
    inline std::map<std::string,BoneInfo>& getBoneInfoMap(){return _boneInfoMap;};
   
private:
    void _readMissingBones(aiAnimation* aniObj,model& modelObj);
    void _readHeirarchyData(AssimpNodeData& dest,aiNode* src);
};

NS_FLYENGINE_END

#endif /* animation_h */
