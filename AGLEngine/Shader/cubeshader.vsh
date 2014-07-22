static const char* CubeVSH = STRINGIFY
(
attribute vec4 position;
uniform mat4 ModelViewProjection;
 
attribute vec2 TexCoordIn;
varying vec2 TexCoordOut;

void main(void) {
    
    gl_Position = ModelViewProjection * position;
    TexCoordOut = TexCoordIn;
}
);