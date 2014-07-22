//
//  Shader_Wrapper.h
//  OpenglEngine
//
//  Created by harald bregu on 15/07/14.
//
//

#ifndef __OpenglEngine__Shader_Wrapper__
#define __OpenglEngine__Shader_Wrapper__

#include <iostream>

class Shader_Wrapper
{
    enum SHADER_TYPE
    {
        SHADER_TYPE_SIMPLE,
        SHADER_TYPE_TOON,
    };
    
public:
    Shader_Wrapper(SHADER_TYPE shaderType);
    ~Shader_Wrapper();
    
    void useShader(SHADER_TYPE shaderType);
};



#endif /* defined(__OpenglEngine__Shader_Wrapper__) */
