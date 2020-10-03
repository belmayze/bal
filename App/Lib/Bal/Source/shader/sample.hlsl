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

// ----------------------------------------------------------------------------
#ifdef BAL_VERTEX_SHADER

struct INPUT
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float2 Texcoord : TEXCOORD0;
};

cbuffer EnvConstantBuffer : register(b0)
{
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
    float4x4 ProjectionViewMatrix;
};

cbuffer MeshConstantBuffer : register(b1)
{
    float4x4 WorldMatrix;
    float4x4 WorldMatrixForNormal; //!< 法線用にスケールを除いた行列
};

VARYING main(INPUT input)
{
    VARYING output;

    output.Position = mul(mul(float4(input.Position, 1.0), WorldMatrix), ProjectionViewMatrix);
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

OUTPUT main(VARYING input)
{
    OUTPUT output;
    float3 vertex_normal = normalize(input.Normal);
    output.Color0 = float4(vertex_normal * 0.5 + 0.5, 1.0);
    return output;
}

#endif // BAL_PIXEL_SHADER
