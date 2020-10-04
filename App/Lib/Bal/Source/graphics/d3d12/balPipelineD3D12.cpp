/*!
 * @file   balPipelineD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <container/balArray.h>
#include <graphics/d3d12/balDescriptorHeapD3D12.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balInputLayoutD3D12.h>
#include <graphics/d3d12/balPipelineD3D12.h>
#include <graphics/d3d12/balTextureD3D12.h>
#include <io/balFile.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------

bool Pipeline::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // レイアウト
    const InputLayout* p_input_layout = reinterpret_cast<const InputLayout*>(arg.mpInputLayout);

    // @TODO: いろいろ固定してあるので、変更する

    // ルートシグネチャ
    // 参考: https://sites.google.com/site/monshonosuana/directxno-hanashi-1/directx-145
    Microsoft::WRL::ComPtr<ID3D12RootSignature> p_root_signature;
    {
        // デスクリプターレンジ（最大数を確保しておく）
        // 1つのデスクリプターヒープに対して、最大2つまで
        std::unique_ptr<D3D12_DESCRIPTOR_RANGE1[]> ranges = make_unique<D3D12_DESCRIPTOR_RANGE1[]>(nullptr, arg.mNumDescriptorHeap * 2);
        uint32_t i_range = 0;

        // デスクリプターテーブル
        std::unique_ptr<D3D12_ROOT_PARAMETER1[]> params = make_unique<D3D12_ROOT_PARAMETER1[]>(nullptr, arg.mNumDescriptorHeap);
        for (uint32_t i_heap = 0; i_heap < arg.mNumDescriptorHeap; ++i_heap)
        {
            D3D12_ROOT_PARAMETER1* p_param           = &params[i_heap];
            const IDescriptorHeap* p_descriptor_heap = arg.mpDescriptorHeaps[i_heap];

            // デスクリプターテーブル
            p_param->ParameterType    = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            p_param->ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

            // デスクリプターレンジはテクスチャーと定数バッファで1つずつ
            D3D12_DESCRIPTOR_RANGE1* p_range_base         = &ranges[i_range];
            uint32_t                 num_descriptor_range = 0;

            // 定数バッファのレンジ
            if (p_descriptor_heap->getNumConstantBuffer() > 0)
            {
                D3D12_DESCRIPTOR_RANGE1* p_range = &ranges[i_range++];
                num_descriptor_range++;
                p_range->RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                p_range->NumDescriptors                    = p_descriptor_heap->getNumConstantBuffer();
                p_range->BaseShaderRegister                = p_descriptor_heap->getConstantRangeBase();
                p_range->RegisterSpace                     = 0;
                p_range->Flags                             = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
                p_range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            }
            // テクスチャーのレンジ
            if (p_descriptor_heap->getNumTexture() > 0)
            {
                D3D12_DESCRIPTOR_RANGE1* p_range = &ranges[i_range++];
                num_descriptor_range++;
                p_range->RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                p_range->NumDescriptors                    = p_descriptor_heap->getNumTexture();
                p_range->BaseShaderRegister                = p_descriptor_heap->getTextureRangeBase();
                p_range->RegisterSpace                     = 0;
                p_range->Flags                             = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
                p_range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            }
            // パラメーター設定
            p_param->DescriptorTable.NumDescriptorRanges = num_descriptor_range;
            p_param->DescriptorTable.pDescriptorRanges   = p_range_base;
        }

        // ルートシグネチャ
        D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc = {};
        desc.Version                = D3D_ROOT_SIGNATURE_VERSION_1_1;
        desc.Desc_1_1.Flags         = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        desc.Desc_1_1.NumParameters = arg.mNumDescriptorHeap;
        desc.Desc_1_1.pParameters   = params.get();

        // サンプラーは固定で使用する
        // @TODO: 複数使用できるようにしておく
        D3D12_STATIC_SAMPLER_DESC sampler_desc = {};
        sampler_desc.Filter           = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler_desc.AddressV         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler_desc.AddressW         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler_desc.ComparisonFunc   = D3D12_COMPARISON_FUNC_NEVER;
        sampler_desc.MaxLOD           = std::numeric_limits<float>::max();
        sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        desc.Desc_1_1.NumStaticSamplers = 1;
        desc.Desc_1_1.pStaticSamplers   = &sampler_desc;

        // 設定をシリアライズして生成
        Microsoft::WRL::ComPtr<ID3DBlob> p_root_signature_serialized;
        if (FAILED(D3D12SerializeVersionedRootSignature(&desc, &p_root_signature_serialized, nullptr)))
        {
            return false;
        }

        if (FAILED(p_device->CreateRootSignature(0, p_root_signature_serialized->GetBufferPointer(), p_root_signature_serialized->GetBufferSize(), IID_PPV_ARGS(&p_root_signature))))
        {
            return false;
        }
    }

    // パイプラインを生成
    Microsoft::WRL::ComPtr<ID3D12PipelineState> p_pipeline_state;
    {
        // ラスタライザー
        D3D12_RASTERIZER_DESC rasterizer_desc = {};
        rasterizer_desc.FillMode              = D3D12_FILL_MODE_SOLID;
        rasterizer_desc.CullMode              = D3D12_CULL_MODE_BACK;
        rasterizer_desc.FrontCounterClockwise = true;
        rasterizer_desc.DepthClipEnable       = true;

        // ブレンダー
        D3D12_BLEND_DESC blend_desc = {};
        blend_desc.RenderTarget[0].BlendEnable           = false;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        // デプス
        D3D12_DEPTH_STENCIL_DESC depth_stencil_desc = {};
        depth_stencil_desc.DepthEnable    = arg.mDepthSettings.mEnableTest;
        depth_stencil_desc.DepthWriteMask = arg.mDepthSettings.mEnableWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
        depth_stencil_desc.DepthFunc      = D3D12_COMPARISON_FUNC_LESS;

        // パイプライン
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
        desc.InputLayout.NumElements        = p_input_layout->getNumInputElementDesc();
        desc.InputLayout.pInputElementDescs = p_input_layout->getInputElementDescs();
        desc.pRootSignature                 = p_root_signature.Get();
        if (arg.mVertexShaderBufferSize > 0)
        {
            desc.VS.pShaderBytecode         = arg.mpVertexShaderBuffer;
            desc.VS.BytecodeLength          = arg.mVertexShaderBufferSize;
        }
        if (arg.mPixelShaderBufferSize > 0)
        {
            desc.PS.pShaderBytecode         = arg.mpPixelShaderBuffer;
            desc.PS.BytecodeLength          = arg.mPixelShaderBufferSize;
        }
        desc.RasterizerState                = rasterizer_desc;
        desc.BlendState                     = blend_desc;
        desc.DepthStencilState              = depth_stencil_desc;
        desc.SampleMask                     = std::numeric_limits<uint32_t>::max();

        switch (arg.mPrimitiveTopology)
        {
            case IMeshBuffer::PrimitiveTopology::Triangles:
                desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                break;
            case IMeshBuffer::PrimitiveTopology::Lines:
                desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
                break;
        }

        // 出力フォーマット
        desc.NumRenderTargets               = arg.mNumOutput;
        for (uint32_t i_output = 0; i_output < arg.mNumOutput; ++i_output)
        {
            desc.RTVFormats[i_output] = Texture::ConvertFormat(arg.mOutputFormats[i_output]);
        }
        desc.DSVFormat                      = Texture::ConvertFormat(arg.mDepthFormat);
        desc.SampleDesc.Count               = 1;

        if (FAILED(p_device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&p_pipeline_state))))
        {
            return false;
        }
    }

    // 保持
    mpRootSignature.reset(p_root_signature.Detach());
    mpPipelineState.reset(p_pipeline_state.Detach());

    return true;
}

// ----------------------------------------------------------------------------

}
