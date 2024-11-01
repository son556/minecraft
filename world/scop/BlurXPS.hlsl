struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer constatData : register(b0)
{
    float dx;
    float dy;
};

static const float weights[5] = 
    { 0.0545, 0.2442, 0.4026, 0.2442, 0.0545 };

float4 main(PS_INPUT input) : SV_TARGET
{
    // Compute Shader X
    float3 color = float3(0, 0, 0);
    
    int i;
    for (i = 0; i < 5; i++)
    {
        color += weights[i] * texture0.Sample(sampler0, 
            input.uv + float2(dx, 0.0) * float(i - 2)).rgb;
    }
    return float4(color, 1.0f);
}