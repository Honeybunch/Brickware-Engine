#version 130 

//Directional Light data
struct DirectionalLight
{
	vec3 direction;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

uniform DirectionalLight directionalLight;

//Point Light Data
const int maxPointLights = 4;
struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

uniform int pointLightCount;
uniform PointLight pointLights[maxPointLights];
uniform vec3 ambientLight = vec3(0);

//Material data
struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float gloss;
};
uniform Material material;

//Texture Data
out vec4 fragColor;

in vec2 texCoord;
in vec4 shadowCoord;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

//Vertex data
in vec3 worldNormal;
in vec3 worldPosition;
in vec3 eyePosition;

float CalcDirShadows()
{
	//Perspective divide
	vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
	//Get into 0-1 range
	projCoords = projCoords * 0.5 + 0.5;

	float bias = 0.0005;

	float visibility = 0.5;
	if (texture(shadowMap, projCoords.xy).z < projCoords.z - bias)
		visibility = 1.0;

	return visibility;
}

//Calcuate the light on this object from the scene's directional light
vec3 CalcDirectionalLight(DirectionalLight light, vec4 hue, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(-lightDir, worldNormal);

	float diffusePower = max(dot(worldNormal, lightDir), 0.0);
	float specularPower = pow(max(dot(viewDir, reflectDir), 0.0), 128);

	vec3 ambient = light.ambientColor * hue.rgb;
	vec3 diffuse = light.diffuseColor * diffusePower * hue.rgb;
	vec3 specular = light.specularColor * specularPower * hue.rgb;

	//Calculate shadows
	float shadow = CalcDirShadows();

	vec3 dirLight = (ambient + (1.0 - shadow) * (diffuse + specular));

	return  dirLight;
}

//Calculate the light on this object from the scene's point lights
vec3 CalcPointLight(PointLight light, vec4 hue, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - worldPosition);
	vec3 reflectDir = reflect(-lightDir, worldNormal);

	float diffusePower = max(dot(worldNormal, lightDir), 0.0); //TODO replace gloss
	float specularPower = pow(max(dot(viewDir, reflectDir), 0.0), 128);

	vec3 ambient  = light.ambientColor  * hue.rgb;
	vec3 diffuse  = light.diffuseColor  * diffusePower  * hue.rgb;
	vec3 specular = light.specularColor * specularPower * hue.rgb;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 diffuseProduct;
	vec3 specularProduct;

	//Get the color of the applied texture
	vec4 hue = texture(diffuseTexture, texCoord);

	vec3 finalColor = vec3(0);

	vec3 viewDirection = normalize(eyePosition - worldPosition);

	//Calculate directional light
	finalColor += CalcDirectionalLight(directionalLight, hue, viewDirection);

	//Calculate products of every point light applying to this object
	for(int i = 0; i < pointLightCount; i++)
	{
		PointLight pointLight = pointLights[i];

		finalColor += CalcPointLight(pointLight, hue, viewDirection);
	}

	fragColor = vec4(finalColor, hue.a);
}
