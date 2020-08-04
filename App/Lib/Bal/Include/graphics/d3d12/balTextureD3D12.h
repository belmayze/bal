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
namespace bal::gfx::d3d12 {

class Texture : public ITexture
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

public:
    //! バッファ
    ID3D12Resource* getTexture() const { return mpTexture.get(); }

    //! フォーマット
    DXGI_FORMAT getFormat() const { return mFormat; }

public:
    //! フォーマットを変換します
    static DXGI_FORMAT ConvertFormat(Format format);

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
    DXGI_FORMAT                                 mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
};

}
