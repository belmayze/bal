/*!
 * @file   balGraphicsD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <wrl.h>
// bal
#include <graphics/balViewport.h>
#include <graphics/d3d12/balCommandListD3D12.h>
#include <graphics/d3d12/balCommandQueueD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balRenderTargetD3D12.h>
#include <graphics/d3d12/balTextureD3D12.h>

namespace bal::gfx::d3d12 {

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

    // アダプターの検索
    Microsoft::WRL::ComPtr<IDXGIAdapter1> p_adapter;
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
            break;
        }
    }

    // デバイス取得
    Microsoft::WRL::ComPtr<ID3D12Device6> p_device;
    if (FAILED(D3D12CreateDevice(p_adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&p_device))))
    {
        return false;
    }
    mpDevice.reset(p_device.Detach());

    // コマンドキュー
    mpCmdQueue = make_unique<CommandQueue>(nullptr);
    {
        CommandQueue::InitializeArg init_arg;
        init_arg.mpGraphics = this;
        init_arg.mType      = CommandQueue::Type::Graphics;
        if (!mpCmdQueue->initialize(init_arg))
        {
            return false;
        }
    }

    // コマンドリスト
    mpCmdList = make_unique<CommandList>(nullptr);
    {
        CommandList::InitializeArg init_arg;
        init_arg.mpGraphics = this;
        if (!mpCmdList->initialize(init_arg))
        {
            return false;
        }
    }

    // スワップチェーン
    Microsoft::WRL::ComPtr<IDXGISwapChain1> p_swap_chain;
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

        if (FAILED(p_factory->CreateSwapChainForHwnd(
            reinterpret_cast<CommandQueue*>(mpCmdQueue.get())->getCommandQueue(),
            arg.mHwnd,
            &desc, &fullscreen_desc,
            nullptr,
            &p_swap_chain)))
        {
            return false;
        }
    }

    // スワップバッファのテクスチャーをレンダーターゲット化
    mpSwapChainColorBuffers  = make_unique<Texture[]>(nullptr, arg.mBufferCount);
    mpSwapChainRenderTargets = make_unique<RenderTargetColor[]>(nullptr, arg.mBufferCount);
    {
        for (uint32_t i_buffer = 0; i_buffer < arg.mBufferCount; ++i_buffer)
        {
            Microsoft::WRL::ComPtr<ID3D12Resource> p_resource;
            if (FAILED(p_swap_chain->GetBuffer(i_buffer, IID_PPV_ARGS(&p_resource))))
            {
                return false;
            }

            {
                Texture::InitializeArg init_arg;
                init_arg.mpGraphics = this;
                init_arg.mpBuffer   = p_resource.Detach();
                init_arg.mFormat    = Texture::Format::R8_G8_B8_A8_UNORM;
                mpSwapChainColorBuffers[i_buffer].initialize(init_arg);
            }
            {
                RenderTargetColor::InitializeArg init_arg;
                init_arg.mpGraphics = this;
                init_arg.mpTexture  = &mpSwapChainColorBuffers[i_buffer];
                mpSwapChainRenderTargets[i_buffer].initialize(init_arg);
            }
        }
    }

    // レンダーバッファを確保
    {
        // テクスチャーを確保
        mpColorBuffer = make_unique<Texture>(nullptr);
        {
            Texture::InitializeArg init_arg;
            init_arg.mpGraphics = this;
            init_arg.mDimension = Texture::Dimension::Texture2D;
            init_arg.mFormat    = Texture::Format::R16_G16_B16_A16_FLOAT;
            init_arg.mSize      = arg.mRenderBufferSize;
            if (!mpColorBuffer->initialize(init_arg)) { return false; }
        }
        mpDepthBuffer = make_unique<Texture>(nullptr);
        {
            Texture::InitializeArg init_arg;
            init_arg.mpGraphics = this;
            init_arg.mDimension = Texture::Dimension::Texture2D;
            init_arg.mFormat    = Texture::Format::D32_FLOAT;
            init_arg.mSize      = arg.mRenderBufferSize;
            if (!mpDepthBuffer->initialize(init_arg)) { return false; }
        }

        // レンダーターゲット
        mpRenderTargetColor = make_unique<RenderTargetColor>(nullptr);
        {
            RenderTargetColor::InitializeArg init_arg;
            init_arg.mpGraphics = this;
            init_arg.mpTexture  = mpColorBuffer.get();
            if (!mpRenderTargetColor->initialize(init_arg)) { return false; }
        }
        mpRenderTargetDepth = make_unique<RenderTargetDepth>(nullptr);
        {
            RenderTargetDepth::InitializeArg init_arg;
            init_arg.mpGraphics = this;
            init_arg.mpTexture  = mpDepthBuffer.get();
            if (!mpRenderTargetDepth->initialize(init_arg)) { return false; }
        }
    }

    // 情報
    mBufferCount = arg.mBufferCount;

    // 必要な COM を保存
    mpSwapChain.reset(p_swap_chain.Detach());

    return true;
}

// ----------------------------------------------------------------------------

void Graphics::loop()
{
    mpCmdList->reset();

    mpCmdList->resourceBarrier(
        mpSwapChainColorBuffers[mCurrentBufferIndex].getTexture(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );

    Viewport vp;
    vp.setOrigin(MathVector2(0.f, 0.f));
    vp.setSize(MathVector2(640.f, 480.f));
    vp.setDepth(0.f, 1.f);
    mpCmdList->setViewport(vp);

    mpCmdList->clearColor(&mpSwapChainRenderTargets[mCurrentBufferIndex], MathColor(1.f, 0.f, 0.f, 1.f));

    mpCmdList->resourceBarrier(
        mpSwapChainColorBuffers[mCurrentBufferIndex].getTexture(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );

    mpCmdList->close();

    mpCmdQueue->execute(mpCmdList.get());

    waitForPreviousFrame();
}

// ----------------------------------------------------------------------------

void Graphics::waitForPreviousFrame()
{
    // 画面の反映
    mpSwapChain->Present(1, 0);

    // コマンドキューの終了待ち
    mpCmdQueue->waitExecuted();

    // バッファーを進める
    if (++mCurrentBufferIndex > (mBufferCount - 1)) { mCurrentBufferIndex = 0; }
}

// ----------------------------------------------------------------------------

bool Graphics::destroy()
{
    // 終了待ち
    waitForPreviousFrame();

    // バッファ破棄
    mpRenderTargetDepth.reset();
    mpRenderTargetColor.reset();
    mpDepthBuffer.reset();
    mpColorBuffer.reset();

    mpSwapChainRenderTargets.reset();
    mpSwapChainColorBuffers.reset();

    // 破棄
    mpCmdList.reset();
    mpCmdQueue.reset();
    mpSwapChain.reset();
    mpDevice.reset();

    return true;
}

// ----------------------------------------------------------------------------

}
