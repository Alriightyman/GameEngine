 //--------------------------- BASIC PROPERTIES ------------------------------
cbuffer MatrixBuffer
{

	// The world transformation
	float4x4 World;
 
	// The view transformation
	float4x4 View;
 
	// The projection transformation
	float4x4 Projection;
 
	// The transpose of the inverse of the world transformation,
	// used for transforming the vertex's normal
	float4x4 WorldInverseTranspose;
};

//--------------------------- DATA STRUCTURES ------------------------------
// The structure used to store information between the application and the
// vertex shader
struct AppToVertex
{
    float4 Position : POSITION;            // The position of the vertex
    float2 TextureCoordinate : TEXCOORD0;    // The texture coordinate of the vertex
    float3 Normal : NORMAL;                // The vertex's normal
};
 
// The structure used to store information between the vertex shader and the
// pixel shader
struct VertexToPixel
{
    float4 Position : SV_POSITION;
    float2 TextureCoordinate : TEXCOORD0;
    float3 Normal : TEXCOORD1;
};
 
//--------------------------- SHADERS ------------------------------
// The vertex shader that does cel shading.
// It really only does the basic transformation of the vertex location,
// and normal, and copies the texture coordinate over.
VertexToPixel main(AppToVertex input)
{
    VertexToPixel output;
 
    // Transform the position
    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);
 
    // Transform the normal
    output.Normal = normalize(mul(input.Normal, WorldInverseTranspose));
 
    // Copy over the texture coordinate
    output.TextureCoordinate = input.TextureCoordinate;
 
    return output;
}
 
