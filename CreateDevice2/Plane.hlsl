struct VS_IN
{
    float3 p : POSITION;
    float2 t : TEXTURE;
};

struct VS_OUT
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};

cbuffer VS_CBUFFER
{
    matrix g_matWorld : packoffset(c0);
    matrix g_matView : packoffset(c4);
    matrix g_matProjection : packoffset(c8);
};

VS_OUT VS(VS_IN vIn)
{
    VS_OUT vOut = (VS_OUT)0;

    //vOut.p = float4(vIn.p.x, vIn.p.y, vIn.p.z, 1);

    vOut.p = mul(float4(vIn.p, 1.0f), g_matWorld);
    vOut.p = mul(vOut.p, g_matView);
    vOut.p = mul(vOut.p, g_matProjection);

    vOut.t = vIn.t;
    return vOut;
}


Texture2D g_txDiffuse1 : register(t0);
SamplerState sample0 : register(s0);
struct PS_IN
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};
float4 PS(PS_IN vIn) : SV_Target
{
    return g_txDiffuse1.Sample(sample0, vIn.t);
}