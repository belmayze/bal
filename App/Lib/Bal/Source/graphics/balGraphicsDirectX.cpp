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
    if (FAILED(D3D12CreateDevice(p_adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&p_device))))
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
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;

        if (FAILED(p_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&p_cmd_queue))))
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
        desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
        desc.BufferCount        = 2;
        desc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;

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

    // デスクリプターヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap;
    UINT descriptor_handle_size;
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = 2;
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        if (FAILED(p_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
        {
            return false;
        }

        // サイズを取得
        descriptor_handle_size = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // コマンドリスト
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> p_cmd_list;
    {
        if (FAILED(p_device->CreateCommandList(
            1, D3D12_COMMAND_LIST_TYPE_DIRECT,
            p_cmd_allocator.Get(),
            nullptr,
            IID_PPV_ARGS(&p_cmd_list))))
        {
            return false;
        }
    }

    // バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource1> p_rtv_buffers[2];
    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = p_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
    {
        D3D12_CPU_DESCRIPTOR_HANDLE handle_tmp = rtv_handle;
        for (int i_buffer = 0; i_buffer < 2; ++i_buffer)
        {
            if (FAILED(p_swap_chain->GetBuffer(i_buffer, IID_PPV_ARGS(&p_rtv_buffers[i_buffer]))))
            {
                return false;
            }

            p_device->CreateRenderTargetView(p_rtv_buffers[i_buffer].Get(), nullptr, handle_tmp);
            handle_tmp.ptr += descriptor_handle_size;
        }
    }

    // フェンス
    Microsoft::WRL::ComPtr<ID3D12Fence> p_fence;
    HANDLE event_handle = CreateEvent(0, FALSE, FALSE, 0);
    {
        if (FAILED(p_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&p_fence))))
        {
            return false;
        }
    }

    // 必要な COM を保存
    mpDevice.reset(p_device.Detach());
    mpSwapChain.reset(p_swap_chain.Detach());
    mpCmdAllocator.reset(p_cmd_allocator.Detach());
    mpCmdQueue.reset(p_cmd_queue.Detach());
    mpCmdList.reset(p_cmd_list.Detach());
    mpDescriptorHeap.reset(p_descriptor_heap.Detach());
    mpRtvBuffers[0].reset(p_rtv_buffers[0].Detach());
    mpRtvBuffers[1].reset(p_rtv_buffers[1].Detach());
    mpFence.reset(p_fence.Detach());
    mRtvHandle = rtv_handle;
    mRtvHandleSize = descriptor_handle_size;
    mEventHandle = event_handle;

    return true;
}

// ----------------------------------------------------------------------------

void DirectX::loop()
{
    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = mRtvHandle;
    rtv_handle.ptr += mCurrentBufferIndex * mRtvHandleSize;

    {
        D3D12_RESOURCE_BARRIER desc = {};
        desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        desc.Transition.pResource   = mpRtvBuffers[mCurrentBufferIndex].get();
        desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        desc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        desc.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
        mpCmdList->ResourceBarrier(1, &desc);
    }

    D3D12_VIEWPORT vp;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width    = 640.f;
    vp.Height   = 480.f;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    mpCmdList->RSSetViewports(1, &vp);

    float clear_color[] = { 1.f, 0.f, 0.f, 1.f };
    mpCmdList->ClearRenderTargetView(rtv_handle, clear_color, 0U, nullptr);

    {
        D3D12_RESOURCE_BARRIER desc = {};
        desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        desc.Transition.pResource   = mpRtvBuffers[mCurrentBufferIndex].get();
        desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        desc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        desc.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
        mpCmdList->ResourceBarrier(1, &desc);
    }

    mpCmdList->Close();
    ID3D12CommandList* cmd_lists[] = { mpCmdList.get() };
    mpCmdQueue->ExecuteCommandLists(1, cmd_lists);

    mpFence->Signal(0);
    mpFence->SetEventOnCompletion(1, mEventHandle);
    mpCmdQueue->Signal(mpFence.get(), 1);
    WaitForSingleObject(mEventHandle, INFINITE);

    mpSwapChain->Present(0, 0);
    if (++mCurrentBufferIndex > 1) { mCurrentBufferIndex = 0; }

    mpCmdAllocator->Reset();
    mpCmdList->Reset(mpCmdAllocator.get(), nullptr);
}

// ----------------------------------------------------------------------------

bool DirectX::destroy()
{
    // 終了待ち
    mpCmdList->Close();
    ID3D12CommandList* cmd_lists[] = { mpCmdList.get() };
    mpCmdQueue->ExecuteCommandLists(1, cmd_lists);

    mpFence->Signal(0);
    mpFence->SetEventOnCompletion(1, mEventHandle);
    mpCmdQueue->Signal(mpFence.get(), 1);
    WaitForSingleObject(mEventHandle, INFINITE);

    mpSwapChain->Present(0, 0);

    // 破棄
    mpFence.reset();
    mpDescriptorHeap.reset();
    mpCmdList.reset();
    mpCmdQueue.reset();
    mpCmdAllocator.reset();
    mpSwapChain.reset();
    //mpRtvBuffers[0].reset();
    //mpRtvBuffers[1].reset();
    mpDevice.reset();

    return true;
}

// ----------------------------------------------------------------------------

}
