//
//  windowUtil.cpp
//  flyEngine_ios
//
//  Created by joe on 15/08/2022.
//

#include "windowUtil.h"
#include <UIKit/UIKit.h>

USE_NS_FLYENGINE

flyEngine::size windowUtil::getWinSize(){
    CGRect rect_screen = [[UIScreen mainScreen] bounds];
    CGSize size_screen = rect_screen.size;
    flyEngine::size st={(float)size_screen.width,(float)size_screen.height};
    return st;
}

flyEngine::size windowUtil::getResolutionSize(){
    CGRect rect_screen = [[UIScreen mainScreen] bounds];
    CGSize size_screen = rect_screen.size;
    CGFloat scale_screen = [UIScreen mainScreen].scale;
    NSLog(@"scale_screen:%.f", scale_screen);
    float widthResolution = size_screen.width * scale_screen;
    float heightResolution = size_screen.height * scale_screen;
    flyEngine::size st={widthResolution,heightResolution};
    return st;
}
