
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 L : LIGHT_L;
	float3 E : LIGHT_E;
	float3 H : LIGHT_H;
	float3 N : LIGHT_N;
};

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 ambientProduct = float3(.1, .1, .1);
	float3 diffuseProduct = float3(.8, .8, .8);
	float3 specularProduct = float3(.1, .1 , .1);

	float gloss = 75.0;

	//Color is just green for now
	float4 hue = float4(0.0f, 0.5f, 0.0f, 1.0f);

	float Kd = max(dot(input.L, input.N), 0.0);
	float Ks = pow(max(dot(input.N, input.H), 0.0), gloss);

	float3 ambient = hue.rgb * ambientProduct;
	float3 diffuse = hue.rgb * Kd * diffuseProduct;
	float3 specular = float3(1, 1, 1) * Ks * specularProduct;

	if (dot(input.L, input.N) < 0.0)
		specular = float3(0.0, 0.0, 0.0);

	float4 color = float4((ambient + diffuse + specular), hue.a);
	return color;
}