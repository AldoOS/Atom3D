//
//  Test01.h
//  OpenglEngine
//
//  Created by harald bregu on 04/06/14.
//
//

#ifndef __OpenglEngine__Test01__
#define __OpenglEngine__Test01__

#include <iostream>
#include "ES2Render.h"

using namespace std;
#include <glm/glm.hpp>
#include <vector>


class Test01 : public ES2Render
{
public:
    
    Test01();
    ~Test01();
    
    void init();
    void draw();
    void tearDownGL();
private:
};

#endif 



