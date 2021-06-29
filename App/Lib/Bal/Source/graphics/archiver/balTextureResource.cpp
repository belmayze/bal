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
    ITexture::Format format;
    uintptr_t        data_offset = 4 + sizeof(DDS::Header);

    if (header.mPixelFormat.mFlags == DDS::PixelFlag::FourCC)
    {
        // 圧縮フォーマット
        if (std::memcmp(header.mPixelFormat.mFourCC, cFourCC_DXT10, sizeof(uint8_t) * 4) == 0)
        {
            // DXT10
            DDS::HeaderDXT10 header_dxt10;
            std::memcpy(&header_dxt10, mFile.getBuffer() + data_offset, sizeof(DDS::HeaderDXT10));
            data_offset += sizeof(DDS::HeaderDXT10);

            format = bal::d3d12::Texture::ConvertFormatDxgi(header_dxt10.mFormat);
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
    }
    else
    {
        // @TODO
        return false;
    }

    // 情報保持
    mFormat     = format;
    mWidth      = header.mWidth;
    mHeight     = header.mHeight;
    mDataSize   = mFile.getBufferSize() - data_offset;
    mDataOffset = data_offset;

    return true;
}
// ----------------------------------------------------------------------------

}
