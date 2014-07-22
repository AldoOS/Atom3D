//
//  GLView.h
//  ShaderExamples
//
//  Created by Brad Larson on 11/13/2010.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>


@interface GLView : UIView

// Add Models and Textures filenames to load
@property (nonatomic, strong) NSMutableArray *models;
@property (nonatomic, strong) NSMutableArray *textures;

// Show a model and a texture from the array
@property (nonatomic, assign) NSUInteger model_Index;
@property (nonatomic, assign) NSUInteger texture_Index;

@property (nonatomic, assign) UIColor *color_Model;

-(void)reloadDraw;
-(void)changeModel;

// Frame Buffer
@property (readonly) GLuint offscreenFramebuffer, offScreenRenderTexture;
-(BOOL)createFrameBuffer;
-(BOOL)presentRenderBuffer;
-(void)switchToDisplayFramebuffer;
-(void)destroyFramebuffer;

@end
