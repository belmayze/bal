/*!
 * @file   balPipelineD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <container/balArray.h>
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
        // ルートシグネチャ
        D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc = {};
        desc.Version        = D3D_ROOT_SIGNATURE_VERSION_1_1;
        desc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        // デスクリプターテーブルのパラメーター
        D3D12_ROOT_PARAMETER1 param = {};
        param.ParameterType    = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

        // デスクリプターレンジの数（とりあえず一つずつ）
        // @TODO: 寿命などでデスクリプターテーブルを分けれるようにしておく
        uint32_t num_descriptor_range = 0;
        if (arg.mNumConstantBuffer != 0) { num_descriptor_range++; }
        if (arg.mNumTexture != 0)        { num_descriptor_range++; }

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

        // デスクリプターテーブル
        std::unique_ptr<D3D12_DESCRIPTOR_RANGE1[]> ranges;
        if (num_descriptor_range != 0)
        {
            ranges = make_unique<D3D12_DESCRIPTOR_RANGE1[]>(nullptr, num_descriptor_range);
            D3D12_DESCRIPTOR_RANGE1* p_target = ranges.get();

            // @TODO: レジスターを0以外から使えるようにしておく
            // パイプラインでレジスターを使う場合は設定しておく
            if (arg.mNumConstantBuffer != 0)
            {
                p_target->RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                p_target->NumDescriptors                    = arg.mNumConstantBuffer;
                p_target->BaseShaderRegister                = 0;
                p_target->RegisterSpace                     = 0;
                p_target->Flags                             = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
                p_target->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                p_target++;
            }
            if (arg.mNumTexture != 0)
            {
                p_target->RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                p_target->NumDescriptors                    = arg.mNumTexture;
                p_target->BaseShaderRegister                = 0;
                p_target->RegisterSpace                     = 0;
                p_target->Flags                             = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
                p_target->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                p_target++;
            }

            // パラメーター設定
            param.DescriptorTable.NumDescriptorRanges = num_descriptor_range;
            param.DescriptorTable.pDescriptorRanges   = ranges.get();

            desc.Desc_1_1.NumParameters = 1;
            desc.Desc_1_1.pParameters   = &param;
        }

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
        depth_stencil_desc.DepthEnable = false;

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
        desc.PrimitiveTopologyType          = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        // 出力フォーマット
        desc.NumRenderTargets               = arg.mNumOutput;
        for (uint32_t i_output = 0; i_output < arg.mNumOutput; ++i_output)
        {
            desc.RTVFormats[i_output] = Texture::ConvertFormat(arg.mOutputFormats[i_output]);
        }
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
