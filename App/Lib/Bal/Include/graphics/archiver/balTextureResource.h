/*!
 * @file   balTextureResource.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <io/balFile.h>
#include <graphics/balITexture.h>

// ----------------------------------------------------------------------------
namespace bal {

class TextureResource
{
public:
    TextureResource() {}
    ~TextureResource() {}

    /*!
     * アーカイバを読み込みます
     * @param[in] file ファイル
     */
    bool load(File&& file);

    //! テクスチャーの横幅
    uint32_t getWidth() const { return mWidth; }
    //! テクスチャーの高さ
    uint32_t getHeight() const { return mHeight; }
    //! 横ラインのバイト数
    uint32_t getRowPitch() const { return mRowPitch; }
    //! 次元（雑）
    ITexture::Dimension getDimension() const { return ITexture::Dimension::Texture2D; }
    //! フォーマット
    ITexture::Format getFormat() const { return mFormat; }
    //! データの先頭アドレス
    const void* getDataPtr() const { return mFile.getBuffer() + mDataOffset; }

private:
    File         mFile;

    ITexture::Format mFormat;
    uint32_t         mWidth       = 0;
    uint32_t         mHeight      = 0;
    uint32_t         mRowPitch    = 0;
    size_t           mDataSize    = 0;
    uintptr_t        mDataOffset  = 0;

private:
    //! フォーマットからビット数を取得する関数
    uint32_t getBitPerPixel_(ITexture::Format format);
};

}
