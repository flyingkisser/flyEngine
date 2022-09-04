//
//  main.m
//  gles1
//
//  Created by joe on 10/08/2022.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#include "defines.h"
#include "window.h"
#include "logUtil.h"
#include "glslUtil.h"

USE_NS_FLYENGINE

int main(int argc, char * argv[]) {
//    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
//        appDelegateClassName = NSStringFromClass([AppDelegate class]);
        NSLog(@"%@",@"main:start!");
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
   
//    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
