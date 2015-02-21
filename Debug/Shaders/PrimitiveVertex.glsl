#version 430

in vec3 position;

uniform mat4 modelMatrix; 
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vPosition, 1.0f);
}