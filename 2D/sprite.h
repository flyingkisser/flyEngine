//
//  sprite.h
//  flyEngine
//
//  Created by joe on 16/07/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#ifndef sprite_h
#define sprite_h

#include <stdio.h>
#include "defines.h"
#include "node.h"
NS_FLYENGINE_BEGIN

class camera;
class shader;

class sprite : public node{
private:
    unsigned int _texID=0;
    const char* _imgFileName=NULL;
    size _size=size{0,0};
    bool _b_dirty_vertices=false;
    bool _b_dirty_shader=false;
    bool _bFlipY=false;
    bool _bFlipX=false;
    bool _bReverseColor=false;
    bool _bGray=false;
 
private:
    void _reInitVertices();
    
public:
    sprite(const char* strImageFileName);
    sprite(unsigned int texID);
    ~sprite();
    bool init();
    void glInit();
    void draw();
    
    const char* getImageFileName(){return _imgFileName;};
    unsigned int getTexID(){return _texID;};
    size getContentSize(){return _size;};
    void setContentSize(size s);
    void flipY(bool s);
    void flipX(bool s);
    void setReveseColor(bool s);
    void setGray(bool s);
    void setScale(glm::vec3 s);
    void setScale(float s);
};

NS_FLYENGINE_END


#endif /* sprite_h */
