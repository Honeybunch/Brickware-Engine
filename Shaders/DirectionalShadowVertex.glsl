#version 130
in vec3 vPosition;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main()
{
	mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
	gl_Position = MVP * vec4(vPosition, 1.0);
}
