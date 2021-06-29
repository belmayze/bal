/*!
 * @file   balDds.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// windows
#include <dxgiformat.h>
// bal
#include <io/balFile.h>

// ----------------------------------------------------------------------------
namespace bal {

class DDS
{
public:
    //! フラグ
    struct PixelFlag
    {
        enum : uint32_t
        {
            AlphaPixels   = 1 << 0,
            Alpha         = 1 << 1,
            FourCC        = 1 << 2,
            PaletteIndex8 = 1 << 5,
            Rgb           = 1 << 6,
            Luminance     = 1 << 17,
            BumpDudv      = 1 << 19
        };
    };
    //! ピクセルフォーマット
    struct PixelFormat
    {
        uint32_t mSize;
        uint32_t mFlags;
        uint8_t  mFourCC[4];
        uint32_t mRGBBitCount;
        uint32_t mRBitMask;
        uint32_t mGBitMask;
        uint32_t mBBitMask;
        uint32_t mABitMask;
    };

    //! フラグ
    enum class EHeaderFlag : uint32_t
    {
        Caps        = 1 << 0,
        Height      = 1 << 1,
        Width       = 1 << 2,
        Pitch       = 1 << 3,
        PixelFormat = 1 << 12,
        MipMapCount = 1 << 17,
        LinearSize  = 1 << 19,
        Depth       = 1 << 23
    };
    enum class ECapsFlag : uint32_t
    {
        Complex = 1 << 3,
        Texture = 1 << 12,
        MipMap  = 1 << 22
    };
    enum class ECpas2Flag : uint32_t
    {
        CubeMap          = 1 <<  9,
        CubeMapPositiveX = 1 << 10,
        CubeMapNegativeX = 1 << 11,
        CubeMapPositiveY = 1 << 12,
        CubeMapNegativeY = 1 << 13,
        CubeMapPositiveZ = 1 << 14,
        CubeMapNegativeZ = 1 << 15,
        Volume           = 1 << 21
    };
    //! ヘッダー
    struct Header
    {
        uint32_t    mSize;
        uint32_t    mFlags;
        uint32_t    mHeight;
        uint32_t    mWidth;
        uint32_t    mPitchOrLinearSize;
        uint32_t    mDepth;
        uint32_t    mMipMapCount;
        uint32_t    mReserved1[11];
        PixelFormat mPixelFormat;
        uint32_t    mCaps;
        uint32_t    mCaps2;
        uint32_t    mCaps3;
        uint32_t    mCaps4;
        uint32_t    mReserved2;
    };

    //! リソース次元
    enum class EResourceDimension : uint32_t
    {
        Texture1D = 2,
        Texture2D = 3,
        Texture3D = 4
    };
    //! フラグ
    enum class ResourceMiscFlag : uint32_t
    {
        TextureCube = 1 << 2
    };
    enum class MiscFlag : uint32_t
    {
        AlphaModeMask = 0x7
    };
    //! DXT10用追加ヘッダー
    struct HeaderDXT10
    {
        DXGI_FORMAT mFormat;
        uint32_t    mResourceDimension;
        uint32_t    mResourceMiscFlags;
        uint32_t    mArraySize;
        uint32_t    mMiscFlags;
    };

    static_assert(sizeof(Header) == 124, "DDS Header size mismatch");
    static_assert(sizeof(HeaderDXT10) == 20, "DDS DXT10 Extended Header size mismatch");
};

}
