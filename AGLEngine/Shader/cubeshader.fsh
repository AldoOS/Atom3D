static const char* CubeFSH = STRINGIFY
(

 varying lowp vec2 TexCoordOut;
 uniform sampler2D Texture;

 lowp vec4 DestinationColor;
 
  void main(void) {
      
    DestinationColor = vec4(1.0, 1.0, 1.0, 1.0);
     gl_FragColor = DestinationColor * texture2D(Texture, TexCoordOut); // 3
 }
 
);