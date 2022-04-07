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

#include "defines.h"
#include "mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

NS_FLYENGINE_BEGIN


class model{
private:
    std::vector<mesh> m_vecMeshes;
    std::string dir;
    bool loadModel(std::string path);
    void processNode(aiNode* node,const aiScene *scene);
    mesh processMesh(aiMesh* node,const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    
    std::string mRootDirectory;
public:
    model(char* szPath);
    void draw(shader* objShader);
};

NS_FLYENGINE_END
#endif /* model_h */
