
// The constant buffer that holds our "per model" data
// - Each object you draw with this shader will probably have
//   slightly different data (at least for the world matrix)
cbuffer perModel : register( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
};

// Defines what kind of data to expect as input
// - This should match our input layout!
struct VertexShaderInput
{ 
	float3 position		: POSITION;
	float4 color		: COLOR;
};

// Defines the output data of our vertex shader
// - At a minimum, you'll need an SV_POSITION
// - Should match the pixel shader's input
struct VertexToPixel
{
	float4 position		: SV_POSITION;	// System Value Position - Has specific meaning to the pipeline!
	float4 color		: COLOR;
};

// The entry point for our vertex shader
VertexToPixel main( VertexShaderInput input )
{
	// Set up output
	VertexToPixel output;

	// Calculate output position
	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);

	// Pass the color through - will be interpolated per-pixel by the rasterizer
	output.color = input.color;

	return output;
}