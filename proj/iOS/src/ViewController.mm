//
//  ViewController.m
//  gles1
//
//  Created by joe on 10/08/2022.
//
#import <OpenGLES/ES3/glext.h>
#import "ViewController.h"
#include "defines.h"
#include "world.h"
#include "window.h"
#include "ios_dirUtil.h"
#include "logUtil.h"
#include "fingerEvent.h"

#include "testMain.h"
USE_NS_FLYENGINE

static CGPoint s_last_pos={0};
static GLKView* s_view;

@implementation ViewController

+(GLKView*) getView{
    return s_view;
}

- (void)viewDidLoad {
    NSLog(@"ViewController:viewDidLoad start!");
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    GLKView *view  = (GLKView *)self.view;
    s_view=view;
    NSAssert([view isKindOfClass:[GLKView class]], @"ViewController's View is Not A GLKView");
    //创建OpenGL ES2.0上下文
    view.context = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableStencilFormat=GLKViewDrawableStencilFormat8;
    view.userInteractionEnabled = true;
    [view setMultipleTouchEnabled:YES];
    //设置当前上下文
    [EAGLContext setCurrentContext:view.context];

    self.baseEffect = [[GLKBaseEffect alloc]init];
    self.baseEffect.useConstantColor = GL_TRUE;
    self.baseEffect.constantColor = GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f);

    self.preferredFramesPerSecond=60;
    
    //缩放手势识别
    UIPinchGestureRecognizer* zoomRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(onZoom:)];
    //旋转手势识别
    UIRotationGestureRecognizer* rotationRecognizer = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(onRotate:)];
    //滑动手势识别
    UIPanGestureRecognizer* moveRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(onMove:)];
    UITapGestureRecognizer* tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onTap:)];
//    tapRecognizer.numberOfTapsRequired=2;

    // 绑定到指定视图来响应手势
    [self.view addGestureRecognizer:zoomRecognizer];
    [self.view addGestureRecognizer:rotationRecognizer];
    [self.view addGestureRecognizer:moveRecognizer];
    [self.view addGestureRecognizer:tapRecognizer];
}

- (void) onZoom:(UIPinchGestureRecognizer*) sender {
//    CGFloat v = sender.scale; //输出缩放比例的大小数值
//    NSLog(@"onZoom scale:%f", v); //0--5.x
    fingerEvent::getInstance()->onZoom(sender.scale);
}
- (void) onRotate:(UIRotationGestureRecognizer*) sender {
//    CGFloat v = sender.rotation;
//    NSLog(@"onRotate rotation:%f", v);    //0--1.x
    fingerEvent::getInstance()->onRotate(sender.rotation);
}

- (void) onMove:(UIPanGestureRecognizer*) sender {
    CGPoint pos = [sender locationInView:self.view];
//    CGPoint speed = [sender velocityInView:self.view];
//    NSLog(@"onMove pos:%f %f, speed %f %f", pos.x,pos.y,speed.x,speed.y);
    if(s_last_pos.x==0 && s_last_pos.y==0){
        s_last_pos=pos;
        return;
    }
    fingerEvent::getInstance()->onMove(pos.x-s_last_pos.x,pos.y-s_last_pos.y);
    s_last_pos=pos;
    //左>0 右<0 下>0 上<0
    //应根据pos的坐标变化来判断，注意左上是00点
}

- (void)onTap:(UITapGestureRecognizer *)sender{
    if (sender.state == UIGestureRecognizerStateEnded){
        fingerEvent::getInstance()->onTap(2);
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect{
//    [self.baseEffect prepareToDraw];
//    flylog("timeSinceLastUpdate %f %f %f",self.timeSinceLastDraw,self.timeSinceLastUpdate,self.timeSinceFirstResume);
    flyEngine::world::getInstance()->renderOnce();

}

- (void)update{
//    flyEngine::world::getInstance()->renderOnce();
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent *)event {
//    self.paused = !self.paused;
}

//一根或者多根手指在view上移动（随着手指的移动，会持续调用该方法）
-(void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event{
    if(touches.count==1){
        //单指触摸
//        UITouch* touch=touches.anyObject;
//        CGPoint pos=[touch locationInView:nil]; //触摸点在UIWindow的位置
//        CGPoint posOld=[touch previousLocationInView:nil];
//        flylog("cur x %f y %f move %f %f",pos.x,pos.y,pos.x-posOld.x,pos.y-posOld.y);
        return;
    }
    
    //多指触摸
//    float move_x_left=0,move_x_right=0;
//    float move_y_up=0,move_y_down=0;
//    int i=0;
//    for(UITouch* touch:touches){
//        CGPoint pos=[touch locationInView:nil];
//        CGPoint posOld=[touch previousLocationInView:nil];
//        flylog("[t%d]cur x %f y %f move %f %f",i++,pos.x,pos.y,pos.x-posOld.x,pos.y-posOld.y);
//    }
}

//一根或者多根手指离开view（手指抬起）
-(void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event{
    
}

//某个系统事件(例如电话呼入)打断触摸过程
-(void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event{
    
}

- (void)dealloc{
    GLKView *view = (GLKView *)self.view;
    [EAGLContext setCurrentContext:view.context];
    if (__gl_vbo) {
        glDeleteBuffers(1,&__gl_vbo);
        __gl_vbo = 0;
    }
    [EAGLContext setCurrentContext:nil];
}

@end
