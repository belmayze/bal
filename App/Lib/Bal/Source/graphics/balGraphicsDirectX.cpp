/*!
 * @file   balHeapBlock.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <wrl.h>
// bal
#include <graphics/balGraphicsDirectX.h>

namespace bal::gfx {

// ----------------------------------------------------------------------------

bool DirectX::initialize(const InitializeArg& arg)
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

    // デバイス取得
    Microsoft::WRL::ComPtr<ID3D12Device> p_device;
    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&p_device))))
    {
        return false;
    }

    // コマンドアロケーター
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> p_cmd_allocator;
    if (FAILED(p_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&p_cmd_allocator))))
    {
        return false;
    }

    // グラフィックスコマンド
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> p_cmd_queue;
    {
        D3D12_COMMAND_QUEUE_DESC desc;
        desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Priority = 0;
        desc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0U;

        if (FAILED(p_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&p_cmd_queue))))
        {
            return false;
        }
    }

    // スワップチェーン
    Microsoft::WRL::ComPtr<IDXGISwapChain1> p_swap_chain;
    {
        DXGI_SWAP_CHAIN_DESC1 desc;
        desc.Width                   = arg.mRenderBufferSize.getWidth();
        desc.Height                  = arg.mRenderBufferSize.getHeight();
        desc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Stereo                  = FALSE;
        desc.SampleDesc.Count                   = 1;
        desc.SampleDesc.Quality                 = 0;
        desc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
        desc.BufferCount                        = 2;
        desc.Scaling                 = DXGI_SCALING_STRETCH;
        desc.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.AlphaMode               = DXGI_ALPHA_MODE_UNSPECIFIED;
        desc.Flags                              = 0U;

        if (FAILED(p_factory->CreateSwapChainForHwnd(
            p_cmd_queue.Get(),
            arg.mHwnd,
            &desc,
            nullptr, nullptr,
            &p_swap_chain)))
        {
            return false;
        }
    }

    return true;
}

// ----------------------------------------------------------------------------

bool DirectX::destroy()
{
    return true;
}

// ----------------------------------------------------------------------------

}
