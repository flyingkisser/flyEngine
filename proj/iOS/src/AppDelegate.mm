//
//  AppDelegate.m
//  gles1
//
//  Created by joe on 10/08/2022.
//

#import "AppDelegate.h"
#import <OpenGLES/ES3/glext.h>

#import "ViewController.h"

#include "defines.h"

#include "glslUtil.h"
#include "uboMgr.h"
#include "timeUtil.h"

#include "window.h"
#include "testMain.h"
#include "world.h"


USE_NS_FLYENGINE

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    NSLog(@"application:didFinishLaunchingWithOptions start!");
    // Override point for customization after application launch.
    self.window = [[UIWindow alloc]initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.backgroundColor = [UIColor whiteColor];
     self.window.rootViewController = [[ViewController alloc]init];
    [self.window makeKeyAndVisible];

//    [GLKViewController prefersStatusBarHidden];
    [[UIApplication sharedApplication] setStatusBarHidden:true];

//    GLKView* view=[[GLKView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
//    view.context=[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
//    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
//    view.drawableStencilFormat=GLKViewDrawableStencilFormat8;
//    view.delegate=self;
//    [view setMultipleTouchEnabled:YES];
//    //设置当前上下文
//    [EAGLContext setCurrentContext:view.context];
    
//    [self.window addSubview:view];
    
//    GLKViewController * viewController = [[GLKViewController alloc] initWithNibName:nil bundle:nil]; // 1
//    viewController.view = view; // 2
//    viewController.delegate = self; // 3
//    viewController.preferredFramesPerSecond = 60; // 4
//    self.window.rootViewController = viewController; // 5


//    ios_dirUtil::test();
//    stringUtil::test();
    
    timeUtil::init();
    window::init();
    glslUtil::init();
    uboMgr::initAllUbo();

    testMainBegin();
    return YES;
}

-(void)glkView:(GLKView*)view drawInRect:(CGRect)rect{
//    glClearColor(1.0, 0.0, 0.0, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT);
    
    flyEngine::world::getInstance()->renderOnce();
}

//only works with sceneDelegate
//#pragma mark - UISceneSession lifecycle
/*
- (UISceneConfiguration *)application:(UIApplication *)application configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession options:(UISceneConnectionOptions *)options {
    // Called when a new scene session is being created.
    // Use this method to select a configuration to create the new scene with.
    return [[UISceneConfiguration alloc] initWithName:@"Default Configuration" sessionRole:connectingSceneSession.role];
}


- (void)application:(UIApplication *)application didDiscardSceneSessions:(NSSet<UISceneSession *> *)sceneSessions {
    // Called when the user discards a scene session.
    // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
    // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
}
*/


- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}


@end
