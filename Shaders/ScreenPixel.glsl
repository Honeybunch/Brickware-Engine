#version 430 core

layout(location = 0) uniform sampler2D gPosition;
layout(location = 1) uniform sampler2D gNormal;
layout(location = 2) uniform sampler2D gAlbedo;

in vec2 uv;
out vec4 fragColor;

uniform vec3 eyePoint;

void main()
{
	vec3 worldPosition = texture(gPosition, uv).rgb;
	vec3 worldNormal = texture(gNormal, uv).rgb;
	vec3 albedo = texture(gAlbedo, uv).rgb;

	vec3 lighting = albedo * 0.1; //Hard ambient
	vec3 viewDir = normalize(eyePoint - worldPosition);

	fragColor = texture(gAlbedo, uv);
}
