//
//  fileUtil.cpp
//  lines
//
//  Created by joe on 19/06/2020.
//  Copyright © 2020 joe. All rights reserved.
//

#include "fileUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logUtil.h"

unsigned char* fileUtil::readFile(const char* szFileName){
    FILE* fFile=fopen(szFileName, "rb");
    if(fFile==NULL){
        flylogErrno("readFile %s error!!!  ",szFileName);
        return NULL;
    }
        
    fseek(fFile,0,SEEK_END);
    long fileSize=ftell(fFile);
    long bufSize=fileSize+1;
    fseek(fFile,0,SEEK_SET);
    unsigned char* buf=(unsigned char*)malloc(bufSize);
    if(buf==NULL){
        fclose(fFile);
        return NULL;
    }
    memset(buf,0,bufSize);
    if(!fread(buf,1,fileSize,fFile)){
        fclose(fFile);
        free(buf);
        return NULL;
    }
    fclose(fFile);
    return buf;
}

int fileUtil::writeFile(const char* szFileName,unsigned char* buf,int size){
    FILE* fFile=fopen(szFileName, "wb");
       if(fFile==NULL)
           return 0;
    int written=(int)fwrite(buf,1,size,fFile);
    fclose(fFile);
    return written;
}

bool fileUtil::isExists(const char* szFileName){
    FILE* fFile=fopen(szFileName, "r");
       if(fFile==NULL)
           return false;
    fclose(fFile);
    return true;
}
