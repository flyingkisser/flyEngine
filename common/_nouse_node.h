////
////  node.h
////  flyEngine
////
////  Created by joe on 01/07/2020.
////  Copyright © 2020 joe. All rights reserved.
////
//
//#ifndef node_h
//#define node_h
//
//#include "defines.h"
//
//using namespace std;
//
//namespace flyEngine {
//
//class node{
//
//public:
//    node();
//    ~node();
//    virtual void draw()=0;
//    p2 getPosition();
//    void setPosition(float x,float y);
//    void setPosition(p2* pos);
//    void addChild(node*);
//    size getSize(){return _size;};
//    void move(float x,float y);
//
//protected:
//    vector<node*> _vectorChild;
//    float sx,sy;
//    float width,height;
//    int _refCount;
//    p2 _pos;
//    size _size;
//    bool _dirty=false;
//
//
//
//};
//
//}
//
//#endif /* node_h */
