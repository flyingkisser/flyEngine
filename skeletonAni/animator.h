//
//  animator.h
//  flyEngine
//
//  Created by Joe on 2023/1/30.
//  Copyright © 2023 joe. All rights reserved.
//

#ifndef animator_h
#define animator_h

#include <stdio.h>
#include <map>
#include "defines.h"
#include "cubeColor.h"

#include "animation.h"
#include "node.h"

using namespace std;

NS_FLYENGINE_BEGIN

class animator:public node
{
private:
    float _currentTime;
    float _delteTime;
    float _lastTime;
    animation* _currentAnimation;
    std::vector<glm::mat4> _finalBoneMatrices;
    
public:
    animator(animation* aniObj);
    void updateAnimation(float dt);
    void playAnimation(animation* aniObj);
    void calculateBoneTransform(AssimpNodeData* nodeData,glm::mat4 parentTransform);
    std::vector<glm::mat4> getFinalBoneMatries(){return _finalBoneMatrices;};
    
    void draw();
    bool init();
};

NS_FLYENGINE_END

#endif /* animator_h */
