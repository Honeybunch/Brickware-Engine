#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 texCoord;

uniform sampler2D diffuse;

void main()
{
	gPosition = worldPosition;
	gNormal = normalize(worldNormal);
	gAlbedo = texture(diffuse, texCoord);
}