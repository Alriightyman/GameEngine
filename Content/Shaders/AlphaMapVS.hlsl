
// globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// structures
struct VertexInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PixelInput main( VertexInput input )
{
	PixelInput output;

	// change the position vectro to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// calculate the position of the vertex against the world, view, and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position,viewMatrix);
	output.position = mul(output.position,projectionMatrix);

	// store the texture coordinates for the pixel shader
	output.tex = input.tex;

	return output;
}