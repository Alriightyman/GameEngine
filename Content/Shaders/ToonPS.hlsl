
//--------------------------- DIFFUSE LIGHT PROPERTIES ------------------------------
cbuffer LightBuffer
{ 

	// The color of the diffuse light
	float4 DiffuseColor;// = float4(1, 1, 1, 1);
 	// The direction of the diffuse light
	float3 DiffuseLightDirection;// = float3(1, 0, 0);
	// The intensity of the diffuse light
	float DiffuseIntensity;// = 1.0;
};
 //--------------------------- TEXTURE PROPERTIES ------------------------------
// The texture being used for the object
Texture2D Texture;
 
// The texture sampler, which will get the texture color
SamplerState textureSampler
{
    Texture = (Texture);
    MinFilter = Linear;
    MagFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};
 

// The structure used to store information between the vertex shader and the
// pixel shader
struct VertexToPixel
{
    float4 Position : POSITION0;
    float2 TextureCoordinate : TEXCOORD0;
    float3 Normal : TEXCOORD1;
};
 
// The pixel shader that does cel shading.  Basically, it calculates
// the color like is should, and then it discretizes the color into
// one of four colors.
float4 main(VertexToPixel input) : SV_TARGET
{
    // Calculate diffuse light amount
    float intensity = dot(normalize(DiffuseLightDirection), input.Normal);
    if(intensity < 0)
        intensity = 0;
 
    // Calculate what would normally be the final color, including texturing and diffuse lighting
    float4 color = Texture.Sample(textureSampler,input.TextureCoordinate) * DiffuseColor * DiffuseIntensity;
		//tex2D(textureSampler, input.TextureCoordinate) * DiffuseColor * DiffuseIntensity;
    color.a = 1;
 
    // Discretize the intensity, based on a few cutoff points
    if (intensity > 0.95)
        color = float4(1.0,1,1,1.0) * color;
    else if (intensity > 0.5)
        color = float4(0.7,0.7,0.7,1.0) * color;
    else if (intensity > 0.05)
        color = float4(0.35,0.35,0.35,1.0) * color;
    else
        color = float4(0.1,0.1,0.1,1.0) * color;
 
    return color;
}
