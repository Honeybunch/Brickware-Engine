#version 430

//Light Data
const int maxLights = 4;
struct Light
{
	vec3 position;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

uniform int lightCount;
uniform Light lights[maxLights];
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
uniform sampler2D diffuseTexture;

//Vertex data
in vec3 worldNormal;
in vec3 worldPosition;
in vec3 eyePosition;

void main()
{
	vec3 diffuseProduct;
	vec3 specularProduct;

	//Get the color of the applied texture
	vec4 hue = texture(diffuseTexture, texCoord);

	vec3 finalColor = vec3(0);

	vec3 viewDirection = normalize(eyePosition - worldPosition);

	//Calculate products of every point light applying to this object
	for(int i = 0; i < lightCount; i++)
	{
		Light light = lights[i];

		vec3 lightDir = normalize(light.position - worldPosition);
		vec3 reflectDir = reflect(-lightDir, worldNormal);

		float diffusePower = max(dot(worldNormal, lightDir), 0.0); //TODO replace gloss
		float specularPower = pow(max(dot(viewDirection, reflectDir), 0.0), 128);

		vec3 ambient  = light.ambientColor  * hue.rgb;
		vec3 diffuse  = light.diffuseColor  * diffusePower  * hue.rgb;
		vec3 specular = light.specularColor * specularPower * hue.rgb;

		finalColor += (ambient + diffuse + specular);
	}

    fragColor = vec4(finalColor, hue.a);
}
