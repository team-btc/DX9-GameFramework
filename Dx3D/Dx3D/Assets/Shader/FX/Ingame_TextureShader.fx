
float4 PlayerPos;
float4 TargetPos;

float PlayerScale;
float TargetScale;

float4 PlayerColor;
float4 TargetColor;

float Density;
float BackGroundDensity;
float Tex1Density;
float Tex2Density;
float Tex3Density;


texture BackGroundTexture;
texture texture1;
texture texture2;
texture texture3;
texture AlphaMap;

float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;

sampler2D BackGroundTex = sampler_state
{
    Texture = (BackGroundTexture);
    MINFILTER = GAUSSIANQUAD;
    MAGFILTER = GAUSSIANQUAD;
    AddressU = Mirror;
    AddressV = Mirror;
};

sampler2D TexSampler1 = sampler_state
{
    Texture = (texture1);
    MINFILTER = GAUSSIANQUAD;
    MAGFILTER = GAUSSIANQUAD;
    AddressU = Mirror;
    AddressV = Mirror;

};
sampler2D TexSampler2 = sampler_state
{
    Texture = (texture2);
    MINFILTER = GAUSSIANQUAD;
    MAGFILTER = GAUSSIANQUAD;
};
sampler2D TexSampler3 = sampler_state
{
    Texture = (texture3);
    MINFILTER = GAUSSIANQUAD;
    MAGFILTER = GAUSSIANQUAD;
};
sampler2D TexAlpha = sampler_state
{
    Texture = (AlphaMap);
};

struct VS_INPUT
{
    float4 mPosition : POSITION;
};

struct VS_OUTPUT
{
    float4 mPosition : POSITION;
};

struct PS_INPUT
{
    float2 uv : TEXCOORD;
};

VS_OUTPUT ColorShader_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;

    Output.mPosition = mul(Input.mPosition, gWorldMatrix);
    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

    return Output;
}

float4 main_0(PS_INPUT Input) : COLOR
{
    float BACK_DEN = BackGroundDensity;
    float TEX1_DEN = Tex1Density;
    float TEX2_DEN = Tex2Density;
    float TEX3_DEN = Tex3Density;

    float2 UV = Input.uv;
    UV.x *= BACK_DEN;
    UV.y *= BACK_DEN;
    float4 Tex0 = tex2D(BackGroundTex, UV);

    UV = Input.uv;
    UV.x *= TEX1_DEN;
    UV.y *= TEX1_DEN;
    float4 Tex1 = tex2D(TexSampler1, UV);

    UV = Input.uv;
    UV.x *= TEX2_DEN;
    UV.y *= TEX2_DEN;
    float4 Tex2 = tex2D(TexSampler2, UV);
    UV = Input.uv;
    UV.x *= TEX3_DEN;
    UV.y *= TEX3_DEN;
    float4 Tex3 = tex2D(TexSampler3, UV);
    float4 Alpha = tex2D(TexAlpha, Input.uv);

    float d = Alpha.r + Alpha.g + Alpha.b;
    float Rd = Alpha.r / d;
    float Gd = Alpha.g / d;
    float Bd = Alpha.b / d;
    float Backd = 1 - Alpha.r - Alpha.g - Alpha.b;
    if (Backd <= 0)
    {
        Backd = 0.0f;
    }
    // Player Bottom Circle Render
    float4 Pbrush = float4(0, 0, 0, 1);
    float Pr = PlayerScale;
    float Pl;
    float3 vPlayerPick = (float3)PlayerPos;
    vPlayerPick.y = 0;
    float3 vPlayerPixel = float3(Input.uv.x, 0, Input.uv.y);
    float3 vPLen = vPlayerPick - vPlayerPixel;
    Pl = length(vPLen);
    if (Pl < Pr)
    {
        Pbrush = PlayerColor;
    }

    // Enemy Bottom Circle Render
    float4 Tbrush = float4(0, 0, 0, 1);
    float Tr = TargetScale;
    float Tl;
    float3 vTargetPick = (float3)TargetPos;
    vTargetPick.y = 0;
    float3 vTargetPixel = float3(Input.uv.x, 0, Input.uv.y);
    float3 vTLen = vTargetPick - vTargetPixel;
    Tl = length(vTLen);
    if (Tl < Tr)
    {
        Tbrush = TargetColor;
    }

    return (Tex0 * Backd + Tex1 * Alpha.r * Rd + Tex2 * Alpha.g * Gd + Tex3 * Alpha.b * Bd + Pbrush * 0.0025f + Tbrush * 0.0025f);
};

technique Shader
{
    pass Pass_0
    {
        PixelShader = compile ps_2_0 main_0();
    }
};