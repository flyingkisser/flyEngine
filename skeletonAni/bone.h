//
//  bone.h
//  flyEngine
//
//  Created by joe on 27/01/2023.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef bone_h
#define bone_h

#include <stdio.h>

#include "defines.h"
#include "cubeColor.h"

#include "assimp/anim.h"

using namespace std;

NS_FLYENGINE_BEGIN

struct KeyPosition{
    glm::vec3 position;
    float timeStamp;
};
struct KeyRotation{
    glm::quat orientation;
    float timeStamp;
};
struct KeyScale{
    glm::vec3 scale;
    float timeStamp;
};


class bone
{
private:
    std::vector<KeyPosition> _positions;
    std::vector<KeyRotation> _rotations;
    std::vector<KeyScale> _scales;
    glm::mat4 _localTransform;
    std::string _name;
    int _id;
    int _numPositions;
    int _numRotations;
    int _numScales;

public:
    bone(std::string name,int id,aiNodeAnim* channel);
    void update(float animationTime);
    glm::mat4 getLocalTransform() { return _localTransform;};
    std::string getBoneName() const { return _name;};
    int getBoneID() { return _id;};
    int getPositionIndex(float animationTime);
    int getRotationIndex(float animationTime);
    int getScaleIndex(float animationTime);
    void dbg_interpolatePosition(float animationTime);
    
private:
    float _getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
    glm::mat4 _interpolatePosition(float animationTime);
    glm::mat4 _interpolateRotation(float animationTime);
    glm::mat4 _interpolateScaling(float animationTime);
};

NS_FLYENGINE_END


#endif /* bone_h */
