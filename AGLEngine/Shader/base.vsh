attribute vec4 a_position;
attribute vec2 a_texcoord;

uniform mat4 u_ModelViewProjection;

varying vec2 texCoord0;

void main() {
    
    gl_Position = u_ModelViewProjection * a_position;
    
    texCoord0 = a_texcoord;
}
