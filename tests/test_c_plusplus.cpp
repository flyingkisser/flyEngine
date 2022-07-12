//
//  test_c_plusplus.cpp
//  flyEngine
//
//  Created by joe on 27/06/2022.
//  Copyright © 2022 joe. All rights reserved.
//

#include "test_c_plusplus.h"

#include <vector>
#include "logUtil.h"

using namespace std;
void test_vector(){
    //声明
    vector<int> s1(2);
    vector<int> s2(2,5);
    vector<int> s3={3,2,1};
    
    //插入
    s1.push_back(20);
    s1.push_back(3);
    s2[2]=6;    //已经越界，不会实际写入
    s2[3]=7;     //已经越界，不会实际写入
    //有边界检测的写入，越界会直接异常
    //s2.at(2)=6;
    
    vector<int>::iterator it=s2.begin();
    s2.insert(it,1);    //插入单个元素
    vector<int>::iterator it_s1=s1.begin();
    s2.insert(s2.begin(),it_s1,it_s1+s1.size());    //插入另一个vector
    
    //遍历
    int i=0;
     for(vector<int>::iterator it=s1.begin();it!=s1.end();it++){
         flylog("s1 [%d]:%d",i++,*it);
     }
    i=0;
    flylog("s2 size %d capacity %d",s2.size(),s2.capacity());
    for(auto v:s2){
        flylog("s2 [%d]:%d",i++,v);
    }
    
    
    //查找
    vector<int>::iterator findIT=find(s2.begin(),s2.end(),1);
    
    //删除
    s2.pop_back();  //删除一个
    if(findIT>=s2.begin() && findIT<=s2.end()){
        flylog("find value 1 at index %d from s2,delete this element!",findIT);
        s2.erase(findIT);   //删除指定位置
    }
       
    s1.clear(); //全部删除
    
    //大小
    flylog("s1 size %d capacity %d",s1.size(),s1.capacity());
    flylog("s2 size %d capacity %d",s2.size(),s2.capacity());
    
    //再次遍历
    for(auto v:s1)
        flylog("s1 %d",v);
    for(auto v:s2)
        flylog("s2 %d",v);
    
    //正排序
    sort(s2.begin(),s2.end());
    //遍历输出
    flylog("after sort");
    for(auto v:s2)
        flylog("s2 %d",v);
    
    //倒排序
    sort(s2.begin(),s2.end(),[](int a,int b){return a>b;});
    //遍历输出
    flylog("after sort again!");
    for(auto v:s2)
        flylog("s2 %d",v);
    
    
    flylog("test end");
}
