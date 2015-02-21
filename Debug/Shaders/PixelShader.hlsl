
texture2D diffuseTexture : register(t0);

SamplerState MeshTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 texCoord		: TEX_COORD;

	float3 LightPos		: LIGHT_POS;
	float3 EyePos		: EYE_POS;
	float3 Halfway		: HALFWAY;
	float3 Normal		: NORMAL;
};

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 ambientProduct = float3(.1, .1, .1);
	float3 diffuseProduct = float3(.8, .8, .8);
	float3 specularProduct = float3(.1, .1 , .1);

	float gloss = 75.0;
	
	//Calculate the color for this pixel based on the texture
	float4 hue = diffuseTexture.Sample(MeshTextureSampler, input.texCoord);

	float Kd = saturate(dot(input.LightPos, input.Normal));
	float Ks = pow(saturate(dot(input.Normal, input.Halfway)), gloss);

	float3 ambient = hue.rgb * ambientProduct;
	float3 diffuse = hue.rgb * Kd * diffuseProduct;
	float3 specular = Ks * specularProduct;

	if (dot(input.LightPos, input.Normal) < 0.0)
		specular = float3(0.0, 0.0, 0.0);

	float3 color = float3(ambient + diffuse + specular);
	return float4(color, hue.a);
}