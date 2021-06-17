/*!
 * @file   balGraphicsD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balDynamicArray.h>
#include <graphics/balIGraphics.h>

namespace bal { class FrameBuffer; }
namespace bal::d3d12 { class CommandQueue; }
namespace bal::d3d12 { class CommandListDirect; }
namespace bal::d3d12 { class RenderTargetColor; }

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

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

    /*!
     * 描画前処理
     */
    virtual void preDraw() override;

    /*!
     * 描画後処理
     */
    virtual void postDraw() override;

    /*!
     * 画面の反映
     */
    virtual void present(bool is_sync) override;

    /*!
     * GPUの実行終了を待機する
     */
    virtual void waitGPU() override;

    /*!
     * 破棄の処理を記述します
     */
    virtual bool destroy() override;

    /*!
     * デバイスを直接取得します
     * @note 基本的には内部で使用する想定です
     */
    ID3D12Device6* getDevice() const { return mpDevice.get(); }

public:
    /*!
     * コマンドリストを取得する
     */
    virtual ICommandListDirect* getCommandList() override;

    /*!
     * スワップチェーンのフレームバッファを取得する
     */
    virtual FrameBuffer* getSwapChainFrameBuffer() override;

    /*!
     * スワップチェーンのフォーマットを取得する
     */
    virtual ITexture::Format getSwapChainColorFormat() override;

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    uint32_t                                         mBufferCount = 2;
    std::unique_ptr<ID3D12Device6, ComDeleter>       mpDevice;
    std::unique_ptr<IDXGISwapChain3, ComDeleter>     mpSwapChain;
    std::unique_ptr<CommandListDirect[]>             mpCmdLists;
    std::unique_ptr<CommandQueue>                    mpCmdQueue;

    std::unique_ptr<RenderTargetColor[]>             mpSwapChainRenderTargets;
    std::unique_ptr<FrameBuffer[]>                   mpSwapChainFrameBuffers;

    UINT                                             mCurrentBufferIndex = 0;
};

}
