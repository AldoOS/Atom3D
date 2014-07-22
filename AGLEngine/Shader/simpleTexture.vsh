attribute vec3 a_position;
attribute vec4 a_color;
uniform mat4 u_ModelViewProjection;

varying vec4 v_color;

void main()
{
    gl_Position = u_ModelViewProjection * vec4 (a_position, 1.0);
    v_color = a_color;
}