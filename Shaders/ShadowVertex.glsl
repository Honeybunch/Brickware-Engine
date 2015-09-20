#version 130
in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;

uniform mat4 depthMVP;

void main()
{
	gl_Position = depthMVP * vec4(vPosition, 1.0);
}