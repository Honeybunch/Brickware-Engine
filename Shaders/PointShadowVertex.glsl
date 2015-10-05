#version 130
in vec3 vPosition;

uniform mat4 depthVP;
uniform mat4 modelMatrix;

out vec3 worldPos;

void main()
{
	mat4 MVP = depthVP * modelMatrix;
	gl_Position = MVP * vec4(vPosition, 1.0);

	//worldPos = (modelMatrix * vec4(vPosition, 1.0)).xyz;
}
