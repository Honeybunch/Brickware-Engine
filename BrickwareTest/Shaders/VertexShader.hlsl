//Pack matrices so that they're all row major like GLSL
#pragma pack_matrix(row_major)

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

	float3 LightPos		: LIGHT_POS;
	float3 EyePos		: EYE_POS;
	float3 Halfway		: HALFWAY;
	float3 Normal		: NORMAL;
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

	matrix rotation = matrix(modelMatrix[0][0], modelMatrix[1][0], modelMatrix[2][0], 0,
							 modelMatrix[0][1], modelMatrix[1][1], modelMatrix[2][1], 0,
							 modelMatrix[0][2], modelMatrix[1][2], modelMatrix[2][2], 0,
							 0, 0, 0, 1);
	float4 worldNormal4v = mul(float4(input.normal, 1.0), rotation);
	float3 worldNormal3v = float3(worldNormal4v.x / worldNormal4v.w,
								  worldNormal4v.y / worldNormal4v.w,
								  worldNormal4v.z / worldNormal4v.w);

	float3 LightPos = normalize(lightPosition - worldCoord3v);
	float3 EyePos = normalize(eyePoint - worldCoord3v);
	float3 Halfway = normalize(LightPos + EyePos);
	float3 Normal = normalize(worldNormal3v);

	output.LightPos = LightPos;
	output.EyePos = EyePos;
	output.Halfway = Halfway;
	output.Normal = Normal;

	return output;
}