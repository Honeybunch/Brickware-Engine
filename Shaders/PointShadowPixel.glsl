#version 150 core

in vec4 fragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
	//Distance from the fragment to the light
	float lightDistance = length(fragPos.xyz - lightPos);

	//Map 0 to 1 by dividing by far plane
	lightDistance = lightDistance / farPlane;

	gl_FragDepth = lightDistance;
}
