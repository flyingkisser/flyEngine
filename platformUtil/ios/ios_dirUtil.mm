//
//  ios_dirUtil.cpp
//  flyEngine_ios
//
//  Created by joe on 15/08/2022.
//
#import <UIKit/UIKit.h>

#include "ios_dirUtil.h"
#include "logUtil.h"
#include "fileUtil.h"
#include "stringUtil.h"

USE_NS_FLYENGINE
const char* ios_dirUtil::getFileFullPathName(const char* filePathName){
    if(filePathName[0]=='/')
        return filePathName;
    std::string dirPath=stringUtil::getStrBeforeFromLast(std::string(filePathName),std::string("/"));
    std::string filePath=stringUtil::getStrAfterFromLast(std::string(filePathName),std::string("/"));
 
    NSString* nsFilePath=[[NSString alloc] initWithCString:filePath.c_str() encoding:NSASCIIStringEncoding];
    NSString* nsDirPath=[[NSString alloc] initWithCString:dirPath.c_str() encoding:NSASCIIStringEncoding];
 
    NSString* path = [[NSBundle mainBundle] pathForResource:nsFilePath ofType:nil inDirectory:nsDirPath];  //work
    
    if(path==nil)
        return "";
    const char* pathChar=[path UTF8String];
    bool isE=fileUtil::isExists(pathChar);
    if(isE)
        return pathChar;
    return "";
}

// /private/var/containers/Bundle/Application/AE38E292-B257-4E17-9F7A-44DC390373A0/flyEngine_ios.app
const char* ios_dirUtil::getBundleRootPath(){
    NSString* path = [[NSBundle mainBundle] bundlePath];
    return [path UTF8String];
}

// /var/mobile/Containers/Data/Application/FBE68CCF-CE47-4A3B-8F3C-F212AFFD99B2
const char* ios_dirUtil::getHomePath(){
    NSString* path =NSHomeDirectory();
    return [path UTF8String];
}

// /var/mobile/Containers/Data/Application/FBE68CCF-CE47-4A3B-8F3C-F212AFFD99B2/Documents
const char* ios_dirUtil::getDocumentPath(){
    NSString* path =NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES).firstObject;
    return [path UTF8String];
}

// /var/mobile/Containers/Data/Application/FBE68CCF-CE47-4A3B-8F3C-F212AFFD99B2/Library/Caches
const char* ios_dirUtil::getCachePath(){
    NSString* path =NSSearchPathForDirectoriesInDomains(NSCachesDirectory,NSUserDomainMask,YES).firstObject;
    return [path UTF8String];
}

// /var/mobile/Containers/Data/Application/FBE68CCF-CE47-4A3B-8F3C-F212AFFD99B2/Library/Application Support
const char* ios_dirUtil::getApplicationSupportPath(){
    NSString* path =NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory,NSUserDomainMask,YES).firstObject;
    return [path UTF8String];
}

// /private/var/mobile/Containers/Data/Application/FBE68CCF-CE47-4A3B-8F3C-F212AFFD99B2/tmp/
const char* ios_dirUtil::getTmpPath(){
    NSString* path =NSTemporaryDirectory();
    return [path UTF8String];
}

void ios_dirUtil::test(){
    flylog("getFileFullPathName %s",getFileFullPathName("res/fire.png"));
    flylog("getFileFullPathName %s",getFileFullPathName("/res/fire.png"));
    flylog("getFileFullPathName %s",getFileFullPathName("./res/fire.png"));
    flylog("getFileFullPathName %s",getFileFullPathName("res/shader/3d_1tex.vs"));
    flylog("getFileFullPathName %s",getFileFullPathName("./res/shader/3d_1tex.vs"));
 
    flylog("getBundleRootPath %s",getBundleRootPath());
    flylog("getDocumentPath %s",getDocumentPath());
    flylog("getCachePath %s",getCachePath());
    flylog("getApplicationSupportPath %s",getApplicationSupportPath());
    flylog("getTmpPath %s",getTmpPath());
    flylog("getHomePath %s",getHomePath());
}
