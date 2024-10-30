struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D sun_moon : register(t0);
Texture2D skybox : register(t1);
SamplerState sampler0 : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 color;
    color = sun_moon.Sample(sampler0, input.uv);
    if (color.r + color.g + color.b < 1)
        color += skybox.Sample(sampler0, input.uv);
    return color;
}