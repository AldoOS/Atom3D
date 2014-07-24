//
//  GLView.m
//  ShaderExamples
//
//  Created by Brad Larson on 11/13/2010.
//

#import "GLView.h"
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#include "ES2Render.h"
#include "Test01.h"

#define MSAA 0

@interface GLView () <UIGestureRecognizerDelegate>
{
    EAGLContext *context;
    
    GLint backingWidth, backingHeight;
    GLuint offscreenFramebuffer, offscreenColorRender, offScreenDepthRender;
    GLuint offScreenRenderTexture;
    GLuint frameBuffer, colorRender, depthRender;
    GLuint msaaFramebuffer, msaaColorRender, msaaDepthRender;
    
    CADisplayLink *displayLink;
    BOOL MSAAEnabled;
    
    Controller controller;
    ModelTransform modelTransform;
}

@property (nonatomic, assign) ES2Render *es2Render;

@end

@implementation GLView
+ (Class) layerClass 
{
	return [CAEAGLLayer class];
}

-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (!context || ![EAGLContext setCurrentContext:context] || ![self createFrameBuffer]) {
            return nil;
        }

        self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
        
        //[self loadGestures];
    }
    return self;
}

#pragma mark - Framebuffer
-(BOOL)createFrameBuffer
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    
    // OFFSCREEN FRAMEBUFFER
    glGenFramebuffers(1, &offscreenFramebuffer); // FRAMEBUFFER
    glBindFramebuffer(GL_FRAMEBUFFER, offscreenFramebuffer);
    glGenRenderbuffers(1, &offscreenColorRender); // COLOR RENDERBUFFER
    glBindRenderbuffer(GL_RENDERBUFFER, offscreenColorRender);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8_OES, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, offscreenColorRender);
    glGenRenderbuffers(1, &offScreenDepthRender); // DEPTH RENDERBUFFER
    glBindRenderbuffer(GL_RENDERBUFFER, offScreenDepthRender);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, offScreenDepthRender);
    
    // OFFSCREEN FRAMEBUFFER TEXTURE TARGET
    glGenTextures(1, &offScreenRenderTexture); // TEXTURE
    glBindTexture(GL_TEXTURE_CUBE_MAP, offScreenRenderTexture);
    glTexParameteri( GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    for (int f = 0; f < 6; ++f) {
        GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + f;
        glTexImage2D(face, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
    // ??
    //	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP, offscreenRenderTexture, 0);
    
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		NSLog(@"Incomplete FBO: %d", status);
        exit(1);
    }
    
    // ONSCREEN FRAMEBUFFER
    glGenFramebuffers(1, &frameBuffer); // FRAMEBUFFER
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glGenRenderbuffers(1, &colorRender); // COLOR RENDERBUFFER
    glBindRenderbuffer(GL_RENDERBUFFER, colorRender);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRender);

#ifdef MSAA
    // Multisampled antialiasing
    glGenFramebuffers(1, &msaaFramebuffer);
    glGenRenderbuffers(1, &msaaColorRender);
    
    glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, msaaColorRender);
    
    // 4X MSAA
    glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_RGBA8_OES, backingWidth, backingHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaColorRender);
    
    glGenRenderbuffers(1, &msaaDepthRender); // DEPTH RENDERBUFFER
    glBindRenderbuffer(GL_RENDERBUFFER, msaaDepthRender);
    glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, msaaDepthRender);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		return NO;
	}
#else
    glGenRenderbuffers(1, &depthRender);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRender);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRender);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		NSLog(@"Failure with framebuffer generation");
		return NO;
	}
#endif
    
    glViewport(0, 0, backingWidth, backingHeight);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		NSLog(@"Incomplete FBO: %d", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }
    
    return YES;
}

-(BOOL)presentRenderBuffer
{
    BOOL success = NO;
    
    if (context != nil)
    {
        //[EAGLContext setCurrentContext:context];
#ifdef MSAA
        glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, msaaFramebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, frameBuffer);
        glResolveMultisampleFramebufferAPPLE();
#endif
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRender);
        success = [context presentRenderbuffer:GL_RENDERBUFFER];
       
#ifdef MSAA
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
#endif
    }
    
    return success;
}

- (void)switchToDisplayFramebuffer;
{
	glViewport(0, 0, backingWidth, backingHeight);
    
#ifdef MSAA
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
#endif
}

- (void)destroyFramebuffer;
{
	if (frameBuffer)
	{
		glDeleteFramebuffers(1, &frameBuffer);
		frameBuffer = 0;
	}
	
	if (colorRender)
	{
		glDeleteRenderbuffers(1, &colorRender);
		colorRender = 0;
	}
    
	if (depthRender)
	{
		glDeleteRenderbuffers(1, &depthRender);
		depthRender = 0;
	}
    
    NSLog(@"Framebuffer is destroyed");
}


#pragma mark - Laying out Subviews
-(void)layoutSubviews
{
    [super layoutSubviews];
    
    // Init object
    [self initObject];

    // Draw object with loop or drag
    [self drawWithStartPos:CGPointMake(0, 0) andCurrentPosition:CGPointMake(0, 0)];
    
    // Refresh framebuffer (not working for Device)
    /*
     [EAGLContext setCurrentContext:context];
     [self destroyFramebuffer];
     [self createFrameBuffer];
     */
}

-(void)reloadDraw
{
    [self drawWithStartPos:CGPointMake(0, 0) andCurrentPosition:CGPointMake(0, 0)];
}

-(void)changeModel
{
    self.es2Render->draw();
    [self presentRenderBuffer];
}

-(NSMutableArray *)models
{
    if (!_models) _models = [[NSMutableArray alloc] init];
    return _models;
}

