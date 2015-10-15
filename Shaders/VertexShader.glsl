#version 330

#define pi 3.141592653589793238462643383279

//Attributes
layout(location = 0)in vec3 vPosition;
layout(location = 1)in vec3 vNormal;
layout(location = 2)in vec2 vTexCoord;

//Uniforms
uniform mat4 worldMatrix;
uniform mat4 modelMatrix;
uniform mat4 rotationMatrix;

uniform mat4 depthBiasVP;
uniform mat4 pointDepthBiasVP;

uniform vec3 lookAt;
uniform vec3 eyePoint;
//uniform vec3 up;

//To be passed to the fragment shader
out vec3 worldNormal;
out vec3 worldPosition;
out vec3 eyePosition;

out vec2 texCoord;
out vec4 shadowCoord;

void main()
{
    gl_Position = worldMatrix * vec4(vPosition, 1.0f);

	//Shading
	vec4 worldCoord4v = modelMatrix * vec4(vPosition, 1.0f);
	mat3 rotation = mat3(rotationMatrix);

	worldNormal = rotation * vNormal;
	worldPosition = vec3(worldCoord4v.x / worldCoord4v.w,
						 worldCoord4v.y / worldCoord4v.w,
						 worldCoord4v.z / worldCoord4v.w);
	eyePosition = eyePoint;

	texCoord = vTexCoord;

	mat4 shadowMVP = depthBiasVP * modelMatrix;
	mat4 pointShadowMVP = pointDepthBiasVP * modelMatrix;

	shadowCoord = (shadowMVP * vec4(vPosition, 1.0f));
}
