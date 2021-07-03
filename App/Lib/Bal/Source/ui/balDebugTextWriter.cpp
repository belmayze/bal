/*!
 * @file   balDebugTextWriter.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
// bal
#include <engine/module/gfx/balGfxMeshContainer.h>
#include <engine/balEngine.h>
#include <ui/balDebugTextWriter.h>

namespace bal::ui
{
// ----------------------------------------------------------------------------
void TextWriter::initialize(const InitializeArg& arg)
{
    // グラフィックス初期化
    {
        // 頂点レイアウト
        std::unique_ptr<InputLayout> p_input_layout = make_unique<InputLayout>(nullptr);
        {
            std::unique_ptr<IInputLayout::InputLayoutDesc[]> descs = make_unique<IInputLayout::InputLayoutDesc[]>(nullptr, 2);
            descs[0] = { .mName = "POSITION", .mType = IInputLayout::Type::Vec3, .mOffset = mod::gfx::MeshContainer::cOffsetPosition };
            descs[1] = { .mName = "TEXCOORD", .mType = IInputLayout::Type::Vec2, .mOffset = mod::gfx::MeshContainer::cOffsetTexcoord };
            IInputLayout::InitializeArg init_arg;
            init_arg.mpGraphics      = arg.mpGraphics;
            init_arg.mNumInputLayout = 2;
            init_arg.mpInputLayouts  = descs.get();
            if (!p_input_layout->initialize(init_arg)) { return; }
        }

        // 定数バッファ
        mpConstantBuffer = make_unique<ConstantBuffer>(nullptr);
        {
            IConstantBuffer::InitializeArg init_arg;
            init_arg.mpGraphics   = arg.mpGraphics;
            init_arg.mBufferCount = 2;
            init_arg.mBufferSize  = sizeof(CBData) * arg.mMaxText;
        }

        // デスクリプターヒープ
        mpDescriptorHeap = make_unique<DescriptorHeap>(nullptr);
        {
            const IConstantBuffer* p_content_buffers[] = { mpConstantBuffer.get() };
            IDescriptorHeap::InitializeArg init_arg;
            init_arg.mpGraphics         = arg.mpGraphics;
            init_arg.mConstantRangeBase = 0;
            init_arg.mNumConstantBuffer = 1;
            init_arg.mpConstantBuffers  = p_content_buffers;
            if (!mpDescriptorHeap->initialize(init_arg)) { return; }
        }

        // パイプライン
        mpPipeline = make_unique<Pipeline>(nullptr);
        {
            const IDescriptorHeap* p_heaps[] = { mpDescriptorHeap.get() };
            IPipeline::InitializeArg init_arg;
            init_arg.mpGraphics         = arg.mpGraphics;
            init_arg.mNumOutput         = 1;
            init_arg.mOutputFormats[0]  = arg.mOutputFormat;
            init_arg.mNumDescriptorHeap = 1;
            init_arg.mpDescriptorHeaps  = p_heaps;
            init_arg.mpInputLayout      = p_input_layout.get();

            // @TODO: シェーダー
        }
    }
}
// ----------------------------------------------------------------------------
void TextWriter::setupText(const StringBase& str)
{
    // @TODO: UBO計算
}
// ----------------------------------------------------------------------------
void TextWriter::draw(ICommandListDirect& cmd_list) const
{

}
// ----------------------------------------------------------------------------
}
