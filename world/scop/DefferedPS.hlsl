Texture2D normal_map : register(t0);
Texture2D position_map : register(t1);
Texture2D color_map : register(t2);
Texture2D shadow_map : register(t3);

SamplerState sampler0 : register(s0);

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // test
    float4 color = color_map.Sample(sampler0, input.uv);
    return float4(color.rgb, 1);
}