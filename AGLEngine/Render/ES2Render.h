//
//  ES2Render.h
//  OpenglEngine
//
//  Created by harald bregu on 06/05/14.
//
//

#ifndef OpenglEngine_ES2Render_h
#define OpenglEngine_ES2Render_h

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#include "Controller.h"
//#include "Atm_Obj_Loader.h"

#include <vector>
using namespace std;

class ES2Render
{

public:
    // Init Draw
    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void tearDownGL() = 0;
    
    // Controller
    struct Controller *controller;
    struct ModelTransform *modelTransform;
    struct ViewTransform  *viewTransform;
    struct ProjectionTransform *projectionTransform;

    // Width and height of renderbuffer
    unsigned int width;
    unsigned int height;
    
    vector<const char*>models;
    vector<const char*>textures;
    int tex_index;
    int index;

    glm::fvec4 colorModel;
};

#endif
