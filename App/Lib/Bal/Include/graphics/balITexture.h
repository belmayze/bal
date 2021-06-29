/*!
 * @file   balITexture.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>

namespace bal { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal {

class ITexture
{
public:
    //! テクスチャーの次元
    enum class Dimension
    {
        Texture1D,
        Texture2D,
        Texture3D
    };
    //! テクスチャーフォーマット
    enum class Format
    {
        Unknown,

        R8_UNORM,
        R8_SNORM,
        R16_FLOAT,
        R32_FLOAT,

        R8_G8_UNORM,
        R8_G8_SNORM,
        R16_G16_FLOAT,
        R32_G32_FLOAT,

        R5_G6_B5_UNORM,
        R11_G11_B10_FLOAT,
        R32_G32_B32_FLOAT,

        R5_G5_B5_A1_UNORM,
        R8_G8_B8_A8_UNORM,
        R8_G8_B8_A8_SNORM,
        R8_G8_B8_A8_SRGB,
        R10_G10_B10_A2_UNORM,
        R16_G16_B16_A16_FLOAT,
        R32_G32_B32_A32_FLOAT,

        D16_UNORM,
        D24_UNORM_S8_UINT,
        D32_FLOAT,

        BC1_UNORM,
        BC1_SRGB,
        BC2_UNORM,
        BC3_UNORM,
        BC3_SRGB,
        BC4_UNORM,
        BC4_SNORM,
        BC5_UNORM,
        BC5_SNORM,
        BC6H_UF16,
        BC6H_SF16,
        BC7_UNORM,
        BC7_SRGB
    };

    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics*  mpGraphics = nullptr;
        MathSize    mSize;
        Dimension   mDimension  = Dimension::Texture2D;
        Format      mFormat     = Format::R8_G8_B8_A8_UNORM;
        uint32_t    mMipNum     = 1;
        uint32_t    mArrayNum   = 1;
        uint32_t    mRowPitch   = 0;
        void*       mpGpuBuffer = nullptr;
        const void* mpBuffer    = nullptr;
    };

public:
    //! デストラクター
    virtual ~ITexture() {}

    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;

    /*!
     * 次元取得
     */
    virtual Dimension getDimension() const = 0;

    /*!
     * フォーマット取得
     */
    virtual Format getFormat() const = 0;
};

}
