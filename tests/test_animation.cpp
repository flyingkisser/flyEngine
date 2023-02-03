//
//  test_animation.cpp
//  flyEngine
//
//  Created by Joe on 2023/1/31.
//  Copyright © 2023 joe. All rights reserved.
//

#include "test_animation.h"
#include "test_pbr.h"
#include "world.h"
#include "control.h"
#include "logUtil.h"
#include "timerUtil.h"
#include "timeUtil.h"
#include "directionLight.h"
#include "cubeTex.h"
#include "quad.h"
#include "semiSphere.h"
#include "material2.h"
#include "texture2.h"
#include "textureHdr.h"
#include "shader.h"
#include "shaderMgr.h"
#include "randUtil.h"
#include "threadUtil.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"
#include "skybox.h"
#include "glslUtil.h"
#include "values.h"
#include "camera.h"
#include "model.h"
#include "fbo.h"
#include "window.h"
#include "bone.h"
#include "animation.h"
#include "animator.h"

USE_NS_FLYENGINE

void test_ani1(){
//    modle* modelObj=new model("res/model/vmpire/dancing_vampire.dae");
    animation* aniData=new animation("res/model/vampire/dancing_vampire.dae");
    animator* aniObj=new animator(aniData);
    world::getInstance()->addChild(aniObj);
    
}
