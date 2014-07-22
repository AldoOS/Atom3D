//
//  Test01.cpp
//  OpenglEngine
//
//  Created by harald bregu on 04/06/14.
//
//

#include "Test01.h"
#include "Atom.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Shader.h"

#include "tiny_obj_loader.h"
#include "Soil2.h"
#include "Model_Manager.h"

#include "Texture_Manager.h"

#include <time.h>
#include <atomic>
#include <chrono>
#include <future>


//----------------------------Arcball algorithm----------------------------------
glm::vec3 get_arcball_vector(int width, int height, float x, float y)
{
    glm::vec3 P = glm::vec3(1.0 * x/width * 2 - 1.0,
                            1.0 * y/height * 2 - 1.0,
                            0);
    P.y = -P.y;
    float OP_squared = P.x * P.x + P.y * P.y;
    if (OP_squared <= 1*1)
        P.z = sqrt(1*1 - OP_squared);  // Pythagore
    else
        P = glm::normalize(P);  // nearest point
    return P;
}

//--------------------------- Costructor & Destructor -----------------------------------
Test01::Test01()
{
    //printf("Init Test 01 \n");
}

Test01::~Test01()
{
    //printf("Destroy Test 01");
}

GLint a_position;
GLint a_textCoord;
GLint u_mvp;
GLint u_texture;
GLuint u_color;
GLuint u_picking;

//---------------------------- Tests ----------------------------------

//---------------------------- Init ----------------------------------
Files modelFiles;
vector<const char *>file_Tex;

void Test01::init()
{
    index = 0;
    tex_index = 0;
    
    // Insert models to load
    for (int i = 0; i<models.size(); i++)
    {
        modelFiles = loadFiles(models, i);
    }
    
    for (int i = 0; i<textures.size(); i++)
    {
        const char *pathTexture = GetBundlePathFromFileName(textures[i]);
        file_Tex.push_back(pathTexture);
    }
    
    loadTextures(file_Tex);

    
    colorModel = glm::fvec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    glClearColor(236.0f/255.0f,
                 240.0f/255.0f,
                 241.0f/255.0f, 1.0f);
    
    // SHADER
    std::string baseVsh;
    std::string baseFsh;
    baseVsh = GetSourceFromFilename("base.vsh");
    baseFsh = GetSourceFromFilename("base.fsh");
    // SHADER
    ShaderAtm::InitShader2(baseVsh.c_str(), baseFsh.c_str());
    ShaderAtm::LinkShader2();
    a_position = glGetAttribLocation(ShaderAtm::program2, "a_position");
    a_textCoord = glGetAttribLocation(ShaderAtm::program2, "a_texcoord");
    u_mvp = glGetUniformLocation(ShaderAtm::program2, "u_ModelViewProjection");
    u_texture = glGetUniformLocation(ShaderAtm::program2, "Texture");
    u_color = glGetUniformLocation(ShaderAtm::program2, "u_color");
    
    u_picking = glGetUniformLocation(ShaderAtm::program2, "picking");
    
    glUseProgram(ShaderAtm::program2);
    ShaderAtm::ValidateShader2();
}

//---------------------------- Draw Continuous ----------------------------------
void Test01::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    // MODEL VIEW PROJECTION
    glm::mat4 Projection = glm::perspective(45.0f,
                                            (float)width / (float)height,
                                            1.0f, 50.0f);
    glm::mat4 View = glm::lookAt(glm::vec3(0.0f, 0.0f, 7.0f), // eye position
                                 glm::vec3(0.0f, 0.0f, -1.0f), // center
                                 glm::vec3(0.0f, 1.0f, 0.0f)); // up camera
    glm::mat4 Model = glm::mat4(1.0);

    static glm::mat4 arcballRot;

    // Arcball rotation
    if (controller->currentPosition.x != controller->startPosition.x
        || controller->currentPosition.y != controller->startPosition.y)
    {
        glm::vec3 start_ArcballVector = get_arcball_vector(width, height, controller->startPosition.x, controller->startPosition.y);
        glm::vec3 current_ArcballVector = get_arcball_vector(width, height, controller->currentPosition.x, controller->currentPosition.y);
        float angle = acos(glm::min(1.0f, glm::dot(start_ArcballVector, current_ArcballVector)));
        glm::vec3 axis = glm::cross(start_ArcballVector, current_ArcballVector);
        arcballRot = glm::rotate(Model, glm::degrees(angle), axis) * arcballRot;
        Model = arcballRot * Model;
    }
    glm::mat4 MVP = Projection * View * Model;

    // TEXTURE
    bindTexture(tex_index);
    
    // COLOR
    glUniform4f(u_color, colorModel.r, colorModel.g, colorModel.b, colorModel.a);
    
    glUniform1f(u_picking, 0.9);
    
    
    // DRAW
    for (size_t i = 0; i < modelFiles[index].shapes.size(); i++)
    {
        glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, 0, &modelFiles[index].shapes[i].mesh.positions[0]);
        glEnableVertexAttribArray(a_position);
        
        glVertexAttribPointer(a_textCoord, 2, GL_FLOAT, GL_FALSE, 0, &modelFiles[index].shapes[i].mesh.texcoords[0]);
        glEnableVertexAttribArray(a_textCoord);
       
        glUniformMatrix4fv(u_mvp, 1, 0, glm::value_ptr(MVP));
        glDrawElements(GL_TRIANGLES,(int)modelFiles[index].shapes[i].mesh.indices.size(), GL_UNSIGNED_INT, &modelFiles[index].shapes[i].mesh.indices[0]);
    }

//    glm::vec3 un = glm::unProject(glm::vec3(controller->currentPosition.x, controller->currentPosition.y, 0),
//                                  Model,
//                                  Projection,
//                                  glm::vec4(0, 0, width, height));
//    printf("%f %f %f\n", un.x, un.y, un.z);
    
//    GLubyte pixel[3];
//    glReadPixels((int)controller->currentPosition.x, (int)controller->currentPosition.y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void*)pixel);
//    printf("%hhu %hhu %hhu\n", pixel[0], pixel[1], pixel[2]);
    
    GLint viewport[4];
	GLubyte pixel[3];
    
	glGetIntegerv(GL_VIEWPORT,viewport);
    
	glReadPixels(0,viewport[3]-0,1,1,
                 GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);
    
    printf("%hhu\n", pixel[0]);

}

//-------------------------- Tear Down GL ------------------------------------
void Test01::tearDownGL()
{

}






