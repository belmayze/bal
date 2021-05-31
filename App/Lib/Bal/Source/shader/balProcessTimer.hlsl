/*!
 * @file   balProcessTimer.hlsl
 * @brief
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
struct VARYING
{
    float4 Position : SV_POSITION;
    float4 Color    : COLOR;
};

// ----------------------------------------------------------------------------
#ifdef BAL_VERTEX_SHADER

struct INPUT
{
    float3 Position : POSITION;
    float4 Color    : COLOR;
};

VARYING main(INPUT input)
{
    VARYING output;

    output.Position = float4(input.Position, 1.0);
    output.Color    = input.Color;

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
    output.Color0 = input.Color;
    return output;
}

#endif // BAL_PIXEL_SHADER
