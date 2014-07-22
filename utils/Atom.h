#ifdef __APPLE__

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "FileWrapper.h"

#else

#endif

//#include "Shader.h"

// Create a debug log justo for Atom Codes
#ifdef Atom_Debug
#define Atom_Debug(argument) printf(argument);
#endif