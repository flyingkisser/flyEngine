//
//  model.h
//  flyEngine
//
//  Created by joe on 19/10/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#ifndef model_h
#define model_h

#include <stdio.h>
#include <map>

#include "defines.h"
#include "mesh.h"
#include "node.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

NS_FLYENGINE_BEGIN

class shader;

class model : public node{
private:
    std::vector<mesh> m_vecMeshes;
    std::string dir;
    std::string mRootDirectory;
    int m_totalVertics=0;
    int m_totalMesh=0;
    std::map<int,std::vector<Texture>> m_mapTexture;
    std::function<void()> _cb_before_draw=NULL;
    
    bool loadModel(std::string path);
    void processNode(aiNode* node,const aiScene *scene);
    mesh processMesh(aiMesh* node,const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType aiTexType);


public:
    model(char* szPath);

    bool init();
    void glInit();
    void draw(camera* cameraObj);
    
    void setCBBeforeDraw(std::function<void()> cb){_cb_before_draw=cb;};
};

NS_FLYENGINE_END
#endif
