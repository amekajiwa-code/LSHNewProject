struct VS_IN
{
    float3 p : POS;
    float2 t : TEXTURE;
};

struct VS_OUT
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};

cbuffer cb0
{
    matrix g_matWorld  : packoffset(c0);
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};
VS_OUT VS(VS_IN vIn)
{
    VS_OUT vOut = (VS_OUT)0;
  
    float4 vWorld = mul(float4(vIn.p, 1.0f), g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;

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