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
    float2 Texcoord : TEXCOORD0;
};

// ----------------------------------------------------------------------------
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

    float4 color = ColorTexture.Sample(ColorSampler, input.Texcoord);

    // @TODO: トーンカーブ

    color = abs(color);

    // ガンマ補正（sRGB）
    if (color.r <= 0.0031308) { output.Color0.r = color.r * 12.92; }
    else                      { output.Color0.r = 1.055 * pow(color.r, 1.0 / 2.4) - 0.055; }
    if (color.g <= 0.0031308) { output.Color0.g = color.g * 12.92; }
    else                      { output.Color0.g = 1.055 * pow(color.g, 1.0 / 2.4) - 0.055; }
    if (color.b <= 0.0031308) { output.Color0.b = color.b * 12.92; }
    else                      { output.Color0.b = 1.055 * pow(color.b, 1.0 / 2.4) - 0.055; }
    output.Color0.a = 1.0;

    return output;
}

#endif // BAL_PIXEL_SHADER
