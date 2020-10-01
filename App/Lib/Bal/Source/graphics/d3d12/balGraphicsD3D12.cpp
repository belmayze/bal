/*!
 * @file   balGraphicsD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#include <thread>
// bal
#include <io/balFile.h>
#include <graphics/balFrameBuffer.h>
#include <graphics/balViewport.h>
#include <graphics/archiver/balShaderArchive.h>
#include <graphics/d3d12/balCommandListBundleD3D12.h>
#include <graphics/d3d12/balCommandListDirectD3D12.h>
#include <graphics/d3d12/balCommandQueueD3D12.h>
#include <graphics/d3d12/balConstantBufferD3D12.h>
#include <graphics/d3d12/balDescriptorTableD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balInputLayoutD3D12.h>
#include <graphics/d3d12/balShapeBufferD3D12.h>
#include <graphics/d3d12/balPipelineD3D12.h>
#include <graphics/d3d12/balRenderTargetD3D12.h>
#include <graphics/d3d12/balTextureD3D12.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------

Graphics::Graphics() {}

// ----------------------------------------------------------------------------

Graphics::~Graphics() {}

// ----------------------------------------------------------------------------

bool Graphics::initialize(const InitializeArg& arg)
{
    // フラグ
    UINT dxgi_factory_flags = 0;

    // デバッグレイヤー
#   if BAL_BUILD_UNDER_DEVELOPMENT
    {
        Microsoft::WRL::ComPtr<ID3D12Debug> p_debug_conrtoller;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&p_debug_conrtoller))))
        {
            p_debug_conrtoller->EnableDebugLayer();
            dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#   endif // BAL_BUILD_UNDER_DEVELOPMENT

    // DXGI
    Microsoft::WRL::ComPtr<IDXGIFactory7> p_factory;
    if (FAILED(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&p_factory))))
    {
        return false;
    }

    // アダプターの検索しながらデバイス取得
    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_1_0_CORE;
    std::array initialize_feature_levels =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    Microsoft::WRL::ComPtr<IDXGIAdapter1> p_adapter;
    Microsoft::WRL::ComPtr<ID3D12Device6> p_device;
    {
        bool found_adapter = false;
        for (uint32_t i_adapter = 0; p_factory->EnumAdapters1(i_adapter, p_adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i_adapter)
        {
            DXGI_ADAPTER_DESC1 desc;
            p_adapter->GetDesc1(&desc);

            if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0)
            {
                // ソフトウェア駆動は除外
                p_adapter.Reset();
                continue;
            }
            else
            {
                // ハードウェアなら初期化
                for (D3D_FEATURE_LEVEL level : initialize_feature_levels)
                {
                    if (SUCCEEDED(D3D12CreateDevice(p_adapter.Get(), level, IID_PPV_ARGS(&p_device))))
                    {
                        feature_level = level;
                        found_adapter = true;
                        break;
                    }
                }
            }

            if (found_adapter) { break; }
        }
        if (!found_adapter)
        {
            return false;
        }
    }
    mpDevice.reset(p_device.Detach());

    // コマンドキュー
    mpCmdQueue = make_unique<CommandQueue>(nullptr);
    {
        CommandQueue::InitializeArg init_arg;
        init_arg.mpGraphics   = this;
        init_arg.mType        = CommandQueue::Type::Graphics;
        init_arg.mBufferCount = arg.mBufferCount;
        if (!mpCmdQueue->initialize(init_arg))
        {
            return false;
        }
    }

    // コマンドリスト
    mpCmdLists = make_unique<CommandListDirect[]>(nullptr, arg.mBufferCount);
    for (uint32_t i_buffer = 0; i_buffer < arg.mBufferCount; ++i_buffer)
    {
        CommandListDirect::InitializeArg init_arg;
        init_arg.mpGraphics = this;
        if (!mpCmdLists[i_buffer].initialize(init_arg))
        {
            return false;
        }
    }

    // スワップチェーン
    Microsoft::WRL::ComPtr<IDXGISwapChain3> p_swap_chain;
    {
        DXGI_SWAP_CHAIN_DESC1 desc = {};
        desc.Width              = arg.mRenderBufferSize.getWidth();
        desc.Height             = arg.mRenderBufferSize.getHeight();
        desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Stereo             = FALSE;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount        = arg.mBufferCount;
        desc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.Flags              = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen_desc = {};
        fullscreen_desc.RefreshRate.Numerator   = 60;
        fullscreen_desc.RefreshRate.Denominator = 1;
        fullscreen_desc.Windowed                = TRUE;

        Microsoft::WRL::ComPtr<IDXGISwapChain1> p_tmp_swap_chain;
        if (FAILED(p_factory->CreateSwapChainForHwnd(
            reinterpret_cast<CommandQueue*>(mpCmdQueue.get())->getCommandQueue(),
            arg.mHwnd,
            &desc, &fullscreen_desc,
            nullptr,
            &p_tmp_swap_chain)))
        {
            return false;
        }

        if (FAILED(p_tmp_swap_chain->QueryInterface(IID_PPV_ARGS(&p_swap_chain))))
        {
            return false;
        }
    }

    // スワップバッファのテクスチャーをレンダーターゲット化
    mpSwapChainRenderTargets = make_unique<RenderTargetColor[]>(nullptr, arg.mBufferCount);
    mpSwapChainFrameBuffers  = make_unique<FrameBuffer[]>(nullptr, arg.mBufferCount);
    {
        for (uint32_t i_buffer = 0; i_buffer < arg.mBufferCount; ++i_buffer)
        {
            Microsoft::WRL::ComPtr<ID3D12Resource> p_resource;
            if (FAILED(p_swap_chain->GetBuffer(i_buffer, IID_PPV_ARGS(&p_resource))))
            {
                return false;
            }

            std::unique_ptr<Texture> p_color_buffer = make_unique<Texture>(nullptr);
            {
                Texture::InitializeArg init_arg;
                init_arg.mpGraphics = this;
                init_arg.mpBuffer   = p_resource.Detach();
                init_arg.mFormat    = Texture::Format::R8_G8_B8_A8_UNORM;
                p_color_buffer->initialize(init_arg);
            }
            {
                RenderTargetColor::InitializeArg init_arg;
                init_arg.mpGraphics = this;
                mpSwapChainRenderTargets[i_buffer].initialize(init_arg, std::move(p_color_buffer));
            }
            {
                mpSwapChainFrameBuffers[i_buffer].setRenderTargetColor(0, &mpSwapChainRenderTargets[i_buffer]);
                mpSwapChainFrameBuffers[i_buffer].setResolution(arg.mRenderBufferSize);
            }
        }
    }

    // レンダーバッファを確保
    {
        // テクスチャーを確保
        std::unique_ptr<Texture> p_color_buffer = make_unique<Texture>(nullptr);
        {
            Texture::InitializeArg init_arg;
            init_arg.mpGraphics = this;
            init_arg.mDimension = Texture::Dimension::Texture2D;
            init_arg.mFormat    = Texture::Format::R16_G16_B16_A16_FLOAT;
            init_arg.mSize      = arg.mRenderBufferSize;
            if (!p_color_buffer->initialize(init_arg)) { return false; }
        }
        std::unique_ptr<Texture> p_depth_buffer = make_unique<Texture>(nullptr);
        {
            Texture::InitializeArg init_arg;
            init_arg.mpGraphics = this;
            init_arg.mDimension = Texture::Dimension::Texture2D;
            init_arg.mFormat    = Texture::Format::D32_FLOAT;
            init_arg.mSize      = arg.mRenderBufferSize;
            if (!p_depth_buffer->initialize(init_arg)) { return false; }
        }

        // レンダーターゲット
        mpRenderTargetColor = make_unique<RenderTargetColor>(nullptr);
        {
            RenderTargetColor::InitializeArg init_arg;
            init_arg.mpGraphics = this;
            if (!mpRenderTargetColor->initialize(init_arg, std::move(p_color_buffer))) { return false; }
        }
        mpRenderTargetDepth = make_unique<RenderTargetDepth>(nullptr);
        {
            RenderTargetDepth::InitializeArg init_arg;
            init_arg.mpGraphics = this;
            if (!mpRenderTargetDepth->initialize(init_arg, std::move(p_depth_buffer))) { return false; }
        }

        // フレームバッファ
        mpFrameBuffer = make_unique<FrameBuffer>(nullptr);
        {
            mpFrameBuffer->setRenderTargetColor(0, mpRenderTargetColor.get());
            mpFrameBuffer->setRenderTargetDepth(mpRenderTargetDepth.get());
            mpFrameBuffer->setResolution(arg.mRenderBufferSize);
        }
    }

    // シェーダーアーカイブを取得
    mpShaderArchive = make_unique<ShaderArchive>(nullptr);
    {
        File file;
        if (!file.loadFromFile("Shader\\bal_shader.bsa")) { return false; }
        if (!mpShaderArchive->loadArchiver(std::move(file))) { return false; }
    }

    // 頂点、インデックスバッファを仮初期化
    mpShapeBuffer = make_unique<ShapeBuffer>(nullptr);
    {
        // とりあえず position, texcoord
        float vertices[] = {
             0.f,  1.f, 0.f,  0.5f, 1.f,
            -1.f, -1.f, 0.f,  0.f,  0.f,
             1.f, -1.f, 0.f,  1.f,  0.f
        };
        uint16_t indices[] = {0, 1, 2};

        ShapeBuffer::InitializeArg init_arg;
        init_arg.mpGraphics         = this;
        init_arg.mpVertexBuffer     = reinterpret_cast<const uint8_t*>(vertices);
        init_arg.mVertexBufferSize  = sizeof(vertices);
        init_arg.mVertexStride      = sizeof(float) * 5;
        init_arg.mpIndexBuffer      = reinterpret_cast<const uint8_t*>(indices);
        init_arg.mIndexBufferSize   = sizeof(indices);
        init_arg.mIndexCount        = 3;
        init_arg.mIndexBufferFormat = ShapeBuffer::IndexBufferFormat::Uint16;
        if (!mpShapeBuffer->initialize(init_arg)) { return false; }
    }

    // モデル用の定数バッファ
    mpModelConstantBuffer = make_unique<ConstantBuffer>(nullptr);
    {
        // 仮
        struct ModelCB
        {
            MathMatrix44 mWorldMtx;
        };

        ConstantBuffer::InitializeArg init_arg;
        init_arg.mpGraphics   = this;
        init_arg.mBufferCount = 1;
        init_arg.mBufferSize  = sizeof(ModelCB);
        if (!mpModelConstantBuffer->initialize(init_arg)) { return false; }

        // 仮
        mpModelConstantBuffer->getBufferPtr<ModelCB>()->mWorldMtx.setRotateZ(Radian(0.f));
    }

    // モデル用のデスクリプターテーブル
    mpModelDescriptorTable = make_unique<DescriptorTable>(nullptr);
    {
        // 定数バッファ
        const IConstantBuffer* p_constant_buffers[] = { mpModelConstantBuffer.get() };

        DescriptorTable::InitializeArg init_arg;
        init_arg.mpGraphics         = this;
        init_arg.mNumConstantBuffer = 1;
        init_arg.mpConstantBuffers  = p_constant_buffers;
        if (!mpModelDescriptorTable->initialize(init_arg)) { return false; }
    }

    // パイプラインを仮初期化
    mpPipeline = make_unique<Pipeline>(nullptr);
    {
        // 頂点レイアウト
        std::unique_ptr<InputLayout> p_input_layout = make_unique<InputLayout>(nullptr);
        {
            std::unique_ptr<InputLayout::InputLayoutDesc[]> descs = make_unique<InputLayout::InputLayoutDesc[]>(nullptr, 2);
            descs[0] = { .mName = "POSITION", .mSemanticIndex = 0, .mType = InputLayout::Type::Vec3, .mOffset =  0 };
            descs[1] = { .mName = "TEXCOORD", .mSemanticIndex = 0, .mType = InputLayout::Type::Vec2, .mOffset = 12 };

            InputLayout::InitializeArg init_arg;
            init_arg.mpGraphics      = this;
            init_arg.mNumInputLayout = 2;
            init_arg.mpInputLayouts  = descs.get();
            if (!p_input_layout->initialize(init_arg)) { return false; }
        }

        // シェーダー取得
        const ShaderArchive::ShaderContainer& shader_container = mpShaderArchive->getShaderContainer(mpShaderArchive->findProgram("Sample1"));

        // パイプライン初期化
        Pipeline::InitializeArg init_arg;
        init_arg.mpGraphics        = this;
        init_arg.mNumOutput        = 1;
        init_arg.mOutputFormats[0] = Texture::Format::R16_G16_B16_A16_FLOAT;
        init_arg.mpInputLayout     = p_input_layout.get();

        init_arg.mNumConstantBuffer = 1;

        init_arg.mpVertexShaderBuffer    = shader_container.mVertexShader.mBuffer;
        init_arg.mVertexShaderBufferSize = shader_container.mVertexShader.mBufferSize;
        init_arg.mpPixelShaderBuffer     = shader_container.mPixelShader.mBuffer;
        init_arg.mPixelShaderBufferSize  = shader_container.mPixelShader.mBufferSize;
        if (!mpPipeline->initialize(init_arg)) { return false; }
    }

    // 情報
    mBufferCount = arg.mBufferCount;

    // 必要な COM を保存
    mpSwapChain.reset(p_swap_chain.Detach());

    return true;
}

// ----------------------------------------------------------------------------

void Graphics::preDraw()
{
    // バッファー切り替え
    mpCmdQueue->waitExecuted(mCurrentBufferIndex);
    mpCmdLists[mCurrentBufferIndex].reset();
}

// ----------------------------------------------------------------------------

void Graphics::postDraw()
{
    // コマンドリストを閉じて実行
    mpCmdLists[mCurrentBufferIndex].close();
    mpCmdQueue->execute(mpCmdLists[mCurrentBufferIndex], mCurrentBufferIndex);

    // 画面の反映
    mpSwapChain->Present(0, 0);

    // 実行待ち
    if (++mCurrentBufferIndex > (mBufferCount - 1)) { mCurrentBufferIndex = 0; }
}

// ----------------------------------------------------------------------------

void Graphics::waitGPU()
{
    mpCmdQueue->waitExecutedAll();
}

#if 0
// ----------------------------------------------------------------------------
void Graphics::loop()
{
    mpCmdQueue->waitExecuted(mCurrentBufferIndex);
    mpCmdLists[mCurrentBufferIndex].reset();

    // バリア
    mpCmdLists[mCurrentBufferIndex].resourceBarrier(
        *mpRenderTargetColor->getTexture(),
        CommandListDirect::ResourceBarrierType::GenericRead,
        CommandListDirect::ResourceBarrierType::RenderTargetColor
    );
    mpCmdLists[mCurrentBufferIndex].resourceBarrier(
        *mpRenderTargetDepth->getTexture(),
        CommandListDirect::ResourceBarrierType::GenericRead,
        CommandListDirect::ResourceBarrierType::RenderTargetDepth
    );

    // レンダリング用バッファに切り替え
    mpCmdLists[mCurrentBufferIndex].setViewport(Viewport(*mpFrameBuffer.get()));
    mpCmdLists[mCurrentBufferIndex].bindFrameBuffer(*mpFrameBuffer.get());
    mpCmdLists[mCurrentBufferIndex].clear(*mpFrameBuffer.get(), CommandListDirect::ClearFlag::Color | CommandListDirect::ClearFlag::Depth, MathColor(0.f, 0.f, 0.f, 1.f), 1.f, 0);

    // 仮: 定数バッファ更新
    {
        struct ModelCB
        {
            MathMatrix44 mWorldMtx;
        };

        static float degree = 0.f;
        degree += 0.1f;
        ModelCB* p_cb = mpModelConstantBuffer->getBufferPtr<ModelCB>();
        p_cb->mWorldMtx.setRotateZ(Degree(degree));
        p_cb->mWorldMtx.setTranslate(MathVector3(0.f, 0.f, 0.f));
    }

    // @TODO: レンダリング
    mpCmdLists[mCurrentBufferIndex].bindPipeline(*mpPipeline);
    mpCmdLists[mCurrentBufferIndex].setDescriptorTable(0, *mpModelDescriptorTable);
    mpCmdLists[mCurrentBufferIndex].executeBundle(*mpCmdBundle);

    // バリア
    mpCmdLists[mCurrentBufferIndex].resourceBarrier(
        *mpRenderTargetColor->getTexture(),
        CommandListDirect::ResourceBarrierType::RenderTargetColor,
        CommandListDirect::ResourceBarrierType::GenericRead
    );
    mpCmdLists[mCurrentBufferIndex].resourceBarrier(
        *mpRenderTargetDepth->getTexture(),
        CommandListDirect::ResourceBarrierType::RenderTargetDepth,
        CommandListDirect::ResourceBarrierType::GenericRead
    );
    mpCmdLists[mCurrentBufferIndex].resourceBarrier(
        *mpSwapChainRenderTargets[mCurrentBufferIndex].getTexture(),
        CommandListDirect::ResourceBarrierType::Present,
        CommandListDirect::ResourceBarrierType::RenderTargetColor
    );

    // スワップチェーンのバッファに書き出し
    mpCmdLists[mCurrentBufferIndex].setViewport(Viewport(mpSwapChainFrameBuffers[mCurrentBufferIndex]));
    mpCmdLists[mCurrentBufferIndex].bindFrameBuffer(mpSwapChainFrameBuffers[mCurrentBufferIndex]);
    mpCmdLists[mCurrentBufferIndex].clear(mpSwapChainFrameBuffers[mCurrentBufferIndex], CommandListDirect::ClearFlag::Color, MathColor(1.f, 0.f, 0.f, 1.f), 1.f, 0);

    // @TODO: 書き出し
    mpCmdLists[mCurrentBufferIndex].bindPipeline(*mpCopyPipeline);
    mpCmdLists[mCurrentBufferIndex].setDescriptorTable(0, *mpCopyDescriptorTable);
    mpCmdLists[mCurrentBufferIndex].drawShape(*mpQuadShapeBuffer);

    mpCmdLists[mCurrentBufferIndex].resourceBarrier(
        *mpSwapChainRenderTargets[mCurrentBufferIndex].getTexture(),
        CommandListDirect::ResourceBarrierType::RenderTargetColor,
        CommandListDirect::ResourceBarrierType::Present
    );

    // コマンドリストを閉じて実行
    mpCmdLists[mCurrentBufferIndex].close();
    mpCmdQueue->execute(mpCmdLists[mCurrentBufferIndex], mCurrentBufferIndex);

    // 画面の反映
    mpSwapChain->Present(0, 0);

    // 実行待ち
    if (++mCurrentBufferIndex > (mBufferCount - 1)) { mCurrentBufferIndex = 0; }
}
#endif

// ----------------------------------------------------------------------------

bool Graphics::destroy()
{
    // バッファ破棄
    mpModelConstantBuffer.reset();
    mpModelDescriptorTable.reset();
    mpShapeBuffer.reset();
    mpCmdBundle.reset();
    mpPipeline.reset();

    mpFrameBuffer.reset();
    mpRenderTargetDepth.reset();
    mpRenderTargetColor.reset();

    mpSwapChainFrameBuffers.reset();
    mpSwapChainRenderTargets.reset();

    // 破棄
    mpCmdLists.reset();
    mpCmdQueue.reset();
    mpSwapChain.reset();
    mpDevice.reset();

    return true;
}

// ----------------------------------------------------------------------------

ICommandListDirect* Graphics::getCommandList()
{
    return &mpCmdLists[mCurrentBufferIndex];
}

// ----------------------------------------------------------------------------

FrameBuffer* Graphics::getSwapChainFrameBuffer()
{
    return &mpSwapChainFrameBuffers[mCurrentBufferIndex];
}

// ----------------------------------------------------------------------------

FrameBuffer* Graphics::getDefaultFrameBuffer()
{
    return mpFrameBuffer.get();
}

// ----------------------------------------------------------------------------

}
