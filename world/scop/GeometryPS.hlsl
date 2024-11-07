SamplerState sampler0 : register(s0);
Texture2DArray texture_arr : register(t0);

struct PS_INPUT
{
    int tex_arr_idx : INDEX;
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 pbr_normal : PBR_NORMAL;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    int lod : LEVEL;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
    float4 position : SV_Target2;
    float4 pbr_normal : SV_Target3;
};

cbuffer eyePos : register(b0)
{
    float3 pos;
    float r;
    matrix view;
};

PS_OUTPUT main(PS_INPUT input)
{
    float4 color;
    float3 uvw;
    PS_OUTPUT output;
    
    output.normal = float4(mul(input.normal, (float3x3) view), 1);
    output.pbr_normal = 
        float4(mul(input.pbr_normal, (float3x3) view), 1);
    output.position = float4(input.world_pos, 1);
    output.position = mul(output.position, view);
    uvw = float3(input.uv, input.tex_arr_idx);
    
    output.color = texture_arr.SampleLevel(sampler0, uvw, 0);
    
    return output;
}