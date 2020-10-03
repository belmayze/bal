﻿/*!
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
#include <graphics/d3d12/balCommandListDirectD3D12.h>
#include <graphics/d3d12/balCommandQueueD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
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

// ----------------------------------------------------------------------------

bool Graphics::destroy()
{
    // バッファ破棄
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

}
