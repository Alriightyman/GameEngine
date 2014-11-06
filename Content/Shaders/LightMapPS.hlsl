
Texture2D shaderTextures[2];
SamplerState SampleType;

struct PixelInput
{
	float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


float4 main(PixelInput input) : SV_TARGET
{
	float4 color,lightColor,finalColor;

	// get the pixel color from the first texture
	color = shaderTextures[0].Sample(SampleType, input.Tex);
	// get the pixel color from the light map
	lightColor = shaderTextures[1].Sample(SampleType, input.Tex);

	// blend the two pixels together
	finalColor = color * lightColor;

	return finalColor;
}