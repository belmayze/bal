/*!
 * @file   balGfxModule.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <engine/balEngine.h>
#include <engine/module/gfx/balGfxModule.h>
#include <engine/module/gfx/balIGfxCustomModule.h>
#include <engine/module/gfx/balGfxShapeContainer.h>
// gfx
#include <graphics/balFrameBuffer.h>
#include <graphics/balICommandListDirect.h>
#include <graphics/balIGraphics.h>
#include <graphics/balIRenderTarget.h>
#include <graphics/balViewport.h>
#include <graphics/archiver/balShaderArchive.h>
// gfx::d3d12
#include <graphics/d3d12/balDescriptorTableD3D12.h>
#include <graphics/d3d12/balInputLayoutD3D12.h>
#include <graphics/d3d12/balPipelineD3D12.h>
#include <graphics/d3d12/balRenderTargetD3D12.h>
#include <graphics/d3d12/balTextureD3D12.h>

namespace bal::mod::gfx {

// ----------------------------------------------------------------------------

Module::Module() {}

// ----------------------------------------------------------------------------

Module::~Module() {}

// ----------------------------------------------------------------------------

void Module::setCustomModule(std::unique_ptr<mod::ICustomModule>&& p_custom_module)
{
    mpCustomModule.reset(static_cast<ICustomModule*>(p_custom_module.release()));
}

// ----------------------------------------------------------------------------

void Module::initialize(const InitializeArg& arg)
{
    // グラフィックスシステム
    IGraphics*     p_graphics         = Engine::GetInstance().getGraphicsSystem();
    const MathSize render_buffer_size = p_graphics->getDefaultFrameBuffer()->getResolution();

    // HDR 用レンダーバッファ
    {

        // テクスチャーを確保
        std::unique_ptr<ITexture> p_color_buffer = make_unique<d3d12::Texture>(nullptr);
        {
            ITexture::InitializeArg init_arg;
            init_arg.mpGraphics = p_graphics;
            init_arg.mDimension = ITexture::Dimension::Texture2D;
            init_arg.mFormat    = ITexture::Format::R16_G16_B16_A16_FLOAT;
            init_arg.mSize      = render_buffer_size;
            if (!p_color_buffer->initialize(init_arg)) { return; }
        }
        std::unique_ptr<ITexture> p_depth_buffer = make_unique<d3d12::Texture>(nullptr);
        {
            ITexture::InitializeArg init_arg;
            init_arg.mpGraphics = p_graphics;
            init_arg.mDimension = ITexture::Dimension::Texture2D;
            init_arg.mFormat    = ITexture::Format::D32_FLOAT;
            init_arg.mSize      = render_buffer_size;
            if (!p_depth_buffer->initialize(init_arg)) { return; }
        }

        // レンダーターゲット
        mpRenderTargetColor = make_unique<d3d12::RenderTargetColor>(nullptr);
        {
            IRenderTargetColor::InitializeArg init_arg;
            init_arg.mpGraphics = p_graphics;
            if (!mpRenderTargetColor->initialize(init_arg, std::move(p_color_buffer))) { return; }
        }
        mpRenderTargetDepth = make_unique<d3d12::RenderTargetDepth>(nullptr);
        {
            IRenderTargetDepth::InitializeArg init_arg;
            init_arg.mpGraphics = p_graphics;
            if (!mpRenderTargetDepth->initialize(init_arg, std::move(p_depth_buffer))) { return; }
        }

        // フレームバッファ
        mpFrameBuffer = make_unique<FrameBuffer>(nullptr);
        {
            mpFrameBuffer->setRenderTargetColor(0, mpRenderTargetColor.get());
            mpFrameBuffer->setRenderTargetDepth(mpRenderTargetDepth.get());
            mpFrameBuffer->setResolution(render_buffer_size);
        }
    }

    // シェーダーアーカイブ読み込み
    mpShaderArchive = make_unique<ShaderArchive>(nullptr);
    {
        File file;
        if (!file.loadFromFile("Shader\\bal_shader.bsa")) { return; }
        if (!mpShaderArchive->loadArchiver(std::move(file))) { return; }
    }

    // 各種パイプラインの作成
    {
        // 最終画面反映
        {
            // 頂点レイアウト
            std::unique_ptr<d3d12::InputLayout> p_input_layout = make_unique<d3d12::InputLayout>(nullptr);
            {
                std::unique_ptr<IInputLayout::InputLayoutDesc[]> descs = make_unique<IInputLayout::InputLayoutDesc[]>(nullptr, 2);
                descs[0] = { .mName = "POSITION", .mType = IInputLayout::Type::Vec3, .mOffset =  0 };
                descs[1] = { .mName = "TEXCOORD", .mType = IInputLayout::Type::Vec2, .mOffset = 12 };

                IInputLayout::InitializeArg init_arg;
                init_arg.mpGraphics      = p_graphics;
                init_arg.mNumInputLayout = 2;
                init_arg.mpInputLayouts = descs.get();
                if (!p_input_layout->initialize(init_arg)) { return; }
            }

            // パイプライン初期化
            mpPresentPipeline = make_unique<d3d12::Pipeline>(nullptr);
            {
                IPipeline::InitializeArg init_arg;
                init_arg.mpGraphics        = p_graphics;
                init_arg.mNumOutput        = 1;
                init_arg.mOutputFormats[0] = p_graphics->getSwapChainFrameBuffer()->getRenderTargetColors()[0]->getTexture()->getFormat();
                init_arg.mNumTexture       = 1;
                init_arg.mpInputLayout     = p_input_layout.get();

                const ShaderArchive::ShaderContainer& shader_container = mpShaderArchive->getShaderContainer(mpShaderArchive->findProgram("Present"));
                init_arg.mpVertexShaderBuffer    = shader_container.mVertexShader.mBuffer;
                init_arg.mVertexShaderBufferSize = shader_container.mVertexShader.mBufferSize;
                init_arg.mpPixelShaderBuffer     = shader_container.mPixelShader.mBuffer;
                init_arg.mPixelShaderBufferSize  = shader_container.mPixelShader.mBufferSize;
                if (!mpPresentPipeline->initialize(init_arg)) { return; }
            }

            // デスクリプターテーブル
            mpPresentDescriptorTable = make_unique<d3d12::DescriptorTable>(nullptr);
            {
                const ITexture* p_textures[] = { mpRenderTargetColor->getTexture() };
                IDescriptorTable::InitializeArg init_arg;
                init_arg.mpGraphics  = p_graphics;
                init_arg.mNumTexture = 1;
                init_arg.mpTextures  = p_textures;
                if (!mpPresentDescriptorTable->initialize(init_arg)) { return; }
            }
        }
    }

    // よく使用するシェイプ形状を初期化する
    ShapeContainer::GetInstance().initialize();
    ShapeContainer::AddGfxFinalizer();

    // カスタムモジュールを保持し初期化
    if (mpCustomModule)
    {
        mpCustomModule->initialize(arg);
    }
}

// ----------------------------------------------------------------------------

void Module::onUpdate(const FrameworkCallback::UpdateArg& arg)
{
}

// ----------------------------------------------------------------------------

void Module::onDraw(const FrameworkCallback::DrawArg& arg)
{
    // レンダリング用バッファに切り替え
    {
        // バリア
        arg.mpCommandList->resourceBarrier(
            *mpRenderTargetColor->getTexture(),
            ICommandListDirect::ResourceBarrierType::GenericRead,
            ICommandListDirect::ResourceBarrierType::RenderTargetColor
        );
        arg.mpCommandList->resourceBarrier(
            *mpRenderTargetDepth->getTexture(),
            ICommandListDirect::ResourceBarrierType::GenericRead,
            ICommandListDirect::ResourceBarrierType::RenderTargetDepth
        );

        // バッファ切り替え
        arg.mpCommandList->setViewport(Viewport(*mpFrameBuffer));
        arg.mpCommandList->bindFrameBuffer(*mpFrameBuffer);
        arg.mpCommandList->clear(*mpFrameBuffer, ICommandListDirect::ClearFlag::Color | ICommandListDirect::ClearFlag::Depth, MathColor(0.f, 0.f, 0.f, 1.f), 1.f, 0);

        // @TODO: レンダリング

        // バリア
        arg.mpCommandList->resourceBarrier(
            *mpRenderTargetColor->getTexture(),
            ICommandListDirect::ResourceBarrierType::RenderTargetColor,
            ICommandListDirect::ResourceBarrierType::GenericRead
        );
        arg.mpCommandList->resourceBarrier(
            *mpRenderTargetDepth->getTexture(),
            ICommandListDirect::ResourceBarrierType::RenderTargetDepth,
            ICommandListDirect::ResourceBarrierType::GenericRead
        );
    }

    // レンダリング用を書き出し
    {
        // バリア
        arg.mpCommandList->resourceBarrier(
            *arg.mpSwapChainFrameBuffer->getRenderTargetColors()[0]->getTexture(),
            ICommandListDirect::ResourceBarrierType::Present,
            ICommandListDirect::ResourceBarrierType::RenderTargetColor
        );

        // フレームバッファ切り替え
        arg.mpCommandList->setViewport(Viewport(*arg.mpSwapChainFrameBuffer));
        arg.mpCommandList->bindFrameBuffer(*arg.mpSwapChainFrameBuffer);

        // 画面反映
        arg.mpCommandList->bindPipeline(*mpPresentPipeline);
        arg.mpCommandList->setDescriptorTable(0, *mpPresentDescriptorTable);
        arg.mpCommandList->drawShape(*ShapeContainer::GetInstance().getQuadBuffer());

        // バリア
        arg.mpCommandList->resourceBarrier(
            *arg.mpSwapChainFrameBuffer->getRenderTargetColors()[0]->getTexture(),
            ICommandListDirect::ResourceBarrierType::RenderTargetColor,
            ICommandListDirect::ResourceBarrierType::Present
        );
    }
}

// ----------------------------------------------------------------------------

}
