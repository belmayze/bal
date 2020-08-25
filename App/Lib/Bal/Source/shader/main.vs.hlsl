/*!
 * @file   main.vs.hlsl
 * @brief
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Texcoord : TEXCOORD0;
};
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};

// 定数バッファ
struct ModelConstantBuffer
{
    float4x4 mWorldMtx;
};
cbuffer ConstantBuffer0 : register(b0) { ModelConstantBuffer ModelCB; };

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.Position = mul(ModelCB.mWorldMtx, float4(input.Position, 1.0));
    output.Texcoord = input.Texcoord;

    return output;
}
