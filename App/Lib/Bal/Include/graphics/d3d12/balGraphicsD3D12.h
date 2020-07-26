/*!
 * @file   balGraphicsD3D12.h
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

namespace bal::gfx { class ICommandQueue; }
namespace bal::gfx { class ICommandList; }

// ----------------------------------------------------------------------------
namespace bal::gfx::d3d12 {

class Graphics : public IGraphics
{
public:
    // コンストラクター
    Graphics();

    //! デストラクター
    virtual ~Graphics();

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

    /*!
     * デバイスを直接取得します
     * @note 基本的には内部で使用する想定です
     */
    ID3D12Device6* getDevice() const { return mpDevice.get(); }

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
    std::unique_ptr<ID3D12DescriptorHeap, ComDeleter>              mpDescriptorHeap;
    std::unique_ptr<std::unique_ptr<ID3D12Resource, ComDeleter>[]> mpRtvBuffers;
    std::unique_ptr<ICommandList>                                  mpCmdList;
    std::unique_ptr<ICommandQueue>                                 mpCmdQueue;
    D3D12_CPU_DESCRIPTOR_HANDLE                                    mRtvHandle;
    UINT                                                           mRtvHandleSize      = 0;
    UINT                                                           mCurrentBufferIndex = 0;
};

}
