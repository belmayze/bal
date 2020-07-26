/*!
 * @file   balCommandListD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <wrl.h>
// bal
#include <graphics/balViewport.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balCommandListD3D12.h>

namespace bal::gfx::d3d12 {

// ----------------------------------------------------------------------------

bool CommandList::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // 種類
    D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    // アロケーター
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> p_cmd_allocator;
    if (FAILED(p_device->CreateCommandAllocator(type, IID_PPV_ARGS(&p_cmd_allocator))))
    {
        return false;
    }

    // コマンドリスト
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> p_cmd_list;
    if (FAILED(p_device->CreateCommandList(1, type, p_cmd_allocator.Get(), nullptr, IID_PPV_ARGS(&p_cmd_list))))
    {
        return false;
    }
    p_cmd_list->Close();

    // 保持
    mpCmdAllocator.reset(p_cmd_allocator.Detach());
    mpCmdList.reset(p_cmd_list.Detach());

    return true;
}

// ----------------------------------------------------------------------------

void CommandList::reset()
{
    mpCmdAllocator->Reset();
    mpCmdList->Reset(mpCmdAllocator.get(), nullptr);
}

// ----------------------------------------------------------------------------

void CommandList::close()
{
    mpCmdList->Close();
}

// ----------------------------------------------------------------------------

void CommandList::setViewport(const Viewport& vp)
{
    D3D12_VIEWPORT viewport;
    viewport.TopLeftX = vp.getOrigin().getX();
    viewport.TopLeftY = vp.getOrigin().getY();
    viewport.Width    = vp.getSize().getX();
    viewport.Height   = vp.getSize().getY();
    viewport.MinDepth = vp.getMinDepth();
    viewport.MaxDepth = vp.getMaxDepth();
    mpCmdList->RSSetViewports(1, &viewport);
}

// ----------------------------------------------------------------------------

void CommandList::clearColor(void* p_handle, const MathColor& color)
{
    mpCmdList->ClearRenderTargetView(*reinterpret_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(p_handle), color, 0U, nullptr);
}

// ----------------------------------------------------------------------------

void CommandList::resourceBarrier(void* p_resource, int before_status, int after_status)
{
    D3D12_RESOURCE_BARRIER desc = {};
    desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    desc.Transition.pResource   = reinterpret_cast<ID3D12Resource*>(p_resource);
    desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    desc.Transition.StateBefore = static_cast<D3D12_RESOURCE_STATES>(before_status);
    desc.Transition.StateAfter  = static_cast<D3D12_RESOURCE_STATES>(after_status);
    mpCmdList->ResourceBarrier(1, &desc);
}

// ----------------------------------------------------------------------------

}
