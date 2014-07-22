//
//  Shader.h
//  OpenglEngine
//
//  Created by harald bregu on 06/05/14.
//
//

#ifndef OpenglEngine_Shader_h
#define OpenglEngine_Shader_h

namespace ShaderAtm {
    
    GLuint program2, vertShader2, fragShader2;
    
    bool compileShader2(GLuint *shader, GLenum type, const GLchar *source)
    {
        GLint status;
        
        if (!source) {
            printf("Failed to load shaders");
            return false;
        }
        
        *shader = glCreateShader(type);
        glShaderSource(*shader, 1, &source, NULL);
        glCompileShader(*shader);
        
        glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
        
        if (status != GL_TRUE)
        {
            GLint logLength;
            glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0) {
                
                GLchar *log = (GLchar *)malloc(logLength);
                glGetShaderInfoLog(*shader, logLength, &logLength, log);
                printf("Shader compile log:\n%s", log);
                free(log);
            }
        }
        return status == GL_TRUE;
    }
    
    void InitShader2(const char *vshSource, const char *fshSource)
    {
        // Creating a program
        program2 = glCreateProgram();
        
        if (!compileShader2(&vertShader2, GL_VERTEX_SHADER, vshSource)) {
            printf("Failed to compile vertex shader");
        }
        
        if (!compileShader2(&fragShader2, GL_FRAGMENT_SHADER, fshSource)) {
            printf("Failed to compile fragment shader");
        }
        // Attach shader
        glAttachShader(program2, vertShader2);
        glAttachShader(program2, fragShader2);
    }
    
    void DeleteShader2()
    {
        if (vertShader2)
            glDeleteShader(vertShader2);
        
        if (fragShader2)
            glDeleteShader(fragShader2);
        
        if (program2)
            glDeleteProgram(program2);
    }
    
    bool LinkShader2()
    {
        GLint status;
        
        glLinkProgram(program2);
        glValidateProgram(program2);
        
        glGetProgramiv(program2, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
            return false;
        
        if (vertShader2)
            glDeleteShader(vertShader2);
        if (fragShader2)
            glDeleteShader(fragShader2);
        
        return true;
    }
    
    void ValidateShader2()
    {
        GLint logLength;
        
        glValidateProgram(program2);
        glGetProgramiv(program2, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetProgramInfoLog(program2, logLength, &logLength, log);
            printf("Program validate log:\n%s", log);
            free(log);
        }
    }
#pragma mark Shaders
    
}

#endif
