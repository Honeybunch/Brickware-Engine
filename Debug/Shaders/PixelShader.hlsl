//Light Data
static const int maxLights = 8;
struct Light
{
	float3 position;

	float3 ambientColor;
	float3 diffuseColor;
	float3 specularColor;
};

//Texture Data
SamplerState MeshTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

//Material Data
struct Material
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 specularColor;

	float gloss;
};

//Constant Data
texture2D diffuseTexture : register(t0);

cbuffer pixelData : register(b1)
{
	int lightCount;
	Light lights[maxLights];
	float3 ambientLight;

	Material material;
}

//Vertex Data
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 texCoord		: TEX_COORD0;

	float3 worldNormal	: TEX_COORD1;
	float3 worldPosition: TEX_COORD2;
	float3 eyePosition	: TEX_COORD3;
};

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 diffuseProduct;
	float3 specularProduct;

	//Get the color of the applied texture
	float4 hue = diffuseTexture.Sample(MeshTextureSampler, input.texCoord);

	float3 finalColor = float3(0, 0, 0);

	float3 viewDirection = normalize(input.eyePosition - input.worldPosition);

	//Calculate products of every point light applying to this object
	for(int i = 0; i < lightCount; i++)
	{
		Light light = lights[i];

		float3 lightDir = normalize(light.position - input.worldPosition);
		float3 reflectDir = reflect(-lightDir, input.worldNormal);

		float diffusePower = saturate(dot(input.worldNormal, lightDir)); //TODO replace gloss
		float specularPower = pow(saturate(dot(viewDirection, reflectDir)), 128);

		float3 ambient = light.ambientColor * hue.rgb;
		float3 diffuse = light.diffuseColor* (diffusePower  * hue.rgb);
		float3 specular = light.specularColor * (specularPower * hue.rgb);
		
		finalColor += (ambient + diffuse + specular);	
	}

	return float4(finalColor, hue.a);
}