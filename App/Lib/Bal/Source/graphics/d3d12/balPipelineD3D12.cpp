/*!
 * @file   balPipelineD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// windows
#include <wrl.h>
// bal
#include <container/balArray.h>
#include <graphics/d3d12/balGraphicsD3D12.h>
#include <graphics/d3d12/balPipelineD3D12.h>
#include <io/balFile.h>

namespace bal::gfx::d3d12 {

// ----------------------------------------------------------------------------

bool Pipeline::initialize(const InitializeArg& arg)
{
    // デバイス
    ID3D12Device6* p_device = reinterpret_cast<Graphics*>(arg.mpGraphics)->getDevice();

    // @TODO: いろいろ固定してあるので、変更する

    // ルートシグネチャ
    // @TODO: 今はとりあえず定数バッファ1つ（b0）のみ
    // 参考: https://sites.google.com/site/monshonosuana/directxno-hanashi-1/directx-145
    Microsoft::WRL::ComPtr<ID3D12RootSignature> p_root_signature;
    {
        // デスクリプターテーブル
        D3D12_DESCRIPTOR_RANGE1 range = {};
        range.RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        range.NumDescriptors                    = 1;
        range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        // ルートパラメーター
        // @TODO: b0 の定数バッファはすべてのシェーダーで見える
        D3D12_ROOT_PARAMETER1 param = {};
        param.ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param.ShaderVisibility                    = D3D12_SHADER_VISIBILITY_ALL;
        param.DescriptorTable.NumDescriptorRanges = 1;
        param.DescriptorTable.pDescriptorRanges   = &range;

        // ルートシグネチャ
        D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc = {};
        desc.Version                = D3D_ROOT_SIGNATURE_VERSION_1_1;
        desc.Desc_1_1.NumParameters = 1;
        desc.Desc_1_1.pParameters   = &param;
        desc.Desc_1_1.Flags         = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

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
        // レイアウト
        Array element_descs =
        {
            D3D12_INPUT_ELEMENT_DESC{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            D3D12_INPUT_ELEMENT_DESC{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
        };

        // ラスタライザー
        D3D12_RASTERIZER_DESC rasterizer_desc = {};
        rasterizer_desc.FillMode              = D3D12_FILL_MODE_SOLID;
        rasterizer_desc.CullMode              = D3D12_CULL_MODE_BACK;
        rasterizer_desc.FrontCounterClockwise = false;
        rasterizer_desc.DepthClipEnable       = true;

        // ブレンダー
        D3D12_BLEND_DESC blend_desc = {};
        blend_desc.RenderTarget[0].BlendEnable           = false;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        // パイプライン
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
        desc.InputLayout.NumElements        = static_cast<uint32_t>(element_descs.size());
        desc.InputLayout.pInputElementDescs = element_descs.data();
        desc.pRootSignature                 = p_root_signature.Get();
        if (arg.mpVSFile)
        {
            desc.VS.pShaderBytecode         = arg.mpVSFile->getBuffer();
            desc.VS.BytecodeLength          = arg.mpVSFile->getBufferSize();
        }
        if (arg.mpPSFile)
        {
            desc.PS.pShaderBytecode         = arg.mpPSFile->getBuffer();
            desc.PS.BytecodeLength          = arg.mpPSFile->getBufferSize();
        }
        desc.RasterizerState                = rasterizer_desc;
        desc.BlendState                     = blend_desc;
        desc.SampleMask                     = std::numeric_limits<std::uint32_t>::max();
        desc.PrimitiveTopologyType          = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        desc.NumRenderTargets               = 1;
        desc.RTVFormats[0]                  = DXGI_FORMAT_R8G8B8A8_UNORM;
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
