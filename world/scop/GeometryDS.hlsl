cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

struct DS_INPUT
{
    int type : TYPE;
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float3 world_pos : POSITION_WORLD;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
};

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
};

struct PatchConstOutput
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 4

[domain("quad")]
PS_INPUT main(
	PatchConstOutput input,
	float2 uv : SV_DomainLocation,
	const OutputPatch<DS_INPUT, NUM_CONTROL_POINTS> patch)
{
	PS_INPUT output;

    float3 position;
    float3 v1 = lerp(patch[0].pos.xyz, patch[1].pos.xyz, uv.x);
    float3 v2 = lerp(patch[2].pos.xyz, patch[3].pos.xyz, uv.x);
    position = lerp(v1, v2, uv.y);
    output.pos = float4(position, 1.0);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    output.dir = patch[0].dir;
    output.normal = patch[0].normal;
    output.world_pos = position;
    output.type = patch[0].type;
    float2 uv1 = lerp(patch[0].uv, patch[1].uv, uv.x);
    float2 uv2 = lerp(patch[2].uv, patch[3].uv, uv.x);
    output.uv = lerp(uv1, uv2, uv.y);
    return output;
}
