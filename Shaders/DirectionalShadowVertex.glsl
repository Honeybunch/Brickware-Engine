#version 130
in vec3 vPosition;

uniform mat4 depthVP;
uniform mat4 modelMatrix;

void main()
{
	mat4 MVP = depthVP * modelMatrix;
	gl_Position = MVP * vec4(vPosition, 1.0);
}
