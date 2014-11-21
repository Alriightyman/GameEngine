
// gloabals
Texture2D shaderTexture[2];
SamplerState sampleType;

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
};

// structures
struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

float4 main(PixelInput input) : SV_TARGET
{
	float4 textureColor;
	float4 bumpMap;
	float3 bumpNormal;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	textureColor = shaderTexture[0].Sample(sampleType,input.tex);

	bumpMap = shaderTexture[1].Sample(sampleType,input.tex);

	// expand the range of the normal value from (0,+1) to (-1, +1)
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	// calculate the normal from the data in the bumpmap
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);

	bumpNormal = normalize(bumpNormal);

	// invert the light direction for calculations
	lightDir = -lightDirection;

	lightIntensity = saturate(dot(bumpNormal,lightDir));

	color = saturate(diffuseColor * lightIntensity);

	color = color * textureColor;

	// fixes alpha problem
	color[3] = 1.0f;

	return color;
}