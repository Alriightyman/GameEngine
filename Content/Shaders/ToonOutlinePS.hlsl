// The structure used to store information between the vertex shader and the
// pixel shader
struct VertexToPixel
{

    float4 Position : POSITION0;
    float2 TextureCoordinate : TEXCOORD0;
    float3 Normal : TEXCOORD1;
};
//--------------------------- TOON SHADER PROPERTIES ------------------------------
// The color to draw the lines in.  Black is a good default.
float4 LineColor = float4(0, 0, 0, 1.0f);

// The pixel shader for the outline.  It is pretty simple:  draw everything with the
// correct line color.
float4 main(VertexToPixel input) : SV_TARGET
{
    return float4(0, 0, 0, 1.0f);
}