/*!
 * @file   balDescriptorTable.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balIDescriptorTable.h>

// ----------------------------------------------------------------------------
namespace bal::gfx::d3d12 {

class DescriptorTable : public IDescriptorTable
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

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
};

}
