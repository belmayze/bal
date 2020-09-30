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
namespace bal { class ShaderArchive; }
namespace bal::d3d12 { class CommandQueue; }
namespace bal::d3d12 { class CommandListBundle; }
namespace bal::d3d12 { class CommandListDirect; }
namespace bal::d3d12 { class ConstantBuffer; }
namespace bal::d3d12 { class DescriptorTable; }
namespace bal::d3d12 { class ShapeBuffer; }
namespace bal::d3d12 { class Pipeline; }
namespace bal::d3d12 { class RenderTargetColor; }
namespace bal::d3d12 { class RenderTargetDepth; }
namespace bal::d3d12 { class Texture; }

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
     * デフォルトのフレームバッファを取得する
     */
    virtual FrameBuffer* getDefaultFrameBuffer() override;

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

    std::unique_ptr<RenderTargetColor>               mpRenderTargetColor;
    std::unique_ptr<RenderTargetDepth>               mpRenderTargetDepth;
    std::unique_ptr<FrameBuffer>                     mpFrameBuffer;

    std::unique_ptr<ShaderArchive>                   mpShaderArchive;

    std::unique_ptr<CommandListBundle>               mpCmdBundle;
    std::unique_ptr<Pipeline>                        mpPipeline;
    std::unique_ptr<ShapeBuffer>                     mpShapeBuffer;
    std::unique_ptr<DescriptorTable>                 mpModelDescriptorTable;
    std::unique_ptr<ConstantBuffer>                  mpModelConstantBuffer;

    std::unique_ptr<ShapeBuffer>                     mpQuadShapeBuffer;
    std::unique_ptr<DescriptorTable>                 mpCopyDescriptorTable;
    std::unique_ptr<Pipeline>                        mpCopyPipeline;

    UINT                                             mCurrentBufferIndex = 0;
};

}
