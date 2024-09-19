cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

cbuffer LMVP : register(b1)
{
    matrix l_world;
    matrix l_view;
    matrix l_proj;
}

struct VS_INPUT
{
    int type : TYPE;
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
};

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    float3 lpos : L_POSITION;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.type = input.type;
    output.pos = float4(input.pos, 1);
    output.world_pos = input.pos;
    output.uv = input.uv;
    output.dir = input.dir;
    
    //output.x_pos = input.x_pos;
    
    output.pos = mul(output.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    // shadow 용
    float4 tmp = float4(input.pos, 1);
    tmp = mul(tmp, l_view);
    tmp = mul(tmp, l_proj);
    output.lpos = tmp.xyz / tmp.w;
    return output;
}