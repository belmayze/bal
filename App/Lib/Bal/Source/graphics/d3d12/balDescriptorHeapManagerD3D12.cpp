/*!
 * @file   balDescriptorHeapManagerD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/d3d12/balDescriptorHeapManagerD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------

bool DescriptorHeapManager::initialize(const InitializeArg& arg)
{
    // デスクリプターヒープは同時に複数のヒープを設定することができない
    // また、ヒープの切り替え処理は重いため1フレーム中に何度も呼ぶべきではない
    //
    // そのため大量のヒープをあらかじめ確保しておいて、それをそのまま使用することにします
    // 実際には DescriptorTable でオフセットを覚えておき設定します

    // デバイス
    ID3D12Device6* p_device = static_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // デスクリプターヒープを作成
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_cbv_srv_uav;
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = arg.mNumCbvSrvUav;
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        if (FAILED(p_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_cbv_srv_uav))))
        {
            return false;
        }
    }

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_sampler;
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = arg.mNumSampler;
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        if (FAILED(p_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_sampler))))
        {
            return false;
        }
    }

    // 保持
    mpCbvSrvUavDescriptorHeap.reset(p_cbv_srv_uav.Detach());
    mpSamplerDescriptorHeap.reset(p_sampler.Detach());

    return true;
}

// ----------------------------------------------------------------------------

}
