#version 430

#define pi 3.141592653589793238462643383279

//Attributes
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

//Uniforms
uniform mat4 modelMatrix; 
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPosition;

uniform sampler2D diffuseTexture;

uniform vec3 lookAt;
uniform vec3 eyePoint;
//uniform vec3 up;

//To be passed to the fragment shader
out vec3 LightPos;
out vec3 EyePos;
out vec3 Halfway;
out vec3 Normal;

out vec2 texCoord;

void main()
{
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vPosition, 1.0f);

	//Shading
	vec4 worldCoord4v = modelMatrix * vec4(vPosition, 1.0f);
	vec3 worldCoord3v = vec3(worldCoord4v.x / worldCoord4v.w,
							 worldCoord4v.y / worldCoord4v.w,
							 worldCoord4v.z / worldCoord4v.w);

	mat3 rotation = mat3(modelMatrix);
	vec3 worldNormal3v = rotation * vNormal;

	LightPos = normalize(lightPosition - worldCoord3v);
	EyePos = normalize(eyePoint - worldCoord3v);
	Halfway = normalize(LightPos + EyePos);
	Normal = normalize(worldNormal3v);

	texCoord = vTexCoord;
}

