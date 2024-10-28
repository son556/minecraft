Texture2D<float4> inputTex : register(t0);
RWTexture2D<float4> outputTex : register(u0);


cbuffer flag : register(b0)
{
    int xy_flag;
};

static const float weights[11] =
{
    0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
};

static const int blurRadius = 5;

#define N 256
#define CACHE_SIZE (N + 2*blurRadius)

[numthreads(1, 1, 1)]
void main( int3 gID : SV_GroupID,
    int3 gtID : SV_GroupThreadID,
    uint3 dtID : SV_DispatchThreadID 
)
{
    uint width, height;
    outputTex.GetDimensions(width, height);
    
    if (xy_flag == 0)
    {
        if (gtID.x < blurRadius)
        {
            int x = min(int(dtID.x) - blurRadius, 0);
        }
    }
    else
    {
        
    }
}