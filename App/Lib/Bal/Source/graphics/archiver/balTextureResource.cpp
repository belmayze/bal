/*!
 * @file   balTextureResource.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/archiver/balDds.h>
#include <graphics/archiver/balTextureResource.h>
#include <graphics/d3d12/balTextureD3D12.h>

namespace {

//! フォーマット識別子
constexpr uint8_t cFourCC_DXT1[4]  = { 'D', 'X', 'T', '1' };
constexpr uint8_t cFourCC_DXT2[4]  = { 'D', 'X', 'T', '2' };
constexpr uint8_t cFourCC_DXT3[4]  = { 'D', 'X', 'T', '3' };
constexpr uint8_t cFourCC_DXT4[4]  = { 'D', 'X', 'T', '4' };
constexpr uint8_t cFourCC_DXT5[4]  = { 'D', 'X', 'T', '5' };
constexpr uint8_t cFourCC_BC4U[4]  = { 'B', 'C', '4', 'U' };
constexpr uint8_t cFourCC_BC4S[4]  = { 'B', 'C', '4', 'S' };
constexpr uint8_t cFourCC_BC5U[4]  = { 'B', 'C', '5', 'U' };
constexpr uint8_t cFourCC_BC5S[4]  = { 'B', 'C', '5', 'S' };
constexpr uint8_t cFourCC_DXT10[4] = { 'D', 'X', '1', '0' };

}

namespace bal {

// ----------------------------------------------------------------------------
bool TextureResource::load(File&& file)
{
    // 保持
    mFile = std::move(file);

    // マジックチェック
    uint8_t magic[4];
    std::memcpy(magic, mFile.getBuffer(), sizeof(magic));
    if (magic[0] != 'D' || magic[1] != 'D' || magic[2] != 'S' || magic[3] != ' ')
    {
        return false;
    }

    // ヘッダー読み込み
    DDS::Header header;
    std::memcpy(&header, mFile.getBuffer() + 4, sizeof(DDS::Header));

    // フォーマットチェック
    ITexture::Format    format;
    ITexture::Dimension dimension     = ITexture::Dimension::Texture2D;
    uintptr_t           data_offset   = 4 + sizeof(DDS::Header);
    uint32_t            block_height  = 1;

    if (header.mPixelFormat.mFlags == DDS::PixelFlag::FourCC)
    {
        // 圧縮フォーマット
        if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_DXT10, sizeof(uint8_t) * 4) == 0)
        {
            // DXT10
            DDS::HeaderDXT10 header_dxt10;
            std::memcpy(&header_dxt10, mFile.getBuffer() + data_offset, sizeof(DDS::HeaderDXT10));
            data_offset += sizeof(DDS::HeaderDXT10);

            format     = bal::d3d12::Texture::ConvertFormatDxgi(header_dxt10.mFormat);
            switch (static_cast<DDS::EResourceDimension>(header_dxt10.mResourceDimension))
            {
                case DDS::EResourceDimension::Texture1D: dimension = ITexture::Dimension::Texture1D; break;
                case DDS::EResourceDimension::Texture2D: dimension = ITexture::Dimension::Texture2D; break;
                case DDS::EResourceDimension::Texture3D: dimension = ITexture::Dimension::Texture3D; break;
            }
        }
        else if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_DXT1, sizeof(uint8_t) * 4) == 0)
        {
            format = ITexture::Format::BC1_SRGB;
        }
        else if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_DXT2, sizeof(uint8_t) * 4) == 0)
        {
            format = ITexture::Format::BC2_UNORM;
        }
        else if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_DXT3, sizeof(uint8_t) * 4) == 0)
        {
            format = ITexture::Format::BC2_UNORM;
        }
        else if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_DXT4, sizeof(uint8_t) * 4) == 0)
        {
            format = ITexture::Format::BC3_SRGB;
        }
        else if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_DXT5, sizeof(uint8_t) * 4) == 0)
        {
            format = ITexture::Format::BC3_SRGB;
        }
        else if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_BC4S, sizeof(uint8_t) * 4) == 0)
        {
            format = ITexture::Format::BC4_SNORM;
        }
        else if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_BC4U, sizeof(uint8_t) * 4) == 0)
        {
            format = ITexture::Format::BC4_UNORM;
        }
        else if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_BC5S, sizeof(uint8_t) * 4) == 0)
        {
            format = ITexture::Format::BC5_SNORM;
        }
        else if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_BC5U, sizeof(uint8_t) * 4) == 0)
        {
            format = ITexture::Format::BC5_UNORM;
        }

        block_height = 4;
    }
    else
    {
        // @TODO
        return false;
    }

    // フォーマットからビット数を計算
    uint32_t bpp = getBitPerPixel_(format);

    // 情報保持
    mFormat     = format;
    mDimension  = dimension;
    mWidth      = header.mWidth;
    mHeight     = header.mHeight;
    mDataSize   = mFile.getBufferSize() - data_offset;
    mDataOffset = data_offset;

    // 横ラインのバイト数
    mDataSize = bpp * mWidth * mHeight / 8;
    mRowPitch = static_cast<uint32_t>(mDataSize / mHeight * block_height);

    return true;
}
// ----------------------------------------------------------------------------
uint32_t TextureResource::getBitPerPixel_(ITexture::Format format)
{
    switch (format)
    {
        case bal::ITexture::Format::BC1_UNORM:
        case bal::ITexture::Format::BC1_SRGB:
        case bal::ITexture::Format::BC4_UNORM:
        case bal::ITexture::Format::BC4_SNORM:
            return 4;

        case bal::ITexture::Format::R8_UNORM:
        case bal::ITexture::Format::R8_SNORM:
        case bal::ITexture::Format::BC2_UNORM:
        case bal::ITexture::Format::BC3_UNORM:
        case bal::ITexture::Format::BC3_SRGB:
        case bal::ITexture::Format::BC5_UNORM:
        case bal::ITexture::Format::BC5_SNORM:
        case bal::ITexture::Format::BC6H_UF16:
        case bal::ITexture::Format::BC6H_SF16:
        case bal::ITexture::Format::BC7_UNORM:
        case bal::ITexture::Format::BC7_SRGB:
            return 8;

        case bal::ITexture::Format::R16_FLOAT:
        case bal::ITexture::Format::R8_G8_UNORM:
        case bal::ITexture::Format::R8_G8_SNORM:
        case bal::ITexture::Format::R5_G6_B5_UNORM:
        case bal::ITexture::Format::R5_G5_B5_A1_UNORM:
        case bal::ITexture::Format::D16_UNORM:
            return 16;

        case bal::ITexture::Format::R32_FLOAT:
        case bal::ITexture::Format::R16_G16_FLOAT:
        case bal::ITexture::Format::R11_G11_B10_FLOAT:
        case bal::ITexture::Format::R8_G8_B8_A8_UNORM:
        case bal::ITexture::Format::R8_G8_B8_A8_SNORM:
        case bal::ITexture::Format::R8_G8_B8_A8_SRGB:
        case bal::ITexture::Format::R10_G10_B10_A2_UNORM:
        case bal::ITexture::Format::D24_UNORM_S8_UINT:
        case bal::ITexture::Format::D32_FLOAT:
            return 32;

        case bal::ITexture::Format::R32_G32_FLOAT:
        case bal::ITexture::Format::R16_G16_B16_A16_FLOAT:
            return 64;

        case bal::ITexture::Format::R32_G32_B32_FLOAT:
            return 96;

        case bal::ITexture::Format::R32_G32_B32_A32_FLOAT:
            return 128;
    }
    return 32;
}
// ----------------------------------------------------------------------------

}
