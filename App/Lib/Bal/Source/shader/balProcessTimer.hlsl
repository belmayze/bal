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

struct InstanceData
{
    float2 BoxOrigin;
    float2 BoxSize;
    float4 BoxColor;
};
cbuffer CBO : register(b0)
{
    InstanceData InstanceArrayData[1024];
};

// ----------------------------------------------------------------------------
#ifdef BAL_VERTEX_SHADER

struct INPUT
{
    float3 Position   : POSITION;
    float3 Normal     : NORMAL;
    float2 Texcoord   : TEXCOORD;
    uint   InstanceId : SV_InstanceID;
};

VARYING main(INPUT input)
{
    VARYING output;

    output.Position    = float4(input.Position, 1.0);
    // 基準点を左下に
    output.Position.xy = output.Position.xy + float2(1.0, 1.0);
    output.Position.xy = output.Position.xy * InstanceArrayData[input.InstanceId].BoxSize + InstanceArrayData[input.InstanceId].BoxOrigin;
    // 基準点を戻す
    output.Position.xy = output.Position.xy - float2(1.0, 1.0);
    output.Color       = InstanceArrayData[input.InstanceId].BoxColor;

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
