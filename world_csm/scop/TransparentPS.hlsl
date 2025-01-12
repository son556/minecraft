struct PS_INPUT
{
    float4 pos : SV_Position;
    float z : CLIP_SPACE;
    float4 color : COLOR;
};


struct PS_OUTPUT
{
    float4 accum : SV_Target0;
    float reveal : SV_Target1;
};

Texture2D tex_depth : register(t0);
SamplerState sampler0 : register(s0);

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    float2 texcoord;
    texcoord.x = (input.pos.x + 1.0f) * 0.5f;
    texcoord.y = (-input.pos.y + 1.0f) * 0.5f;
    if (input.pos.z > tex_depth.Sample(sampler0, texcoord).r)
        discard;
    
    // weight function
    float weight = 
        clamp(pow(min(1.0, input.color.a * 10.0) + 0.01, 3.0) * 1e8 * 
            pow(1.0 - input.z * 0.9, 3.0), 1e-2, 3e3);
	
	// store pixel color accumulation
    output.accum = 
        float4(input.color.rgb * input.color.a, input.color.a) * weight;
	
	// store pixel revealage threshold
    output.reveal = input.color.a;
	return output;
}