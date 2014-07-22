

uniform lowp float picking;

varying lowp vec2 texCoord0;

uniform sampler2D Texture;
uniform lowp vec4 u_color;

void main() {
    
    if (picking < 1.0) {
        gl_FragColor = vec4(picking, 0.0, 0.0, 1.0);
    } else
    //gl_FragColor = u_color;
    gl_FragColor = u_color * texture2D(Texture, texCoord0);
}