//
//  test_h.cpp
//  flyEngine
//
//  Created by joe on 28/10/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include <math.h>
#include "defines.h"

#include "test_node.h"
#include "texture2.h"
#include "textureMgr.h"
#include "VAOMgr.h"
#include "shader.h"
#include "shaderMgr.h"

#ifdef BUILD_MAC
#include "keyboardEventMgr.h"
#include "mouseEventMgr.h"
#endif

#include "camera.h"
#include "control.h"
#include "world.h"
#include "material2.h"
#include "cubeTex.h"
#include "cubeColor.h"
#include "directionLight.h"
#include "moveBy.h"
#include "scaleBy.h"
#include "scaleTo.h"
#include "rotateTo.h"
#include "rotateBy.h"

#include "sequence.h"
#include "spawn.h"
#include "repeat.h"
#include "forever.h"


#include "logUtil.h"
#include "timerUtil.h"
#include "threadUtil.h"
#include "window.h"
#include "glslUtil.h"


USE_NS_FLYENGINE;

static material2* createMaterial(float ambient,float diffuse,float specular,float shineness){
    return new material2(glm::vec3(ambient,ambient,ambient),glm::vec3(diffuse,diffuse,diffuse),glm::vec3(specular,specular,specular),shineness);
//    return NULL;
}

static void init_light_direction(){
    //设置环境光
    directionLight* dirLight=new directionLight(glm::vec3(1.0f,1.0f,1.0f));
    world::getInstance()->setDirectiontLight(dirLight);
    //因为只有环境光，所以设置的比较亮
    //dirLight->setMaterial(createMaterial(1.0,1.0,1.0,0.2));
}

void test_cubeColor(){
    init_light_direction();
    cubeColor* cubeObj=new cubeColor(glm::vec4(0.8,0.2,0,1));
    if(!cubeObj->init()){
       flylog("cubeObj init failed!");
       return;
    }
    cubeObj->setPosition(glm::vec3(0.8,0.8,-5));
    cubeObj->setMaterial(createMaterial(1.0,1.0,1.0,0.2));
    world::getInstance()->addChild(cubeObj);

    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);

    timerUtil* timerMgrObj=new timerUtil("light_test_timer");
    timerMgrObj->exec(0.1,[](node* _node){
       _node->rotateBy(glm::vec3(0.5f,0,0));
    },cubeObj);
}


void test_oneCubeTex(){
    init_light_direction();
    
    node* cubeObj=new cubeTex("res/wood.png");
    if(!cubeObj->init()){
        flylog("node init failed!");
        return;
    }
    cubeObj->setPosition(glm::vec3(0,0,-5));
    cubeObj->setRotation(glm::vec3(30,60,30));
    cubeObj->setMaterial(createMaterial(1.0,1.0,1.0,0.2));
    
    world::getInstance()->addChild(cubeObj);
    cubeObj->print();
    
//    cubeObj->setShader(new shader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex.fs"));
    
    //通过按住鼠标右键，控制模型旋转
    control* controlObj=world::getInstance()->getControl();
    controlObj->bindNode(cubeObj);
    
    timerUtil* timerMgrObj=new timerUtil("cube_test_timer");
    timerMgrObj->exec(0.1,[](node* _node){
        _node->rotateBy(glm::vec3(0.5f,0,0));
    },cubeObj);

    
    int proID=cubeObj->getShader()->getProgramID();
//    glslUtil::printAllUniforms(proID);
   
    glslUtil::printAllUniformAndBlock(proID);
    glslUtil::printUniformValue(proID, "matModel");
}

void test_twoCubeTex(){
    init_light_direction();
    node* nodeObj1=new cubeTex("res/fire.png");
    if(!nodeObj1->init()){
      flylog("node1 init failed!");
      return;
    }
    nodeObj1->setPosition(glm::vec3(-0.7,0,-5));
    nodeObj1->setMaterial(createMaterial(1.0,1.0,1.0,0.2));
    world::getInstance()->addChild(nodeObj1);

    //node* nodeObj2=new cubeTex("res/smile.png");
    node* nodeObj2=new cubeTex("res/fire.png");
    if(!nodeObj2->init()){
      flylog("node2 init failed!");
      return;
    }
    nodeObj2->setPosition(glm::vec3(0.7,0,-5));
    nodeObj2->setMaterial(createMaterial(1.0,1.0,1.0,128));
    world::getInstance()->addChild(nodeObj2);
    
    action* act1=new rotateBy(1,glm::vec3(10,0,0));
    action* act2=new rotateBy(1,glm::vec3(10,0,0));
    nodeObj1->runAction(new forever(1,act1));
    nodeObj2->runAction(new forever(1,act2));
    
    world::getInstance()->getControl()->bindNode(nodeObj2);
    nodeObj2->setShader(new shader("./res/shader/3d_1tex.vs","./res/shader/3d_1tex.fs"));
}

void test_multiCubeTex(int count){
    init_light_direction();
    float inner=2.0/count;
    for(int i=0;i<count;i++){
        node* nodeObj=new cubeTex("res/fire.png");
        if(!nodeObj->init()){
            flylog("node%d init failed!",i);
            return;
        }
        float x=-1+inner*(i+1);
        float y=0;
        nodeObj->setPosition(glm::vec3(x,y,-5));
        nodeObj->setMaterial(createMaterial(1.0,1.0,1.0,0.2));
        world::getInstance()->addChild(nodeObj);
        nodeObj->runAction(new forever(1,new rotateBy(1,glm::vec3(10,0,0))));
    }
}



void drawCubeRaw(){
    unsigned int vao,vbo,ebo;
    int texID1,texID2;
    int stride=5*sizeof(float);

    texture2* texObj1=textureMgr::getInstance()->getTexture("res/fire.png");
    texture2* texObj2=textureMgr::getInstance()->getTexture("res/smile.png");
    if(texObj1==NULL || texObj2==NULL)
       return;

    texID1=texObj1->getTextureID();
    texID2=texObj2->getTextureID();
   
    int shaderID=shaderMgr::createShaderFromFile("res/shader/3dTextureApha.vs","res/shader/3dTextureApha.fs");
 
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&vbo);
    glGenBuffers(1,&ebo);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(g_verticeArrWithTexCoord),g_verticeArrWithTexCoord,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride,(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,stride,(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
        
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0); //0号纹理GL_TEXTURE0
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1); //1号纹理GL_TEXTURE1
    glUniform1f(glGetUniformLocation(shaderID, "mixValue"), 0.7);
   
    glm::mat4 matModel=glm::translate(glm::mat4(1.0),glm::vec3(0,0,-3));
    flyEngine::camera* cameraObj=new flyEngine::camera();
    cameraObj->update();
    cameraObj->print();
    control* controlObj=new control();
    controlObj->bindCamera(cameraObj);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderID,"matModel"),1,GL_FALSE,glm::value_ptr(matModel));
    
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderID, "texture2"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texID1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texID2);
    glEnable(GL_DEPTH_TEST);
    
#ifdef BUILD_MAC
    while(!glfwWindowShouldClose(g_window)){
        threadUtil::sleep(0.1);   //1000 means 1ms

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texID1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texID2);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);
        
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
#endif
    
}
