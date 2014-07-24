//
//  ViewController.m
//  Atom3D
//
//  Created by harald bregu on 24/07/14.
//  Copyright (c) 2014 harald bregu. All rights reserved.
//

#import "ViewController.h"
#import "EAGLView.h"

//#include "Shader.hpp"
#include "ShaderProgram.hpp"

#include "Atom.h"

@interface ViewController ()<EAGLViewDelegate>
{
    EAGLView *eaglView;
}
@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    eaglView = [[EAGLView alloc] initWithFrame:self.view.bounds andPreferedRenderer:ESRendererVersion_20 andDepth:NO andAA:NO andNumSamples:0 andRetina:YES];
    [self.view addSubview:eaglView];
    eaglView.delegate = self;
    
    
    
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    
    GLfloat triangle [] =
    {   0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    
    const char *vshFilePath = GetBundlePathFromFileName("newsh.vsh");
    Shader vertexShader(GL_VERTEX_SHADER);
    vertexShader.loadFromFile(vshFilePath);
    vertexShader.compile();
    
    const char *fshFilePath = GetBundlePathFromFileName("newsh.fsh");
    Shader fragmentShader(GL_FRAGMENT_SHADER);
    fragmentShader.loadFromFile(fshFilePath);
    fragmentShader.compile();
    
    
    ShaderProgram *shaderProgram;
    // Set up shader program
    shaderProgram = new ShaderProgram();
    shaderProgram->attachShader(vertexShader);
    shaderProgram->attachShader(fragmentShader);
    shaderProgram->linkProgram();
    
    shaderProgram->addAttribute("a_position");
    GLuint a_position = shaderProgram->attribute("a_position");
    
    shaderProgram->use();
    
    glEnableVertexAttribArray(a_position);
    glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, 0, triangle);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    shaderProgram->disable();
    
    //[eaglView startRender];
    [eaglView drawView];
    [eaglView startAnimation];
   
    
}

-(void)glViewDraw
{
    
}



@end
