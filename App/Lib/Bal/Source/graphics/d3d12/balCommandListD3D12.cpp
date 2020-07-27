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
#include <container/balArray.h>
#include <graphics/balFrameBuffer.h>
#include <graphics/balViewport.h>
#include <graphics/d3d12/balCommandListD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balRenderTargetD3D12.h>

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

void CommandList::bindFrameBuffer(const FrameBuffer& frame_buffer)
{
    // レンダーターゲット取得
    const Array<IRenderTargetColor*, 8>& render_target_colors  = frame_buffer.getRenderTargetColors();
    const IRenderTargetDepth*            p_render_target_depth = frame_buffer.getRenderTargetDepth();

    // 設定されているバッファーをチェック
    // 配列に連続して入る必要があります。以下の場合は 1 つとしてカウントします（シェーダーの index とずれるため）
    //（{TargetA, nullptr, TargetB, nullptr, ...}）
    int num_color = 0;
    Array<D3D12_CPU_DESCRIPTOR_HANDLE, 8> color_handles;
    for (const IRenderTargetColor* p_render_target : render_target_colors)
    {
        if (!p_render_target) { break; }
        color_handles[num_color] = reinterpret_cast<const RenderTargetColor*>(p_render_target)->getHandle();
        ++num_color;
    }
    D3D12_CPU_DESCRIPTOR_HANDLE depth_handle;
    if (p_render_target_depth) { depth_handle = reinterpret_cast<const RenderTargetDepth*>(p_render_target_depth)->getHandle(); }

    mpCmdList->OMSetRenderTargets(num_color, color_handles.data(), FALSE, p_render_target_depth ? &depth_handle : nullptr);
}

// ----------------------------------------------------------------------------

void CommandList::clearColor(IRenderTargetColor* p_render_target, const MathColor& color)
{
    RenderTargetColor* p_rtv = reinterpret_cast<RenderTargetColor*>(p_render_target);
    mpCmdList->ClearRenderTargetView(p_rtv->getHandle(), color, 0U, nullptr);
}

// ----------------------------------------------------------------------------

void CommandList::clearDepthStencil(IRenderTargetDepth* p_render_target, DepthClearFlag clear_flag, float depth, uint32_t stencil)
{
    RenderTargetDepth* p_dsv = reinterpret_cast<RenderTargetDepth*>(p_render_target);

    D3D12_CLEAR_FLAGS flag;
    switch (clear_flag)
    {
        case DepthClearFlag::Depth:           flag = D3D12_CLEAR_FLAG_DEPTH; break;
        case DepthClearFlag::Stencil:         flag = D3D12_CLEAR_FLAG_STENCIL; break;
        case DepthClearFlag::DepthAndStencil: flag = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL; break;
    }

    mpCmdList->ClearDepthStencilView(p_dsv->getHandle(), flag, depth, stencil, 0, nullptr);
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
