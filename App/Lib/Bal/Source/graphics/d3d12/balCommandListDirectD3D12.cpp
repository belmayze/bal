﻿/*!
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
#include <graphics/d3d12/balCommandListBundleD3D12.h>
#include <graphics/d3d12/balCommandListDirectD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balRenderTargetD3D12.h>
#include <graphics/d3d12/balTextureD3D12.h>

namespace bal::gfx::d3d12 {

// ----------------------------------------------------------------------------

bool CommandListDirect::initialize(const InitializeArg& arg)
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

void CommandListDirect::reset()
{
    mpCmdAllocator->Reset();
    mpCmdList->Reset(mpCmdAllocator.get(), nullptr);
}

// ----------------------------------------------------------------------------

void CommandListDirect::close()
{
    mpCmdList->Close();
}

// ----------------------------------------------------------------------------

void CommandListDirect::setViewport(const Viewport& vp)
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

void CommandListDirect::bindFrameBuffer(const FrameBuffer& frame_buffer)
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

void CommandListDirect::clear(const FrameBuffer& frame_buffer, uint32_t clear_flag, const MathColor& color, float depth, uint8_t stencil)
{
    // カラー
    if ((clear_flag & ClearFlag::Color) != 0)
    {
        for (const IRenderTargetColor* p_render_target : frame_buffer.getRenderTargetColors())
        {
            if (!p_render_target) { break; }

            mpCmdList->ClearRenderTargetView(reinterpret_cast<const RenderTargetColor*>(p_render_target)->getHandle(), color, 0U, nullptr);
        }
    }

    // デプス
    if (frame_buffer.getRenderTargetDepth() && (clear_flag & (ClearFlag::Depth | ClearFlag::Stencil)) != 0)
    {
        const RenderTargetDepth* p_render_target = reinterpret_cast<const RenderTargetDepth*>(frame_buffer.getRenderTargetDepth());

        D3D12_CLEAR_FLAGS flag = static_cast<D3D12_CLEAR_FLAGS>(0);
        if ((clear_flag & ClearFlag::Depth) != 0)   { flag |= D3D12_CLEAR_FLAG_DEPTH; }
        if ((clear_flag & ClearFlag::Stencil) != 0) { flag |= D3D12_CLEAR_FLAG_STENCIL; }

        mpCmdList->ClearDepthStencilView(p_render_target->getHandle(), flag, depth, stencil, 0U, nullptr);
    }
}

// ----------------------------------------------------------------------------

void CommandListDirect::resourceBarrier(const ITexture& texture, int before_status, int after_status)
{
    const Texture* p_texture = reinterpret_cast<const Texture*>(&texture);
    resourceBarrier_(p_texture->getTexture(), before_status, after_status);
}

// ----------------------------------------------------------------------------

void CommandListDirect::executeBundle(const ICommandListBundle* p_cmd_bundle)
{
    const CommandListBundle* p_bundle = reinterpret_cast<const CommandListBundle*>(p_cmd_bundle);
    mpCmdList->ExecuteBundle(p_bundle->getCommandList());
}

// ----------------------------------------------------------------------------

void CommandListDirect::resourceBarrier_(ID3D12Resource* p_resource, int before_status, int after_status)
{
    D3D12_RESOURCE_BARRIER desc = {};
    desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    desc.Transition.pResource   = p_resource;
    desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    desc.Transition.StateBefore = static_cast<D3D12_RESOURCE_STATES>(before_status);
    desc.Transition.StateAfter  = static_cast<D3D12_RESOURCE_STATES>(after_status);
    mpCmdList->ResourceBarrier(1, &desc);
}

// ----------------------------------------------------------------------------

}