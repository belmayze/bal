/*!
 * @file   balConstantBufferD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/d3d12/balConstantBufferD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------

bool ConstantBuffer::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // メモリー確保
    mpResources  = make_unique<std::unique_ptr<ID3D12Resource, ComDeleter>[]>(nullptr, arg.mBufferCount);
    mpBufferPtrs = make_unique<uint8_t* []>(nullptr, arg.mBufferCount);

    // 定数バッファを作成
    for (uint32_t i_buffer = 0; i_buffer < arg.mBufferCount; ++i_buffer)
    {
        Microsoft::WRL::ComPtr<ID3D12Resource> p_resource;
        {
            D3D12_HEAP_PROPERTIES prop = {};
            prop.Type             = D3D12_HEAP_TYPE_UPLOAD;
            prop.CreationNodeMask = 1;
            prop.VisibleNodeMask  = 1;

            D3D12_RESOURCE_DESC desc = {};
            desc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
            desc.Width            = static_cast<uint32_t>(Math::Ceil(arg.mBufferSize, static_cast<size_t>(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT)));
            desc.Height           = 1;
            desc.DepthOrArraySize = 1;
            desc.MipLevels        = 1;
            desc.SampleDesc.Count = 1;
            desc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

            // 作成
            if (FAILED(p_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&p_resource))))
            {
                return false;
            }
        }

        // CPUメモリーを取得
        // 定数バッファは、終了するまで Unmap する必要はないとのこと
        if (FAILED(p_resource->Map(0, nullptr, reinterpret_cast<void**>(&mpBufferPtrs[i_buffer]))))
        {
            return false;
        }

        // 保持
        mpResources[i_buffer].reset(p_resource.Detach());
    }

    // 保持
    mBufferSize = arg.mBufferSize;

    return true;
}

// ----------------------------------------------------------------------------

void ConstantBuffer::flush()
{
    // @TODO: 必要なはず
}

// ----------------------------------------------------------------------------

void ConstantBuffer::swapBuffer()
{
    if (++mBufferIndex >= mBufferCount) { mBufferIndex = 0; }
}

// ----------------------------------------------------------------------------

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::getGPUVirtualAddress() const
{
    return mpResources[mBufferIndex]->GetGPUVirtualAddress();
}

// ----------------------------------------------------------------------------

size_t ConstantBuffer::getBufferSize() const
{
    return mBufferSize;
}

// ----------------------------------------------------------------------------

uint8_t* ConstantBuffer::getBufferPtr_()
{
    return mpBufferPtrs[mBufferIndex];
}

// ----------------------------------------------------------------------------

}
