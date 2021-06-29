/*!
 * @file   balPresent.hlsl
 * @brief
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
struct VARYING
{
    float4 Position : SV_POSITION;
    float3 Normal   : NORMAL;
    float2 Texcoord : TEXCOORD0;
};

cbuffer EnvConstantBuffer : register(b0)
{
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
    float3   DirectionalLightDir;
    float3   DirectionalLightColor;
};

// ----------------------------------------------------------------------------
#ifdef BAL_VERTEX_SHADER

struct INPUT
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float2 Texcoord : TEXCOORD0;
};

cbuffer MeshConstantBuffer : register(b1)
{
    float4x4 WorldMatrix;
    float4x4 WorldMatrixForNormal; //!< 法線用にスケールを除いた行列
};

VARYING main(INPUT input)
{
    VARYING output;

    output.Position = mul(mul(mul(float4(input.Position, 1.0), WorldMatrix), ViewMatrix), ProjectionMatrix);
    output.Normal   = mul(float4(input.Normal,   0.0), WorldMatrixForNormal).xyz;
    output.Texcoord = input.Texcoord;

    return output;
}

#endif // BAL_VERTEX_SHADER

// ----------------------------------------------------------------------------
#ifdef BAL_PIXEL_SHADER

struct OUTPUT
{
    float4 Color0 : SV_TARGET0;
};

Texture2D    ColorTexture : register(t0);
SamplerState ColorSampler : register(s0);

OUTPUT main(VARYING input)
{
    OUTPUT output;

    // サーフェース
    float  tex          = ColorTexture.Sample(ColorSampler, input.Texcoord).x;
    float3 albedo       = float3(1.0, 1.0, 1.0) * tex;
    float3 normal_world = normalize(input.Normal);

    // 簡易ライティング
    float  diffuse_coeff   = saturate(dot(normal_world, -DirectionalLightDir));
    float3 diffuse_direct  = albedo * (DirectionalLightColor * diffuse_coeff);

    float3 diffuse_ambient = albedo * 0.01;

    output.Color0 = float4(diffuse_direct + diffuse_ambient, 1.0);
    return output;
}

#endif // BAL_PIXEL_SHADER
