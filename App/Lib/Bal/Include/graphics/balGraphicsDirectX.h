/*!
 * @file   balGraphicsDirectX.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// d3d12
#include <d3d12.h>
#include <dxgi1_6.h>
// bal
#include <graphics/balIGraphics.h>

// ----------------------------------------------------------------------------
namespace bal::gfx {

class DirectX : public IGraphics
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

    // @Debug
    void loop();

    /*!
     * 画面の反映を待機します
     */
    void waitForPreviousFrame();

    /*!
     * 破棄の処理を記述します
     */
    virtual bool destroy() override;

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    uint32_t                                                       mBufferCount = 2;
    std::unique_ptr<ID3D12Device6, ComDeleter>                     mpDevice;
    std::unique_ptr<IDXGISwapChain1, ComDeleter>                   mpSwapChain;
    std::unique_ptr<ID3D12CommandAllocator, ComDeleter>            mpCmdAllocator;
    std::unique_ptr<ID3D12CommandQueue, ComDeleter>                mpCmdQueue;
    std::unique_ptr<ID3D12GraphicsCommandList, ComDeleter>         mpCmdList;
    std::unique_ptr<ID3D12DescriptorHeap, ComDeleter>              mpDescriptorHeap;
    std::unique_ptr<std::unique_ptr<ID3D12Resource, ComDeleter>[]> mpRtvBuffers;
    std::unique_ptr<ID3D12Fence, ComDeleter>                       mpFence;
    D3D12_CPU_DESCRIPTOR_HANDLE                                    mRtvHandle;
    UINT                                                           mRtvHandleSize      = 0;
    UINT                                                           mCurrentBufferIndex = 0;
    HANDLE                                                         mEventHandle;
};

}
