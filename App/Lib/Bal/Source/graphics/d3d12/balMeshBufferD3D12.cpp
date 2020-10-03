/*!
 * @file   balShapeBufferD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balMeshBufferD3D12.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------

bool MeshBuffer::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // 頂点バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> p_vertex_buffer;
    {
        // ヒープ設定
        D3D12_HEAP_PROPERTIES prop = {};
        prop.Type             = D3D12_HEAP_TYPE_UPLOAD;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask  = 1;

        // リソース設定
        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width            = arg.mVertexBufferSize;
        desc.Height           = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels        = 1;
        desc.SampleDesc.Count = 1;
        desc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // 作成
        if (FAILED(p_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&p_vertex_buffer))))
        {
            return false;
        }

        // データを転送
        uint8_t* p_map = nullptr;
        if (FAILED(p_vertex_buffer->Map(0, nullptr, reinterpret_cast<void**>(&p_map))))
        {
            return false;
        }
        std::memcpy(p_map, arg.mpVertexBuffer, arg.mVertexBufferSize);
        p_vertex_buffer->Unmap(0, nullptr);
    }

    // インデックスバッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> p_index_buffer;
    {
        // ヒープ設定
        D3D12_HEAP_PROPERTIES prop = {};
        prop.Type             = D3D12_HEAP_TYPE_UPLOAD;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask  = 1;

        // リソース設定
        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width            = arg.mIndexBufferSize;
        desc.Height           = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels        = 1;
        desc.SampleDesc.Count = 1;
        desc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // 作成
        if (FAILED(p_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&p_index_buffer))))
        {
            return false;
        }

        // データを転送
        uint8_t* p_map = nullptr;
        if (FAILED(p_index_buffer->Map(0, nullptr, reinterpret_cast<void**>(&p_map))))
        {
            return false;
        }
        std::memcpy(p_map, arg.mpIndexBuffer, arg.mIndexBufferSize);
        p_index_buffer->Unmap(0, nullptr);
    }

    // ビューの設定
    DXGI_FORMAT index_buffer_format = DXGI_FORMAT_R16_UINT;
    if (arg.mIndexBufferFormat == IndexBufferFormat::Uint32) { index_buffer_format = DXGI_FORMAT_R32_UINT; }

    mVertexBufferView.BufferLocation = p_vertex_buffer->GetGPUVirtualAddress();
    mVertexBufferView.SizeInBytes    = static_cast<uint32_t>(arg.mVertexBufferSize);
    mVertexBufferView.StrideInBytes  = static_cast<uint32_t>(arg.mVertexStride);

    mIndexBufferView.BufferLocation  = p_index_buffer->GetGPUVirtualAddress();
    mIndexBufferView.SizeInBytes     = static_cast<uint32_t>(arg.mIndexBufferSize);
    mIndexBufferView.Format          = index_buffer_format;

    // 保持
    mpVertexBuffer.reset(p_vertex_buffer.Detach());
    mpIndexBuffer.reset(p_index_buffer.Detach());
    mIndexCount        = arg.mIndexCount;
    mPrimitiveTopology = arg.mPrimitiveTopology;

    return true;
}

// ----------------------------------------------------------------------------

}
