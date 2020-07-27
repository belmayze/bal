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
    float2 Texcoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    output.Position = float4(input.Position, 1.0);
    output.Texcoord = input.Texcoord;

    return output;
}
