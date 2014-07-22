
#include "Soil2.h"
#include <vector>

std::vector<GLuint>texes(5);

void loadTextures(vector<const char *>filepaths)
{
    for (int i = 0; i<filepaths.size(); i++) {
        
        texes[i] = SOIL_load_OGL_texture
        (
         filepaths[i],
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID,
         NULL
         );
        
        if (0 == texes[i])
        {
            printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        }
        
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    }
}

void bindTexture(unsigned int index)
{
    glBindTexture(GL_TEXTURE_2D, texes[index]);
    
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

}

void hideTexture()
{
    glBindTexture(GL_TEXTURE_2D, NULL);
}


void disableTexture(bool disable)
{
    if (disable == true) {
        glDisable(GL_TEXTURE_2D);
    }
    if (disable == false) {
        glEnable(GL_TEXTURE_2D);
    }
}
