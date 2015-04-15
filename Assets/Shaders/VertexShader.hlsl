//Pack matrices so that they're all row major like GLSL
#pragma pack_matrix(row_major)

cbuffer perModel : register(b0)
{
	matrix worldMatrix;
	matrix modelMatrix;
	matrix rotationMatrix;

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
	float2 texCoord		: TEX_COORD0;

	float3 worldNormal	: TEX_COORD1;
	float3 worldPosition: TEX_COORD2;
	float3 eyePosition	: TEX_COORD3;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	// Calculate output position
	output.position = mul(float4(input.position, 1.0f), worldMatrix);
	output.texCoord = input.texCoord;

	//try to do some lighting
	float4 worldCoord4v = mul(float4(input.position, 1.0f),modelMatrix);
	float3 worldCoord3v = float3(worldCoord4v.x / worldCoord4v.w,
								 worldCoord4v.y / worldCoord4v.w,
								 worldCoord4v.z / worldCoord4v.w);

	float3x3 rotation = (float3x3)rotationMatrix;
	float3 worldNormal3v = mul(input.normal, rotation);

	output.worldNormal = worldNormal3v;
	output.worldPosition = worldCoord3v;
	output.eyePosition = eyePoint;

	return output;
}