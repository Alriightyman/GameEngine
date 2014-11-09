// globals
cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix projection;
};

// structures
struct VertexInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

PixelInput main( VertexInput input )
{
	PixelInput output;

	input.position.w  = 1.0f;

	output.position = mul(input.position,world);
	output.position = mul(output.position,view);
	output.position = mul(output.position,projection);

	// store the tex coords
	output.tex = input.tex;

	// calculate the normal vector against the world matrix only and then normalize the final value
	output.normal = mul(input.normal,(float3x3)world);
	output.normal = normalize(output.normal);

	// calculate the tangents
	output.tangent = mul(input.tangent,(float3x3)world);
	output.tangent = normalize(output.tangent);
	// calculate the binormals
	output.binormal = mul(input.binormal,(float3x3)world);
	output.binormal = normalize(output.binormal);

	return output;
}