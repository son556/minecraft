struct PS_INPUT
{
    float4 pos : SV_Position;
    int shadow_flag : SHADOW;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 shadow = input.shadow_flag * float3(1, 1, 1);
	return float4(shadow, 1.0f);
}