static const char* ToonFSH = STRINGIFY
(
 
precision highp float;

uniform vec3 DiffuseColor;
uniform vec3 PhongColor;
uniform float Edge;
uniform float Phong;

varying vec3 currentNormal;

void main (void)
{
	vec3 color = DiffuseColor;
	float f = dot(vec3(0,0,1),currentNormal);
	if (abs(f) < Edge)
		color = vec3(0);
	if (f > Phong)
		color = PhongColor;

	gl_FragColor = vec4(color, 1);
}

);