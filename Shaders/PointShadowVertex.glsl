#version 150 core
in vec3 vPosition;

uniform mat4 modelMatrix;

void main()
{
	//Just translate to model space; geometry shader will take care of view and projection
	gl_Position = modelMatrix * vec4(vPosition, 1.0);
}
