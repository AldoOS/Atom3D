static const char* PlainDisplayFSH = STRINGIFY
(

varying highp float zDepth;

void main()
{
	gl_FragColor = vec4(zDepth, zDepth, 0.0, 1.0);
}

 );