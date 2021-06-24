//
//  3DNode.cpp
//  flyEngine
//
//  Created by joe on 24/06/2021.
//  Copyright © 2021 joe. All rights reserved.
//

#include "3DNode.h"

void 3DNode::updateModel(){
    glUniformMatrix4fv(glGetUniformLocation(_shaderID, _matNameModel.c_str()), 1,GL_FALSE,glm::value_ptr(_matModel));
}

void camera::print(){
    cout<<"DNode: "<<endl;
}

camera::DNode(std::string texPath){
    _texPath=texPath;
    _shaderID=ShaderUtil::create(texPath);
    _screenRatio=screenRatio;
    
    _matModel=matModel;
    _matModelOrigin=matModel;
  

    updateModel();
    
    

}
