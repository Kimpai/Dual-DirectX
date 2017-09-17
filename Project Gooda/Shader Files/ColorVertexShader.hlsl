struct VertexShaderInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VertexShaderOutput
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

cbuffer ConstantBuffer : register(b0)
{
    float4 color;
};

VertexShaderOutput main( VertexShaderInput input )
{
    VertexShaderOutput output;
    output.pos = float4(input.pos, 1.0f);
    output.color = input.color * color;

    return output;
}