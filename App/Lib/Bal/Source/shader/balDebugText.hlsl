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
    float2 Texcoord : TEXCOORD;
};

struct InstanceData
{
    float2 Offset;
    float2 Texcoord;
};
cbuffer CBO : register(b0)
{
    float2 BaseMatrix0;
    float2 BaseMatrix1;
    float2 BaseTranslate;
    float2 TexcoordScale;
    InstanceData InstanceArrayData[1024];
};

// ----------------------------------------------------------------------------
#ifdef BAL_VERTEX_SHADER

struct INPUT
{
    float3 Position   : POSITION;
    float2 Texcoord   : TEXCOORD;
    uint   InstanceId : SV_InstanceID;
};

VARYING main(INPUT input)
{
    VARYING output;

    output.Position.x = dot(BaseMatrix0, input.Position.xy) + BaseTranslate.x + InstanceArrayData[input.InstanceId].Offset.x;
    output.Position.y = dot(BaseMatrix1, input.Position.xy) + BaseTranslate.y + InstanceArrayData[input.InstanceId].Offset.y;
    output.Position.z = 1.0;
    output.Position.w = 1.0;
    output.Texcoord = (input.Texcoord * TexcoordScale) + InstanceArrayData[input.InstanceId].Texcoord;

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
    output.Color0 = ColorTexture.Sample(ColorSampler, input.Texcoord);
    return output;
}

#endif // BAL_PIXEL_SHADER
