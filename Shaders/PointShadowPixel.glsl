#version 130

in vec3 worldPos;

uniform vec3 lightPos;

out float fragColor;

void main()
{
	//Distance from the object's vert to the light
	vec3 lightToVertex = worldPos - lightPos;
	float lightToPixelDistance = length(lightToVertex);

	fragColor = lightToPixelDistance;
}
