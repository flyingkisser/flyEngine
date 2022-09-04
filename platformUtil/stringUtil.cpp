//
//  stringUtil.cpp
//  flyEngine
//
//  Created by joe on 22/09/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "stringUtil.h"
#include "logUtil.h"
#include <string.h>

char* stringUtil::utoa(unsigned x, char* dest, const unsigned base)
{
    char *p = dest;
    do{
        *p++ = "0123456789abcdef"[x % base];
        x /= base;
    }
    while(x);
    *p = 0;
    std::reverse(dest, p);
    return dest;
}

std::string stringUtil::float2Str(float s){
    char szFmt[32]={0};
    std::string str;
    snprintf(szFmt, sizeof(szFmt), "%f",s);
    return std::string(szFmt);
    //return std::to_string(s);
}

std::string stringUtil::getFileExtName(std::string& fileName){
    std::size_t pos=fileName.find_last_of(".");
    if(pos!=std::string::npos)
        return fileName.substr(pos,fileName.length()-pos);
    return "";
}



std::string stringUtil::getStrBefore(std::string str1,std::string strFind){
    std::size_t found=str1.find(strFind);
    if(found==std::string::npos || found==0)
        return std::string("");
    return str1.substr(0,found);
}
std::string stringUtil::getStrBeforeFromLast(std::string str1,std::string strFind){
    std::size_t found=str1.find_last_of(strFind);
    if(found==std::string::npos || found==0)
        return std::string("");
    return str1.substr(0,found);
}

std::string stringUtil::getStrAfter(std::string str1,std::string strFind){
    std::size_t found=str1.find(strFind);
    if(found==std::string::npos || found==str1.length()-1)
        return std::string("");
    return str1.substr(found+strFind.length());
}

std::string stringUtil::getStrAfterFromLast(std::string str1,std::string strFind){
    std::size_t found=str1.find_last_of(strFind);
    if(found==std::string::npos || found==str1.length()-1)
        return std::string("");
    return str1.substr(found+strFind.length());
}

std::string stringUtil::replace(std::string str1,std::string strFind,std::string strReplace){
    std::size_t found=str1.find(strFind);
    if(found==std::string::npos)
        return str1;
    return str1.replace(found,strFind.length(),strReplace);
}

const char* stringUtil::replace(const char* str1,const char* strFind,const char* strReplace){
    const char* pos=strstr(str1,strFind);
    if(pos==NULL)
        return NULL;
    int size=strlen(str1)-strlen(strFind)+strlen(strReplace);
    if(size>=1024*1024*100){
        flylog("replace:try to allocate mem %d,too big!",size);
        return NULL;
    }
    char* buf=(char*)malloc(size);
    if(buf==NULL){
        flylog("replace:try to malloc %d failed!",size);
        return NULL;
    }
    memset(buf,0,size);
    unsigned int firstSize=(unsigned int)((long long)pos-(long long)str1);
    char* lastPart=(char*)((long long)str1+strlen(strFind)+1);
    strncat(buf,str1,firstSize);
    strcat(buf,strReplace);
    strcat(buf,lastPart);
    return buf;
}

void stringUtil::test(){
    std::string s11=stringUtil::getStrBefore(std::string("/var/sh/a.vs"),std::string("/"));
    std::string s12=stringUtil::getStrBeforeFromLast(std::string("/var/sh/a.vs"),std::string("s"));
    std::string s13=stringUtil::getStrAfter(std::string("/var/sh/a.vs"),std::string("vs"));
    std::string s14=stringUtil::getStrAfterFromLast(std::string("/var/shvs/a.vs"),std::string("vs"));
    
    std::string s111=stringUtil::getStrBefore(std::string("/var/sh/a.vs"),std::string("dd"));
    std::string s112=stringUtil::getStrBeforeFromLast(std::string("/var/sh/a.vs"),std::string("dd"));
    std::string s113=stringUtil::getStrAfter(std::string("/var/sh/a.vs"),std::string("ss"));
    std::string s114=stringUtil::getStrAfterFromLast(std::string("/var/shvs/a.vs"),std::string("ss"));
    
    std::string s1=stringUtil::getStrBefore(std::string("/var/sh/a.vs"),std::string("a.vs"));
    std::string s2=stringUtil::getStrBeforeFromLast(std::string("/var/sh/a.vs"),std::string("/"));
    std::string s3=stringUtil::getStrAfter(std::string("/var/sh/a.vs"),std::string("/var"));
    std::string s4=stringUtil::getStrAfterFromLast(std::string("/var/sh/a.vs"),std::string("/"));
}