-(NSMutableArray *)textures
{
    if (!_textures) _textures = [[NSMutableArray alloc] init];
    return _textures;
}

-(void)setModel_Index:(NSUInteger)model_Index
{
    _model_Index = model_Index;
    self.es2Render->index = _model_Index;
    [self changeModel];
}

-(void)setTexture_Index:(NSUInteger)texture_Index
{
    _texture_Index = texture_Index;
    self.es2Render->tex_index = _texture_Index;
    [self changeModel];
}

-(void)setColor_Model:(UIColor *)color_Model
{
    _color_Model = color_Model;

    const CGFloat *components = CGColorGetComponents([_color_Model CGColor]);

    self.es2Render->colorModel.r = components[0];
    self.es2Render->colorModel.g = components[1];
    self.es2Render->colorModel.b = components[2];
    self.es2Render->colorModel.a = components[3];

    [self changeModel];
}


-(void)dealloc
{
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [self destroyFramebuffer];
}

#pragma mark - Init & Render
-(BOOL)initObject
{
    if(!self.es2Render) {
        self.es2Render = new Test01;
        // Set the name of the model
        
        for (int i = 0; i < _models.count; i++)
        {
            self.es2Render->models.push_back([_models[i] UTF8String]);
        }
        
        for (int t = 0; t<_textures.count; t++)
        {
            self.es2Render->textures.push_back([_textures[t] UTF8String]);
        }
        
        //rself.es2Render->fileNameModel = [[self.models objectAtIndex:0] UTF8String];
        self.es2Render->init();
        self.es2Render->controller = &controller;
        self.es2Render->modelTransform = &modelTransform;
    }
    return YES;
}

-(void)drawWithStartPos:(CGPoint)startPosition andCurrentPosition:(CGPoint)currentPosition
{

    // 1 - Get width and height
    self.es2Render->width = backingWidth;
    self.es2Render->height = backingHeight;

    // 2. Get coordinates
    controller.startPosition.x = startPosition.x;
    controller.startPosition.y = startPosition.y;
    controller.currentPosition.x = currentPosition.x;
    controller.currentPosition.y = currentPosition.y;
    
    //modelTransform.scale = 1.0;

    
    // 3 - Draw object
    self.es2Render->draw();

    // 4 - Present renderbuffer
    [self presentRenderBuffer];
}

#pragma mark - Touch events responder
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];

    //CGPoint currentPosition = [[touches anyObject] locationInView:self];

}


-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    CGPoint startPosition = [[touches anyObject] previousLocationInView:self];
    CGPoint currentPosition = [[touches anyObject] locationInView:self];
    
    
    [self drawWithStartPos:startPosition andCurrentPosition:currentPosition];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesEnded:touches withEvent:event];
    
}

#pragma mark - Gesture Reconizer
/*-(void)loadGestures
{
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(pan:)];
    [self addGestureRecognizer:pan];
//    pan.delegate = self;
//    pan.minimumNumberOfTouches = 2;
//    UIRotationGestureRecognizer *rotate = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(rotate:)];
//    [self addGestureRecognizer:rotate];
//    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tap:)];
//    [self addGestureRecognizer:tap];
//    tap.numberOfTapsRequired = 2;
//    tap.numberOfTouchesRequired = 2;
    UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinch:)];
    [self addGestureRecognizer:pinch];
}

-(void)pinch:(UIPinchGestureRecognizer *)pinchGesture
{
    float scale = [pinchGesture scale];
    //modelTransform.scale = scale;
    //printf("scale :%f", modelTransform.scale);
}

-(void)pan:(UIPanGestureRecognizer *)panGesture
{
    
    if (panGesture.state == UIGestureRecognizerStateBegan)
    {
        //startPosition = [panGesture locationInView:self];
    }
    else if (panGesture.state == UIGestureRecognizerStateChanged)
    {
        
//        CGPoint location = [panGesture locationInView:panGesture.view];
//        CGPoint translation = [panGesture translationInView:self];
        
        
        //float x = location.x * (360 / panGesture.view.frame.size.width);
        
        
        // Pan (1 Finger)
        if(panGesture.numberOfTouches == 1)
        {
            
            //NSLog(@"End");
            //rot = x * 180;
            //            modelTransform.Rotation.axisY = rotY;
            //            rotY = x * 180;
            //            viewTransform.PositionCamera.posY += y;
            //            //[self startRenderLoop];
        }
    }
    else if (panGesture.state == UIGestureRecognizerStateEnded)
    {
    }
    
    //NSLog(@"%f", startPosition.x);

}


-(void)rotate:(UIRotationGestureRecognizer *)rotateGesture
{
    //    float rotation = [rotateGesture rotation];
    //    printf("%f", rotation);
}

-(void)tap:(UITapGestureRecognizer *)tapGesture
{

    if (tapGesture.state == UIGestureRecognizerStateBegan) {
        //NSLog(@"Begin");
//        printf("%f", [tapGesture locationInView:tapGesture.view].x);
    }
//
//    if (tapGesture.state == UIGestureRecognizerStateEnded) {
//        //displayLink.paused = NO;
//    }
}


*/

#pragma mark - Rendering Loop
/*- (void)startRenderLoop
{
    if(displayLink == nil)
    {
        displayLink = [self.window.screen displayLinkWithTarget:self selector:@selector(renderByrotating:rX:rZ:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        NSLog(@"Start Render Loop");
    }
}

- (void)stopRenderLoop
{
    if (displayLink != nil) {
        [displayLink invalidate];
        displayLink = nil;
        NSLog(@"Stopping Render Loop");
    }
}*/

#pragma mark - Offscreen framebuffer
@synthesize offscreenFramebuffer, offScreenRenderTexture;

@end

