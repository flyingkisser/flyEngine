//
//  ViewController.h
//  gles1
//
//  Created by joe on 10/08/2022.
//

#import <GLKit/GLKit.h>

@interface ViewController : GLKViewController

@property (readwrite, nonatomic) unsigned int _gl_vao;
@property (readwrite, nonatomic) unsigned int _gl_vbo;
@property (nonatomic,strong) GLKBaseEffect *baseEffect;

@end

