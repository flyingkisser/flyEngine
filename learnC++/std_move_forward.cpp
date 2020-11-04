//
//  std_move_forward.cpp
//  flyEngine
//
//  Created by joe on 20/10/2020.
//  Copyright © 2020 joe. All rights reserved.
//

//#include <stdio.h>
#include <iostream>

#include "std_move_forward.h"

using namespace std;

//class Stack
//{
//public:
//    // 构造
//    Stack(int size = 1000)
//    :msize(size), mtop(0)
//    {
//    cout << "Stack(int)" << endl;
//    mpstack = new int[size];
//    }
//
//    // 析构
//    ~Stack()
//    {
//    cout << "~Stack()" << endl;
//    delete[]mpstack;
//    mpstack = nullptr;
//    }
//
//    // 拷贝构造
//    Stack(const Stack &src)
//    :msize(src.msize), mtop(src.mtop)
//    {
//    cout << "Stack(const Stack&)" << endl;
//    mpstack = new int[src.msize];
//    for (int i = 0; i < mtop; ++i) {
//        mpstack[i] = src.mpstack[i];
//    }
//    }
//
//    // 赋值重载
//    Stack& operator=(const Stack &src)
//    {
//    cout << "operator=" << endl;
//    if (this == &src)
//             return *this;
//
//    delete[]mpstack;
//
//    msize = src.msize;
//    mtop = src.mtop;
//    mpstack = new int[src.msize];
//    for (int i = 0; i < mtop; ++i) {
//        mpstack[i] = src.mpstack[i];
//    }
//    return *this;
//    }
//
//    int getSize()
//    {
//    return msize;
//    }
//private:
//    int *mpstack;
//    int mtop;
//    int msize;
//};

class Stack
{
public:
    // 构造
    Stack(int size = 1000,string str="default_stack")
    :msize(size), mtop(0),mName(str)
    {
        cout << "Stack(int)"<<"new int[] "<<size <<" "<<mName<< endl;
        mpstack = new int[size];
    }
    
    // 析构
    ~Stack()
    {
        if(mpstack!=nullptr){
            cout << "~Stack() delete[] " <<mName<< endl;
            delete[] mpstack;
            mpstack = nullptr;
        }else{
            cout << "~Stack() " <<mName<< endl;
        }
    }
    
    
    // 赋值重载
    Stack& operator=(Stack src)
    {
        cout << "operator=" <<mName<< endl;
        if (this == &src)
            return *this;
        delete[] mpstack;
        msize = src.msize;
        mtop = src.mtop;
        mpstack=src.mpstack;
        src.mpstack=nullptr;
        return *this;
    }
    
     Stack& operator=(Stack* src)
    {
        cout << "operator*=" <<mName<< endl;
        if (this == src)
            return *this;
        delete[] mpstack;
        msize = src->msize;
        mtop = src->mtop;
        mpstack=src->mpstack;
        src->mpstack=nullptr;
        return *this;
    }

    int getSize() {return msize;}
    
public:
    int *mpstack;
    
private:
    string mName;
    int mtop;
    int msize;
};


static Stack GetStack(Stack &stack)
{
    Stack tmp(stack.getSize(),"tmp");
    return tmp;
}

static Stack* GetStack2(Stack &stack)
{
//    Stack tmp(stack.getSize(),"tmp");
//    return &tmp;
    Stack* tmp=new Stack(stack.getSize(),"tmp");
    return tmp;
}

static Stack* GetStack3(Stack* stack)
{
//    Stack tmp(stack.getSize(),"tmp");
//    return &tmp;
    Stack* tmp=new Stack(stack->getSize(),"tmp");
    return tmp;
}

int std_move_forward_test()
{
    //一切正常
//    Stack s;
//    s = GetStack(s);
    
    //内存泄漏，s声明并默认值初始化以后，被GetStack2()的返回值重新赋值为"tmp"对象，
    //tmp对象是new出来的，需要显式的delete，但代码中没有，所以tmp对象得不到释放
    //return 0后，栈对象s default_stack被释放
//    Stack s;
//    s = GetStack2(s);
    
    //内存泄漏，s声明但未初始化，在GetStack3()中声明了"tmp"对象，对象指针赋值给s，
    //return 0后，因o没有显示delete tmp对象，所以tmp对象得不到释放
    //加上 delete s;即可
    Stack* s;
    s = GetStack3(s);
    
    return 0;
}
