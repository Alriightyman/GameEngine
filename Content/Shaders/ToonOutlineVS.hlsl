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
 
//--------------------------- TOON SHADER PROPERTIES ------------------------------
// The thickness of the lines.  This may need to change, depending on the scale of
// the objects you are drawing.
float LineThickness = .03;
 

// The vertex shader that does the outlines
VertexToPixel main(AppToVertex input)
{
    VertexToPixel output = (VertexToPixel)0;
 
    // Calculate where the vertex ought to be.  This line is equivalent
    // to the transformations in the CelVertexShader.
    float4 original = mul( mul( mul( input.Position, World ), View ), Projection );
 
    // Calculates the normal of the vertex like it ought to be.
    float4 normal = mul(mul(mul(input.Normal, World), View), Projection);
 
    // Take the correct "original" location and translate the vertex a little
    // bit in the direction of the normal to draw a slightly expanded object.
    // Later, we will draw over most of this with the right color, except the expanded
    // part, which will leave the outline that we want.
    output.Position    = original + (mul(0.03, normal));
 
    return output;
}
