/*!
 * @file   balRenderTargetD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balIRenderTarget.h>

namespace bal::gfx { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal::gfx::d3d12 {

class RenderTargetColor : public IRenderTargetColor
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg     初期化構造体
     * @param[in] texture テクスチャー
     */
    virtual bool initialize(const InitializeArg& arg, std::unique_ptr<ITexture>&& texture) override;

public:
    //! ヒープハンドル
    D3D12_CPU_DESCRIPTOR_HANDLE getHandle() const { return mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart(); }

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    std::unique_ptr<ID3D12DescriptorHeap, ComDeleter> mpDescriptorHeap;
};

class RenderTargetDepth : public IRenderTargetDepth
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg     初期化構造体
     * @param[in] texture テクスチャー
     */
    virtual bool initialize(const InitializeArg& arg, std::unique_ptr<ITexture>&& texture) override;

public:
    //! ヒープハンドル
    D3D12_CPU_DESCRIPTOR_HANDLE getHandle() const { return mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart(); }

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    std::unique_ptr<ID3D12DescriptorHeap, ComDeleter> mpDescriptorHeap;
};

}
