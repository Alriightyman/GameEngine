
Texture2D shaderTextures[2];
SamplerState SampleType;

struct PixelInput
{
	float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


float4 main(PixelInput input) : SV_TARGET
{
	float4 color1,color2,blendColor;

	// get the pixel color from the first texture
	color1 = shaderTextures[0].Sample(SampleType, input.Tex);
	// get the pixel color from the second texture
	color2 = shaderTextures[1].Sample(SampleType, input.Tex);

	// blend the two pixels together and multiply by the gamma value
	blendColor = color1 * color2 * 2.0f;

	// saturate the final color
	blendColor = saturate(blendColor);

	return blendColor;
}