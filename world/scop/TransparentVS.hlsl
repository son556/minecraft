struct VS_INPUT
{
    float3 pos : POSITION;
    float4 col : COLOR;
};


struct PS_INPUT
{
    float4 pos : SV_Position;
    float z : CLIP_SPACE;
    float4 color : COLOR;
};

cbuffer mvp : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
}

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1);
    output.pos = mul(output.pos, model);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    output.z = output.pos.z;
    output.color = input.col;
	return output;
}