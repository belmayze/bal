/*!
 * @file   balDescriptorHeap.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balIDescriptorHeap.h>

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

class DescriptorHeap : public IDescriptorHeap
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

    /*!
     * テクスチャーのレンジベース
     */
    virtual uint32_t getTextureRangeBase() const override;

    /*!
     * テクスチャー枚数
     */
    virtual uint32_t getNumTexture() const override;

    /*!
     * 定数バッファのレンジベース
     */
    virtual uint32_t getConstantRangeBase() const override;

    /*!
     * 定数バッファ数
     */
    virtual uint32_t getNumConstantBuffer() const override;

public:
    //! デスクリプターヒープ
    ID3D12DescriptorHeap* getDesciptorHeap() const { return mpDescriptorHeap.get(); }

    //! ヒープハンドル
    D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle() const { return mpDescriptorHeap->GetGPUDescriptorHandleForHeapStart(); }

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    std::unique_ptr<ID3D12DescriptorHeap, ComDeleter> mpDescriptorHeap;
    uint32_t                                          mTextureRangeBase  = 0;
    uint32_t                                          mNumTexture        = 0;
    uint32_t                                          mConstantRangeBase = 0;
    uint32_t                                          mNumConstantBuffer = 0;
};

}
