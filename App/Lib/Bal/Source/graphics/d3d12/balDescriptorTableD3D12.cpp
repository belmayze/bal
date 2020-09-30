/*!
 * @file   balDescriptorTableD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/d3d12/balConstantBufferD3D12.h>
#include <graphics/d3d12/balDescriptorTableD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balTextureD3D12.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------

bool DescriptorTable::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // デスクリプターヒープを作成
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap;
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = arg.mNumTexture + arg.mNumConstantBuffer;
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        if (FAILED(p_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
        {
            return false;
        }
    }

    // デスクリプターの設定
    D3D12_CPU_DESCRIPTOR_HANDLE handle = p_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
    uint32_t increment_size = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // テクスチャー
    for (uint32_t i_texture = 0; i_texture < arg.mNumTexture; ++i_texture)
    {
        const Texture* p_texture = reinterpret_cast<const Texture*>(arg.mpTextures[i_texture]);

        // 次元を取得
        D3D12_SRV_DIMENSION dimension;
        switch (p_texture->getDimension())
        {
            case Texture::Dimension::Texture1D:
                dimension = D3D12_SRV_DIMENSION_TEXTURE1D;
                break;
            case Texture::Dimension::Texture2D:
                dimension = p_texture->isCubemap() ? D3D12_SRV_DIMENSION_TEXTURECUBE : D3D12_SRV_DIMENSION_TEXTURE2D;
                break;
            case Texture::Dimension::Texture3D:
                dimension = D3D12_SRV_DIMENSION_TEXTURE3D;
                break;
        }
        if (p_texture->getNumArray() > 1)
        {
            switch (dimension)
            {
                case D3D12_SRV_DIMENSION_TEXTURE1D:
                    dimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
                    break;
                case D3D12_SRV_DIMENSION_TEXTURE2D:
                    dimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
                    break;
                case D3D12_SRV_DIMENSION_TEXTURECUBE:
                    dimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
                    break;
            }
        }

        D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
        desc.ViewDimension           = dimension;
        desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        desc.Format                  = Texture::ConvertFormat(p_texture->getFormat());

        switch (dimension)
        {
            case D3D12_SRV_DIMENSION_TEXTURE1D:
                desc.Texture1D.MipLevels = p_texture->getNumMipLevel();
                break;
            case D3D12_SRV_DIMENSION_TEXTURE1DARRAY:
                desc.Texture1DArray.MipLevels = p_texture->getNumMipLevel();
                desc.Texture1DArray.ArraySize = p_texture->getNumArray();
                break;
            case D3D12_SRV_DIMENSION_TEXTURE2D:
                desc.Texture2D.MipLevels = p_texture->getNumMipLevel();
                break;
            case D3D12_SRV_DIMENSION_TEXTURE2DARRAY:
                desc.Texture2DArray.MipLevels = p_texture->getNumMipLevel();
                desc.Texture2DArray.ArraySize = p_texture->getNumArray();
                break;
            case D3D12_SRV_DIMENSION_TEXTURE3D:
                desc.Texture3D.MipLevels = p_texture->getNumMipLevel();
                break;
            case D3D12_SRV_DIMENSION_TEXTURECUBE:
                desc.TextureCube.MipLevels = p_texture->getNumMipLevel();
                break;
            case D3D12_SRV_DIMENSION_TEXTURECUBEARRAY:
                desc.TextureCubeArray.MipLevels = p_texture->getNumMipLevel();
                desc.TextureCubeArray.NumCubes  = p_texture->getNumArray();
                break;
        }

        p_device->CreateShaderResourceView(p_texture->getTexture(), &desc, handle);
        handle.ptr += increment_size;
    }

    // 定数バッファ
    for (uint32_t i_constant = 0; i_constant < arg.mNumConstantBuffer; ++i_constant)
    {
        const ConstantBuffer* p_constant_buffer = reinterpret_cast<const ConstantBuffer*>(arg.mpConstantBuffers[i_constant]);

        D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
        desc.BufferLocation = p_constant_buffer->getGPUVirtualAddress();
        desc.SizeInBytes    = static_cast<uint32_t>(Math::Ceil(p_constant_buffer->getBufferSize(), 256LLU));

        p_device->CreateConstantBufferView(&desc, handle);
        handle.ptr += increment_size;
    }

    // 保持
    mpDescriptorHeap.reset(p_descriptor_heap.Detach());

    return true;
}

// ----------------------------------------------------------------------------

}
