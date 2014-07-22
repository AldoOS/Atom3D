static const char* PlainDisplayVSH = STRINGIFY
(
attribute vec4 position;

varying float zDepth;

uniform mat4 modelViewProjMatrix;


void main()
{

	vec4 newPosition = modelViewProjMatrix * position;
    gl_Position = newPosition;
	zDepth = (6.0 - (1.0 + newPosition.z))/2.0;
}

);