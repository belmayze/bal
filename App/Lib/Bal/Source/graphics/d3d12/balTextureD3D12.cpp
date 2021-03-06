﻿/*!
 * @file   balTextureD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/d3d12/balTextureD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balResourceUpdaterD3D12.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------

bool Texture::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // 種類
    DXGI_FORMAT              format      = ConvertFormat(arg.mFormat);
    bool                     is_compress = IsCompressFormat(arg.mFormat);
    D3D12_RESOURCE_DIMENSION dimension   = ConvertDimension(arg.mDimension);

    if (arg.mpGpuBuffer)
    {
        // すでにバッファーが存在していたら、それを使用する
        ID3D12Resource* p_texture = reinterpret_cast<ID3D12Resource*>(arg.mpGpuBuffer);
        mpTexture.reset(p_texture);
    }
    else
    {
        // テクスチャーを確保
        Microsoft::WRL::ComPtr<ID3D12Resource> p_texture;
        {
            bool is_depth         = format == DXGI_FORMAT_D16_UNORM || format == DXGI_FORMAT_D24_UNORM_S8_UINT || format == DXGI_FORMAT_D32_FLOAT;
            bool is_render_target = !is_compress;
            // フラグチェック
            D3D12_RESOURCE_FLAGS  resource_flag       = D3D12_RESOURCE_FLAG_NONE;
            D3D12_RESOURCE_STATES resource_state_flag = arg.mpBuffer ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_GENERIC_READ;
            D3D12_CLEAR_VALUE     clear_value;
            if (is_render_target)
            {
                if (is_depth)
                {
                    resource_flag                   |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
                    clear_value.Format               = format;
                    clear_value.DepthStencil.Depth   = 1.f;
                    clear_value.DepthStencil.Stencil = 0;
                }
                else
                {
                    resource_flag       |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
                    clear_value.Format   = format;
                    clear_value.Color[0] = 0.f;
                    clear_value.Color[1] = 0.f;
                    clear_value.Color[2] = 0.f;
                    clear_value.Color[3] = 1.f;
                }
            }

            // 列挙
            D3D12_RESOURCE_DESC desc = {};
            desc.Dimension        = dimension;
            desc.Width            = arg.mSize.getWidth();
            desc.Height           = arg.mSize.getHeight();
            desc.DepthOrArraySize = arg.mArrayNum;
            desc.MipLevels        = arg.mMipNum;
            desc.Format           = format;
            desc.SampleDesc.Count = 1;
            desc.Flags            = resource_flag;

            D3D12_HEAP_PROPERTIES prop = {};
            prop.Type                 = D3D12_HEAP_TYPE_DEFAULT;
            prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            prop.CreationNodeMask     = 1;
            prop.VisibleNodeMask      = 1;

            if (FAILED(p_device->CreateCommittedResource(
                &prop, D3D12_HEAP_FLAG_NONE,
                &desc, resource_state_flag,
                is_render_target ? &clear_value : nullptr,
                IID_PPV_ARGS(&p_texture)
            )))
            {
                return false;
            }

            if (arg.mpBuffer)
            {
                // テクスチャーバッファがあればコピーして使う
                ResourceUpdater* p_updater = reinterpret_cast<Graphics*>(arg.mpGraphics)->getResourceUpdater();
                ResourceUpdater::UpdateArg update_arg;
                update_arg.mpGraphics    = arg.mpGraphics;
                update_arg.mpDstResource = p_texture.Get();
                update_arg.mpSrcResource = arg.mpBuffer;
                update_arg.mRowPitch     = arg.mRowPitch;
                p_updater->updateResource(update_arg);
            }
        }

        // 保持
        mpTexture.reset(p_texture.Detach());
    }

    // D3D12_RESOURCE_STATE_GENERIC_READ へ
    if (!arg.mpGpuBuffer && arg.mpBuffer)
    {
        reinterpret_cast<Graphics*>(arg.mpGraphics)->getCommandList()->resourceBarrier(
            *this,
            ICommandListDirect::ResourceBarrierType::CopyDst,
            ICommandListDirect::ResourceBarrierType::GenericRead
        );
    }

    mFormat      = arg.mFormat;
    mDimension   = arg.mDimension;
    mNumArray    = arg.mArrayNum;
    mNumMipLevel = arg.mMipNum;

    return true;
}

// ----------------------------------------------------------------------------

DXGI_FORMAT Texture::ConvertFormat(Format format)
{
    switch (format)
    {
        // Color R
        case Format::R8_UNORM:  return DXGI_FORMAT_R8_UNORM;
        case Format::R8_SNORM:  return DXGI_FORMAT_R8_SNORM;
        case Format::R16_FLOAT: return DXGI_FORMAT_R16_FLOAT;
        case Format::R32_FLOAT: return DXGI_FORMAT_R32_FLOAT;

        // Color RG
        case Format::R8_G8_UNORM:   return DXGI_FORMAT_R8G8_UNORM;
        case Format::R8_G8_SNORM:   return DXGI_FORMAT_R8G8_SNORM;
        case Format::R16_G16_FLOAT: return DXGI_FORMAT_R16G16_FLOAT;
        case Format::R32_G32_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;

        // Color RGB
        case Format::R5_G6_B5_UNORM:    return DXGI_FORMAT_B5G6R5_UNORM;
        case Format::R11_G11_B10_FLOAT: return DXGI_FORMAT_R11G11B10_FLOAT;
        case Format::R32_G32_B32_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;

        // Color RGBA
        case Format::R5_G5_B5_A1_UNORM:     return DXGI_FORMAT_B5G5R5A1_UNORM;
        case Format::R8_G8_B8_A8_UNORM:     return DXGI_FORMAT_R8G8B8A8_UNORM;
        case Format::R8_G8_B8_A8_SNORM:     return DXGI_FORMAT_R8G8B8A8_SNORM;
        case Format::R8_G8_B8_A8_SRGB:      return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case Format::R10_G10_B10_A2_UNORM:  return DXGI_FORMAT_R10G10B10A2_UNORM;
        case Format::R16_G16_B16_A16_FLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case Format::R32_G32_B32_A32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;

        // Depth
        case Format::D16_UNORM:         return DXGI_FORMAT_D16_UNORM;
        case Format::D24_UNORM_S8_UINT: return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case Format::D32_FLOAT:         return DXGI_FORMAT_D32_FLOAT;

        // Compress
        case Format::BC1_UNORM: return DXGI_FORMAT_BC1_UNORM;
        case Format::BC1_SRGB:  return DXGI_FORMAT_BC1_UNORM_SRGB;
        case Format::BC2_UNORM: return DXGI_FORMAT_BC2_UNORM;
        case Format::BC3_UNORM: return DXGI_FORMAT_BC3_UNORM;
        case Format::BC3_SRGB:  return DXGI_FORMAT_BC3_UNORM_SRGB;
        case Format::BC4_UNORM: return DXGI_FORMAT_BC4_UNORM;
        case Format::BC4_SNORM: return DXGI_FORMAT_BC4_SNORM;
        case Format::BC5_UNORM: return DXGI_FORMAT_BC5_UNORM;
        case Format::BC5_SNORM: return DXGI_FORMAT_BC5_SNORM;
        case Format::BC6H_UF16: return DXGI_FORMAT_BC6H_UF16;
        case Format::BC6H_SF16: return DXGI_FORMAT_BC6H_SF16;
        case Format::BC7_UNORM: return DXGI_FORMAT_BC7_UNORM;
        case Format::BC7_SRGB:  return DXGI_FORMAT_BC7_UNORM_SRGB;
    }

    return DXGI_FORMAT_UNKNOWN;
}
// ----------------------------------------------------------------------------
Texture::Format Texture::ConvertFormatDxgi(DXGI_FORMAT format)
{
    switch (format)
    {
        // Color R
        case DXGI_FORMAT_R8_UNORM:  return Format::R8_UNORM;
        case DXGI_FORMAT_R8_SNORM:  return Format::R8_SNORM;
        case DXGI_FORMAT_R16_FLOAT: return Format::R16_FLOAT;
        case DXGI_FORMAT_R32_FLOAT: return Format::R32_FLOAT;

        // Color RG
        case DXGI_FORMAT_R8G8_UNORM:   return Format::R8_G8_UNORM;
        case DXGI_FORMAT_R8G8_SNORM:   return Format::R8_G8_SNORM;
        case DXGI_FORMAT_R16G16_FLOAT: return Format::R16_G16_FLOAT;
        case DXGI_FORMAT_R32G32_FLOAT: return Format::R32_G32_FLOAT;

        // Color RGB
        case DXGI_FORMAT_B5G6R5_UNORM:    return Format::R5_G6_B5_UNORM;
        case DXGI_FORMAT_R11G11B10_FLOAT: return Format::R11_G11_B10_FLOAT;
        case DXGI_FORMAT_R32G32B32_FLOAT: return Format::R32_G32_B32_FLOAT;

        // Color RGBA
        case DXGI_FORMAT_B5G5R5A1_UNORM:      return Format::R5_G5_B5_A1_UNORM;
        case DXGI_FORMAT_R8G8B8A8_UNORM:      return Format::R8_G8_B8_A8_UNORM;
        case DXGI_FORMAT_R8G8B8A8_SNORM:      return Format::R8_G8_B8_A8_SNORM;
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: return Format::R8_G8_B8_A8_SRGB;
        case DXGI_FORMAT_R10G10B10A2_UNORM:   return Format::R10_G10_B10_A2_UNORM;
        case DXGI_FORMAT_R16G16B16A16_FLOAT:  return Format::R16_G16_B16_A16_FLOAT;
        case DXGI_FORMAT_R32G32B32A32_FLOAT:  return Format::R32_G32_B32_A32_FLOAT;

        // Depth
        case DXGI_FORMAT_D16_UNORM:         return Format::D16_UNORM;
        case DXGI_FORMAT_D24_UNORM_S8_UINT: return Format::D24_UNORM_S8_UINT;
        case DXGI_FORMAT_D32_FLOAT:         return Format::D32_FLOAT;

        // Compress
        case DXGI_FORMAT_BC1_UNORM:      return Format::BC1_UNORM;
        case DXGI_FORMAT_BC1_UNORM_SRGB: return Format::BC1_SRGB;
        case DXGI_FORMAT_BC2_UNORM:      return Format::BC2_UNORM;
        case DXGI_FORMAT_BC3_UNORM:      return Format::BC3_UNORM;
        case DXGI_FORMAT_BC3_UNORM_SRGB: return Format::BC3_SRGB;
        case DXGI_FORMAT_BC4_UNORM:      return Format::BC4_UNORM;
        case DXGI_FORMAT_BC4_SNORM:      return Format::BC4_SNORM;
        case DXGI_FORMAT_BC5_UNORM:      return Format::BC5_UNORM;
        case DXGI_FORMAT_BC5_SNORM:      return Format::BC5_SNORM;
        case DXGI_FORMAT_BC6H_UF16:      return Format::BC6H_UF16;
        case DXGI_FORMAT_BC6H_SF16:      return Format::BC6H_SF16;
        case DXGI_FORMAT_BC7_UNORM:      return Format::BC7_UNORM;
        case DXGI_FORMAT_BC7_UNORM_SRGB: return Format::BC7_SRGB;
    }
    return Format::Unknown;
}
// ----------------------------------------------------------------------------
bool Texture::IsCompressFormat(Format format)
{
    switch (format)
    {
        case bal::ITexture::Format::R8_UNORM:
        case bal::ITexture::Format::R8_SNORM:
        case bal::ITexture::Format::R16_FLOAT:
        case bal::ITexture::Format::R32_FLOAT:
        case bal::ITexture::Format::R8_G8_UNORM:
        case bal::ITexture::Format::R8_G8_SNORM:
        case bal::ITexture::Format::R16_G16_FLOAT:
        case bal::ITexture::Format::R32_G32_FLOAT:
        case bal::ITexture::Format::R5_G6_B5_UNORM:
        case bal::ITexture::Format::R11_G11_B10_FLOAT:
        case bal::ITexture::Format::R32_G32_B32_FLOAT:
        case bal::ITexture::Format::R5_G5_B5_A1_UNORM:
        case bal::ITexture::Format::R8_G8_B8_A8_UNORM:
        case bal::ITexture::Format::R8_G8_B8_A8_SNORM:
        case bal::ITexture::Format::R8_G8_B8_A8_SRGB:
        case bal::ITexture::Format::R10_G10_B10_A2_UNORM:
        case bal::ITexture::Format::R16_G16_B16_A16_FLOAT:
        case bal::ITexture::Format::R32_G32_B32_A32_FLOAT:
        case bal::ITexture::Format::D16_UNORM:
        case bal::ITexture::Format::D24_UNORM_S8_UINT:
        case bal::ITexture::Format::D32_FLOAT:
            return false;

        case bal::ITexture::Format::BC1_UNORM:
        case bal::ITexture::Format::BC1_SRGB:
        case bal::ITexture::Format::BC2_UNORM:
        case bal::ITexture::Format::BC3_UNORM:
        case bal::ITexture::Format::BC3_SRGB:
        case bal::ITexture::Format::BC4_UNORM:
        case bal::ITexture::Format::BC4_SNORM:
        case bal::ITexture::Format::BC5_UNORM:
        case bal::ITexture::Format::BC5_SNORM:
        case bal::ITexture::Format::BC6H_UF16:
        case bal::ITexture::Format::BC6H_SF16:
        case bal::ITexture::Format::BC7_UNORM:
        case bal::ITexture::Format::BC7_SRGB:
            return true;
    }
    return false;
}
// ----------------------------------------------------------------------------
D3D12_RESOURCE_DIMENSION Texture::ConvertDimension(Dimension dimension)
{
    switch (dimension)
    {
        case ITexture::Dimension::Texture1D: return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
        case ITexture::Dimension::Texture2D: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        case ITexture::Dimension::Texture3D: return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
    }

    return D3D12_RESOURCE_DIMENSION_UNKNOWN;
}
Texture::Dimension Texture::ConvertDimensionD3D12(D3D12_RESOURCE_DIMENSION dimension)
{
    switch (dimension)
    {
        case D3D12_RESOURCE_DIMENSION_TEXTURE1D: return ITexture::Dimension::Texture1D;
        case D3D12_RESOURCE_DIMENSION_TEXTURE2D: return ITexture::Dimension::Texture2D;
        case D3D12_RESOURCE_DIMENSION_TEXTURE3D: return ITexture::Dimension::Texture3D;
    }
    return ITexture::Dimension::Unknown;
}

// ----------------------------------------------------------------------------

}
