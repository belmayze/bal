/*!
 * @file   balTextureD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balITexture.h>

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

class Texture : public ITexture
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

    //! 次元
    virtual Dimension getDimension() const override { return mDimension; }

    //! フォーマット
    virtual Format getFormat() const override { return mFormat; }

public:
    //! バッファ
    ID3D12Resource* getTexture() const { return mpTexture.get(); }

    //! キューブマップチェック
    bool isCubemap() const { return false; }

    //! 配列数
    uint32_t getNumArray() const { return mNumArray; }

    //! ミップマップ数
    uint32_t getNumMipLevel() const { return mNumMipLevel; }

public:
    //! フォーマットを変換します
    static DXGI_FORMAT ConvertFormat(Format format);
    static Format ConvertFormatDxgi(DXGI_FORMAT format);

    //! 圧縮フォーマットかを取得します
    static bool IsCompressFormat(Format format);

    //! 次元を変換します
    static D3D12_RESOURCE_DIMENSION ConvertDimension(Dimension dimension);

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    std::unique_ptr<ID3D12Resource, ComDeleter> mpTexture;
    Format                                      mFormat      = Format::R8_G8_B8_A8_UNORM;
    Dimension                                   mDimension   = Dimension::Texture2D;
    uint32_t                                    mNumArray    = 1;
    uint32_t                                    mNumMipLevel = 1;
};

}
