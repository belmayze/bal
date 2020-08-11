/*!
 * @file   copy.hlsl
 * @brief
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
struct VARYING
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};

#ifdef BAL_VERTEX_SHADER

struct INPUT
{
    float3 Position : POSITION;
    float2 Texcoord : TEXCOORD0;
};

VARYING main(INPUT input)
{
    VARYING output;

    output.Position = float4(input.Position, 1.0);
    output.Texcoord = input.Texcoord;

    return output;
}

#endif // BAL_VERTEX_SHADER
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

    output.Color0 = ColorTexture.Sample(ColorSampler, input.Texcoord);

    return output;
}

#endif // BAL_PIXEL_SHADER
