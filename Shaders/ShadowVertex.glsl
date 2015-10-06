#version 130
in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;

uniform mat4 depthVP;
uniform mat4 modelMatrix;

void main()
{
	mat4 MVP = depthVP * modelMatrix;
	gl_Position = MVP * vec4(vPosition, 1.0);
}
