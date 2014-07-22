#ifndef __OpenglEngine__Controller__
#define __OpenglEngine__Controller__

#include <iostream>
#include <glm/glm.hpp>

typedef glm::vec2 AGLPoint;

// CONTROLLER
struct Controller
{
    glm::vec2 startPosition;
    glm::vec2 currentPosition;
};



// MODEL MATRIX
struct ModelTransform
{
    // Scale model
    float scale;
    
    // Rotate
    struct Rotation
    {
        float axisX, axisY, axisZ;
    };
    struct Rotation Rotation;
    
    // Translate
    struct Translate
    {
        float dirX, dirY, dirZ;
    };
    struct Translate Translate;
};

// VIEW MATRIX (CAMERA)
struct ViewTransform
{
    struct PositionCamera
    {
        float posX, posY, posZ;
    };
    struct PositionCamera PositionCamera;
    
    struct LookToPosition
    {
        float lookToX, lookToY, lookToZ;
    };
    struct LookToPosition LookToPosition;
    
};

// PROJECTION MATRIX
struct ProjectionTransform
{
  
};

#endif
