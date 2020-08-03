/*!
 * @file   main.ps.hlsl
 * @brief
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0;
};

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    output.Color0 = float4(input.Texcoord, 0.0, 1.0);

    return output;
}