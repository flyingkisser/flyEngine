//
//  AppDelegate.h
//  gles1
//
//  Created by joe on 10/08/2022.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

//@interface AppDelegate : UIResponder <UIApplicationDelegate>
@interface AppDelegate : UIResponder <UIApplicationDelegate,GLKViewDelegate,GLKViewControllerDelegate>
@property (strong, nonatomic) UIWindow *window;

@end

