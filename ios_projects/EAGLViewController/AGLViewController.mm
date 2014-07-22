//
//  AGLViewController.m
//  OpenglEngine
//
//  Created by harald bregu on 09/04/14.
//
//

#import "AGLViewController.h"

#include "Atom.h"
#import "GLView.h"

@interface AGLViewController ()<UIGestureRecognizerDelegate>
{
    GLView *glView;
}

@end
 
@implementation AGLViewController

-(void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    
    
    
    glView = [[GLView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:glView];
    
    glView.models = [@[@"house.obj", @"cube.obj", @"hheli.obj", @"jeep.obj", @"torus.obj"] mutableCopy];
    glView.textures = [@[ @"house_diffuse.png", @"squared_metal.png", @"hheli.png", @"jeep_rood.png", @"heightmap.png"] mutableCopy];
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        //glView.color_Model = [UIColor greenColor];
    });
    
    UISegmentedControl *segment = [[UISegmentedControl alloc] initWithItems:@[@"uno", @"due", @"tre", @"quatro", @"cinque"]];
    segment.frame = CGRectMake(0, 400, 300, 40);
    [glView addSubview:segment];
    [segment addTarget:self action:@selector(segment:) forControlEvents:UIControlEventValueChanged];
    segment.selectedSegmentIndex = 0;
}



-(void)segment:(UISegmentedControl*)segmentcontrol
{
    NSUInteger selectedIndex = segmentcontrol.selectedSegmentIndex;
        
    glView.model_Index = selectedIndex;
    glView.texture_Index = selectedIndex;
}

@end
