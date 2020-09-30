/*!
 * @file   balRenderTargetD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balRenderTargetD3D12.h>
#include <graphics/d3d12/balTextureD3D12.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------

bool RenderTargetColor::initialize(const InitializeArg& arg, std::unique_ptr<ITexture>&& texture)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // テクスチャー
    Texture* p_texture = reinterpret_cast<Texture*>(texture.get());

    // デスクリプターヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap;
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = 1;
        if (FAILED(p_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
        {
            return false;
        }
    }

    // レンダーターゲット
    {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format        = Texture::ConvertFormat(p_texture->getFormat());
        desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        p_device->CreateRenderTargetView(p_texture->getTexture(), &desc, p_descriptor_heap->GetCPUDescriptorHandleForHeapStart());
    }

    // 保持
    mpTexture = std::move(texture);
    mpDescriptorHeap.reset(p_descriptor_heap.Detach());

    return true;
}

// ----------------------------------------------------------------------------

bool RenderTargetDepth::initialize(const InitializeArg& arg, std::unique_ptr<ITexture>&& texture)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // テクスチャー
    Texture* p_texture = reinterpret_cast<Texture*>(texture.get());

    // デスクリプターヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap;
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        desc.NumDescriptors = 1;
        if (FAILED(p_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
        {
            return false;
        }
    }

    // レンダーターゲット
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
        desc.Format        = Texture::ConvertFormat(p_texture->getFormat());
        desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        p_device->CreateDepthStencilView(p_texture->getTexture(), &desc, p_descriptor_heap->GetCPUDescriptorHandleForHeapStart());
    }

    // 保持
    mpTexture = std::move(texture);
    mpDescriptorHeap.reset(p_descriptor_heap.Detach());

    return true;
}

// ----------------------------------------------------------------------------

}
