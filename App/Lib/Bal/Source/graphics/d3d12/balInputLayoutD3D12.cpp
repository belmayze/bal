/*!
 * @file   balInputLayoutD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <container/balArray.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balInputLayoutD3D12.h>
#include <io/balFile.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------

bool InputLayout::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // 引数から頂点レイアウトを作ります
    mNumInputElementDesc = arg.mNumInputLayout;
    mpInputElementDescs  = make_unique<D3D12_INPUT_ELEMENT_DESC[]>(nullptr, mNumInputElementDesc);

    for (uint32_t i_layout = 0; i_layout < mNumInputElementDesc; ++i_layout)
    {
        const InputLayoutDesc& layout   = arg.mpInputLayouts[i_layout];
        D3D12_INPUT_ELEMENT_DESC* p_out = &mpInputElementDescs[i_layout];

        p_out->SemanticName         = layout.mName.c_str();
        p_out->SemanticIndex        = layout.mSemanticIndex;
        p_out->Format               = convertFormat_(layout.mType);
        p_out->InputSlot            = layout.mSlotIndex;
        p_out->AlignedByteOffset    = layout.mOffset;
        p_out->InputSlotClass       = layout.mInstanceDataStepRate == 0 ? D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA : D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
        p_out->InstanceDataStepRate = layout.mInstanceDataStepRate;
    }

    return true;
}

// ----------------------------------------------------------------------------

DXGI_FORMAT InputLayout::convertFormat_(Type type) const
{
    switch (type)
    {
        case Type::Int:   return DXGI_FORMAT_R32_SINT;
        case Type::IVec2: return DXGI_FORMAT_R32G32_SINT;
        case Type::IVec3: return DXGI_FORMAT_R32G32B32_SINT;
        case Type::IVec4: return DXGI_FORMAT_R32G32B32A32_SINT;
        case Type::UInt:  return DXGI_FORMAT_R32_UINT;
        case Type::UVec2: return DXGI_FORMAT_R32G32_UINT;
        case Type::UVec3: return DXGI_FORMAT_R32G32B32_UINT;
        case Type::UVec4: return DXGI_FORMAT_R32G32B32A32_UINT;
        case Type::Float: return DXGI_FORMAT_R32_FLOAT;
        case Type::Vec2:  return DXGI_FORMAT_R32G32_FLOAT;
        case Type::Vec3:  return DXGI_FORMAT_R32G32B32_FLOAT;
        case Type::Vec4:  return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }

    return DXGI_FORMAT_UNKNOWN;
}

// ----------------------------------------------------------------------------

}
