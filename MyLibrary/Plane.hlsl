struct VS_INPUT
{
    float3 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR;
    float2 t : TEXTURE;
};

struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float3 n : NORMAL;
    float4 c : COLOR0;
    float2 t : TEXTURE;
};

cbuffer cb0
{
    matrix g_matWorld  : packoffset(c0); 
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};

VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;   
    float4 vWorld = mul(float4(vIn.p, 1.0f), g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    vOut.n = vIn.n;
    vOut.t = vIn.t;
    vOut.c = vIn.c;
    return vOut;
}

Texture2D g_txDiffuse1 : register(t0);
SamplerState sample0 : register(s0);

struct PS_IN
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};

float4 PS(VS_OUTPUT vIn) : SV_Target
{
    return g_txDiffuse1.Sample(sample0, vIn.t);
}