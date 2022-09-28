//
//  modelIns.h
//  flyEngine
//
//  Created by joe on 26/09/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef modelIns_h
#define modelIns_h

#include <stdio.h>
#include <map>

#include "defines.h"
#include "nodeIns.h"
#include "mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

NS_FLYENGINE_BEGIN

class modelIns:public nodeIns{
private:
    std::vector<mesh> m_vecMeshes;
    std::string dir;
    std::string mRootDirectory;
    int m_totalVertics=0;
    int m_totalMesh=0;
    std::map<int,std::vector<Texture>> m_mapTexture;
//    std::function<void()> _cb_before_draw=NULL;
    
    bool loadModel(std::string path);
    void processNode(aiNode* node,const aiScene *scene);
    mesh processMesh(aiMesh* node,const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType aiTexType);

    

public:
    modelIns(char* szPath,int count);

    bool init();
    void draw();
    void useInsByVBO();

};

NS_FLYENGINE_END

#endif /* modelIns_h */
