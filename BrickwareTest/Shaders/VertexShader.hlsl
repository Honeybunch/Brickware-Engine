//Pack matrices so that they're all row major like GLSL
#pragma pack_matrix(row_major)

cbuffer perModel : register(b0)
{
	matrix modelMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

	texture2D tex;

	float3 lookAt;
	float3 eyePoint;
};

struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 texCoord		: TEX_COORD;

	float3 worldNormal	: WORLD_NORMAL;
	float3 worldPosition: WORLD_POS;
	float3 eyePosition	: EYE_POS;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	// Calculate output position
	matrix modelViewProj = mul(mul(modelMatrix, viewMatrix), projectionMatrix);
	output.position = mul(float4(input.position, 1.0f), modelViewProj);
	output.texCoord = input.texCoord;

	//try to do some lighting
	float4 worldCoord4v = mul(float4(input.position, 1.0f),modelMatrix);
	float3 worldCoord3v = float3(worldCoord4v.x / worldCoord4v.w,
								 worldCoord4v.y / worldCoord4v.w,
								 worldCoord4v.z / worldCoord4v.w);

	float3x3 rotation = (float3x3)modelMatrix;
	float3 worldNormal3v = mul(input.normal, rotation);

	output.worldNormal = worldNormal3v;
	output.worldPosition = worldCoord3v;
	output.eyePosition = eyePoint;

	return output;
}