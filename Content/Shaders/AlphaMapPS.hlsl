// globals
Texture2D shaderTextures[3];
SamplerState SampleType;

// structures
struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


float4 main(PixelInput input) : SV_TARGET
{
	float4 color1;
	float4 color2;
	float4 alphaValue;
	float4 blendColor;

	// get the pixel color from the first texture
	color1 = shaderTextures[0].Sample(SampleType,input.tex);
	// get the pixel color from the second texture
	color2 = shaderTextures[1].Sample(SampleType,input.tex);
	// get the alpha value from the alpha map texture
	alphaValue = shaderTextures[2].Sample(SampleType,input.tex);

	// combine the two textures based on the alpha value
	blendColor = (alphaValue * color1) + ((1.0f - alphaValue) * color2);

	// saturate the final color value
	blendColor = saturate(blendColor);

	return blendColor;
}