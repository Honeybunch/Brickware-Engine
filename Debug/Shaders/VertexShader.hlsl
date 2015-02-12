//Pack matrices so that they're all row major like GLSL
#pragma pack_matrix(row_major)

// The constant buffer that holds our "per model" data
// - Each object you draw with this shader will probably have
//   slightly different data (at least for the world matrix)
cbuffer perModel : register(b0)
{
	matrix modelMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

	float3 lightPosition;

	texture2D tex;

	float3 lookAt;
	float3 eyePoint;
};

// Defines what kind of data to expect as input
// - This should match our input layout!
struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
};

// Defines the output data of our vertex shader
// - At a minimum, you'll need an SV_POSITION
// - Should match the pixel shader's input
struct VertexToPixel
{
	float4 position		: SV_POSITION; // System Value Position - Has specific meaning to the pipeline!
	float3 L : LIGHT_L;
	float3 E : LIGHT_E;
	float3 H : LIGHT_H;
	float3 N : LIGHT_N;
};

// The entry point for our vertex shader
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;

	// Calculate output position
	matrix modelViewProj = mul(mul(modelMatrix, viewMatrix), projectionMatrix);
	output.position = mul(float4(input.position, 1.0f), modelViewProj);

	//try to do some lighting
	float4 worldCoord4v = mul(modelMatrix, float4(input.position, 1.0f));
	float3 worldCoord3v = float3(worldCoord4v.x / worldCoord4v.w,
								 worldCoord4v.y / worldCoord4v.w,
								 worldCoord4v.z / worldCoord4v.w);

	matrix rotation = matrix(modelMatrix[0][0], modelMatrix[1][0], modelMatrix[2][0], 0,
							 modelMatrix[0][1], modelMatrix[1][1], modelMatrix[2][1], 0,
							 modelMatrix[0][2], modelMatrix[1][2], modelMatrix[2][2], 0,
							 0, 0, 0, 1);
	float4 worldNormal4v = mul(rotation,  float4(input.normal, 1.0));
	float3 worldNormal3v = float3(worldNormal4v.x / worldNormal4v.w,
								  worldNormal4v.y / worldNormal4v.w,
								  worldNormal4v.z / worldNormal4v.w);

	float3 L = normalize(lightPosition - worldCoord3v);
	float3 E = normalize(eyePoint - worldCoord3v);
	float3 H = normalize(L + E);
	float3 N = normalize(worldNormal3v);

	output.L = L;
	output.E = E;
	output.H = H;
	output.N = N;

	return output;
}