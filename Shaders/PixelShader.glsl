#version 330

//Directional Light data
struct DirectionalLight
{
	vec3 direction;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float shadowStrength;
	float shadowBias;
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

	float shadowStrength;
	float shadowBias;
	float farPlane;
};

uniform int pointLightCount;
uniform PointLight pointLights[maxPointLights];
uniform vec3 ambientLight = vec3(0);

//Texture Data
out vec4 fragColor;

in vec2 texCoord;
in vec4 shadowCoord;

uniform float shadowStrength;
uniform float shadowBias;

//Vertex data
uniform sampler2D a_worldPosition;
uniform sampler2D b_worldNormal;
uniform sampler2D c_albedo;

//Shadow maps
uniform sampler2D d_shadowMap;
uniform samplerCube e_pointShadowMap;

uniform vec3 eyePosition;

float CalcDirShadows(DirectionalLight light)
{
	//Perspective divide
	vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
	//Get into 0-1 range
	projCoords = projCoords * 0.5 + 0.5;

	float visibility = 0.0;

	float closestDepth = texture(d_shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z - light.shadowBias;

	float shadow = currentDepth > closestDepth ? 0.0 : light.shadowStrength;

	return shadow;
}

float CalcPointShadows(PointLight light, vec3 worldPosition)
{
	// Get vector between fragment position and light position
	vec3 fragToLight = worldPosition - light.position;
	// Use the light to fragment vector to sample from the depth map
	float closestDepth = texture(e_pointShadowMap, fragToLight).r;
	// It is currently in linear range between [0,1]. Re-transform back to original value
	closestDepth *= light.farPlane;
	// Now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight) - light.shadowBias;
	// Now test for shadows
	float shadow = currentDepth > closestDepth ? 0.0 : light.shadowStrength;

	return shadow;
}

//Calcuate the light on this object from the scene's directional light
vec3 CalcDirectionalLight(DirectionalLight light, vec3 worldPosition, vec3 worldNormal, vec4 albedo, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(-lightDir, worldNormal);

	float diffusePower = max(dot(worldNormal, lightDir), 0.0);
	float specularPower = pow(max(dot(viewDir, reflectDir), 0.0), 128);

	vec3 color = albedo.rgb;

	vec3 ambient  = light.ambientColor  * color;
	vec3 diffuse  = light.diffuseColor  * diffusePower * color;
	vec3 specular = light.specularColor * specularPower * color;

	//Calculate shadows
	float shadow = CalcDirShadows(light);

	vec3 dirLight = (ambient + shadow * (diffuse + specular));

	return dirLight;
}

//Calculate the light on this object from the scene's point lights
vec3 CalcPointLight(PointLight light, vec3 worldPosition, vec3 worldNormal, vec4 albedo, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - worldPosition);
	vec3 reflectDir = reflect(-lightDir, worldNormal);

	float diffusePower = max(dot(worldNormal, lightDir), 0.0); //TODO replace gloss
	float specularPower = pow(max(dot(viewDir, reflectDir), 0.0), 128);

	vec3 ambient  = light.ambientColor  * albedo.rgb;
	vec3 diffuse  = light.diffuseColor  * diffusePower  * albedo.rgb;
	vec3 specular = light.specularColor * specularPower * albedo.rgb;

	//Calc shadows
	float shadow = CalcPointShadows(light, worldPosition);

	vec3 pointLight = (ambient + shadow * (diffuse + specular));

	return pointLight;
}

void main()
{
	//Get data out of samplers
	vec3 worldPosition = texture(a_worldPosition, texCoord).rgb;
	vec3 worldNormal = texture(b_worldNormal, texCoord).rgb;
	vec4 albedo = texture(c_albedo, texCoord);

	vec3 diffuseProduct;
	vec3 specularProduct;

	vec3 finalColor = vec3(0);

	vec3 viewDirection = normalize(eyePosition - worldPosition);

	//Calculate directional light
	finalColor += CalcDirectionalLight(directionalLight, worldPosition, worldNormal, albedo, viewDirection);

	//Calculate products of every point light applying to this object
	for(int i = 0; i < pointLightCount; i++)
	{
		PointLight pointLight = pointLights[i];

		finalColor += CalcPointLight(pointLight, worldPosition, worldNormal, albedo, viewDirection);
	}

	fragColor = vec4(finalColor, albedo.a);
}
