﻿/*!
 * @file   balTextureD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <wrl.h>
// bal
#include <graphics/d3d12/balTextureD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>

namespace
{

constexpr DXGI_FORMAT ConvertNativeFormat(bal::gfx::d3d12::Texture::Format format)
{
    switch (format)
    {
        // Color R
        case bal::gfx::d3d12::Texture::Format::R8_UNORM:
            return DXGI_FORMAT_R8_UNORM;
        case bal::gfx::d3d12::Texture::Format::R8_SNORM:
            return DXGI_FORMAT_R8_SNORM;
        case bal::gfx::d3d12::Texture::Format::R16_FLOAT:
            return DXGI_FORMAT_R16_FLOAT;
        case bal::gfx::d3d12::Texture::Format::R32_FLOAT:
            return DXGI_FORMAT_R32_FLOAT;

        // Color RG
        case bal::gfx::d3d12::Texture::Format::R8_G8_UNORM:
            return DXGI_FORMAT_R8G8_UNORM;
        case bal::gfx::d3d12::Texture::Format::R8_G8_SNORM:
            return DXGI_FORMAT_R8G8_SNORM;
        case bal::gfx::d3d12::Texture::Format::R16_G16_FLOAT:
            return DXGI_FORMAT_R16G16_FLOAT;
        case bal::gfx::d3d12::Texture::Format::R32_G32_FLOAT:
            return DXGI_FORMAT_R32_FLOAT;

        // Color RGB
        case bal::gfx::d3d12::Texture::Format::R5_G6_B5_UNORM:
            return DXGI_FORMAT_B5G6R5_UNORM;
        case bal::gfx::d3d12::Texture::Format::R11_G11_B10_FLOAT:
            return DXGI_FORMAT_R11G11B10_FLOAT;
        case bal::gfx::d3d12::Texture::Format::R32_G32_B32_FLOAT:
            return DXGI_FORMAT_R32G32B32_FLOAT;

        // Color RGBA
        case bal::gfx::d3d12::Texture::Format::R5_G5_B5_A1_UNORM:
            return DXGI_FORMAT_B5G5R5A1_UNORM;
        case bal::gfx::d3d12::Texture::Format::R8_G8_B8_A8_UNORM:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        case bal::gfx::d3d12::Texture::Format::R8_G8_B8_A8_SNORM:
            return DXGI_FORMAT_R8G8B8A8_SNORM;
        case bal::gfx::d3d12::Texture::Format::R8_G8_B8_A8_SRGB:
            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case bal::gfx::d3d12::Texture::Format::R10_G10_B10_A2_UNORM:
            return DXGI_FORMAT_R10G10B10A2_UNORM;
        case bal::gfx::d3d12::Texture::Format::R16_G16_B16_A16_FLOAT:
            return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case bal::gfx::d3d12::Texture::Format::R32_G32_B32_A32_FLOAT:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;

        // Depth
        case bal::gfx::d3d12::Texture::Format::D16_UNORM:
            return DXGI_FORMAT_D16_UNORM;
        case bal::gfx::d3d12::Texture::Format::D24_UNORM_S8_UINT:
            return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case bal::gfx::d3d12::Texture::Format::D32_FLOAT:
            return DXGI_FORMAT_D32_FLOAT;

        // Compress
        case bal::gfx::d3d12::Texture::Format::BC1_UNORM:
            return DXGI_FORMAT_BC1_UNORM;
        case bal::gfx::d3d12::Texture::Format::BC1_SRGB:
            return DXGI_FORMAT_BC1_UNORM_SRGB;
        case bal::gfx::d3d12::Texture::Format::BC2_UNORM:
            return DXGI_FORMAT_BC2_UNORM;
        case bal::gfx::d3d12::Texture::Format::BC3_UNORM:
            return DXGI_FORMAT_BC3_UNORM;
        case bal::gfx::d3d12::Texture::Format::BC3_SRGB:
            return DXGI_FORMAT_BC3_UNORM_SRGB;
        case bal::gfx::d3d12::Texture::Format::BC4_UNORM:
            return DXGI_FORMAT_BC4_UNORM;
        case bal::gfx::d3d12::Texture::Format::BC4_SNORM:
            return DXGI_FORMAT_BC4_SNORM;
        case bal::gfx::d3d12::Texture::Format::BC5_UNORM:
            return DXGI_FORMAT_BC5_UNORM;
        case bal::gfx::d3d12::Texture::Format::BC5_SNORM:
            return DXGI_FORMAT_BC5_SNORM;
        case bal::gfx::d3d12::Texture::Format::BC6H_UF16:
            return DXGI_FORMAT_BC6H_UF16;
        case bal::gfx::d3d12::Texture::Format::BC6H_SF16:
            return DXGI_FORMAT_BC6H_SF16;
        case bal::gfx::d3d12::Texture::Format::BC7_UNORM:
            return DXGI_FORMAT_BC7_UNORM;
        case bal::gfx::d3d12::Texture::Format::BC7_SRGB:
            return DXGI_FORMAT_BC7_UNORM_SRGB;
    }
}

constexpr D3D12_RESOURCE_DIMENSION ConvertNativeDimension(bal::gfx::d3d12::Texture::Dimension dimension)
{
    switch (dimension)
    {
        case bal::gfx::d3d12::Texture::Dimension::Texture1D:
            return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
        case bal::gfx::d3d12::Texture::Dimension::Texture2D:
            return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        case bal::gfx::d3d12::Texture::Dimension::Texture3D:
            return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
    }
}

}

namespace bal::gfx::d3d12 {

// ----------------------------------------------------------------------------

bool Texture::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // 種類
    DXGI_FORMAT              format    = ConvertNativeFormat(arg.mFormat);
    D3D12_RESOURCE_DIMENSION dimension = ConvertNativeDimension(arg.mDimension);

    // すでにバッファーが存在していたら、それを使用する
    if (arg.mpBuffer)
    {
        ID3D12Resource* p_texture = reinterpret_cast<ID3D12Resource*>(arg.mpBuffer);
        mpTexture.reset(p_texture);
    }
    else
    {
        // デプスバッファーかどうか
        bool is_depth = format == DXGI_FORMAT_D16_UNORM || format == DXGI_FORMAT_D24_UNORM_S8_UINT || format == DXGI_FORMAT_D32_FLOAT;

        // テクスチャーを確保
        Microsoft::WRL::ComPtr<ID3D12Resource> p_texture;
        {
            D3D12_RESOURCE_DESC desc = {};
            desc.Dimension        = dimension;
            desc.Width            = arg.mSize.getWidth();
            desc.Height           = arg.mSize.getHeight();
            desc.DepthOrArraySize = arg.mArrayNum;
            desc.MipLevels        = arg.mMipNum;
            desc.Format           = format;
            desc.SampleDesc.Count = 1;
            desc.Flags            = is_depth ? D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL : D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

            D3D12_HEAP_PROPERTIES prop = {};
            prop.Type                 = D3D12_HEAP_TYPE_CUSTOM;
            prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE;
            prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
            prop.CreationNodeMask     = 1;
            prop.VisibleNodeMask      = 1;

            if (FAILED(p_device->CreateCommittedResource(
                &prop, D3D12_HEAP_FLAG_NONE,
                &desc, is_depth ? D3D12_RESOURCE_STATE_DEPTH_WRITE : D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr, IID_PPV_ARGS(&p_texture)
            )))
            {
                return false;
            }
        }

        // 保持
        mpTexture.reset(p_texture.Detach());
    }
    mFormat = format;

    return true;
}

// ----------------------------------------------------------------------------

}