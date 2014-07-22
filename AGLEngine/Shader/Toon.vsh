static const char* ToonVSH = STRINGIFY
(
attribute vec4 position;
attribute vec4 normal;

uniform mat4 modelViewProjMatrix;

varying vec3 currentNormal;

void main(void)
{
	currentNormal = normalize((modelViewProjMatrix * normal).xyz);
	gl_Position = modelViewProjMatrix * position;
}

);