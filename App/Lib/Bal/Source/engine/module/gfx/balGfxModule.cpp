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
#include <engine/module/gfx/balGfxMeshContainer.h>
// gfx
#include <graphics/balFrameBuffer.h>
#include <graphics/balICommandListDirect.h>
#include <graphics/balIGraphics.h>
#include <graphics/balIRenderTarget.h>
#include <graphics/balViewport.h>
#include <graphics/archiver/balShaderArchive.h>
// gfx::d3d12
#include <graphics/d3d12/balDescriptorHeapD3D12.h>
#include <graphics/d3d12/balConstantBufferD3D12.h>
#include <graphics/d3d12/balInputLayoutD3D12.h>
#include <graphics/d3d12/balMeshBufferD3D12.h>
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
    const MathSize render_buffer_size = p_graphics->getSwapChainFrameBuffer()->getResolution();

    // HDR 用レンダーバッファ
    {
        // テクスチャーを確保
        std::unique_ptr<ITexture> p_color_buffer = make_unique<Texture>(nullptr);
        {
            ITexture::InitializeArg init_arg;
            init_arg.mpGraphics = p_graphics;
            init_arg.mDimension = ITexture::Dimension::Texture2D;
            init_arg.mFormat    = ITexture::Format::R16_G16_B16_A16_FLOAT;
            init_arg.mSize      = render_buffer_size;
            if (!p_color_buffer->initialize(init_arg)) { return; }
        }
        std::unique_ptr<ITexture> p_depth_buffer = make_unique<Texture>(nullptr);
        {
            ITexture::InitializeArg init_arg;
            init_arg.mpGraphics = p_graphics;
            init_arg.mDimension = ITexture::Dimension::Texture2D;
            init_arg.mFormat    = ITexture::Format::D32_FLOAT;
            init_arg.mSize      = render_buffer_size;
            if (!p_depth_buffer->initialize(init_arg)) { return; }
        }

        // レンダーターゲット
        mpRenderTargetColor = make_unique<RenderTargetColor>(nullptr);
        {
            IRenderTargetColor::InitializeArg init_arg;
            init_arg.mpGraphics = p_graphics;
            if (!mpRenderTargetColor->initialize(init_arg, std::move(p_color_buffer))) { return; }
        }
        mpRenderTargetDepth = make_unique<RenderTargetDepth>(nullptr);
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

    // シーン用定数バッファを生成
    {
        mpSceneConstantBuffer = make_unique<ConstantBuffer>(nullptr);
        {
            ConstantBuffer::InitializeArg init_arg;
            init_arg.mpGraphics   = p_graphics;
            init_arg.mBufferSize  = mpCustomModule ? mpCustomModule->getSceneConstantBufferSize() : sizeof(SceneConstantBufferBase);
            init_arg.mBufferCount = 1;
            mpSceneConstantBuffer->initialize(init_arg);
        }
    }

    // グリッドとマニピュレーターのメッシュ情報
    {
        mpGridMeshBuffer = make_unique<MeshBuffer>(nullptr);
        {
            // 100m x 100m (1m 単位)
            std::unique_ptr<DebugMeshVertex[]> vertices = make_unique<DebugMeshVertex[]>(nullptr, (101 + 101) * 2);
            for (int i_x = 0; i_x <= 100; ++i_x)
            {
                vertices[i_x * 2 + 0].mPosition = MathVector3(static_cast<float>(i_x - 50), 0.f,  50.f);
                vertices[i_x * 2 + 1].mPosition = MathVector3(static_cast<float>(i_x - 50), 0.f, -50.f);
                vertices[i_x * 2 + 0].mColor    = MathColor(1.f, 1.f, 1.f);
                vertices[i_x * 2 + 1].mColor    = MathColor(1.f, 1.f, 1.f);
            }
            for (int i_z = 0; i_z <= 100; ++i_z)
            {
                vertices[(101 + i_z) * 2 + 0].mPosition = MathVector3( 50, 0.f, static_cast<float>(i_z - 50));
                vertices[(101 + i_z) * 2 + 1].mPosition = MathVector3(-50, 0.f, static_cast<float>(i_z - 50));
                vertices[(101 + i_z) * 2 + 0].mColor    = MathColor(1.f, 1.f, 1.f);
                vertices[(101 + i_z) * 2 + 1].mColor    = MathColor(1.f, 1.f, 1.f);
            }
            std::unique_ptr<uint16_t[]> indices = make_unique<uint16_t[]>(nullptr, (101 + 101) * 2);
            for (int i = 0; i < (101 + 101); ++i)
            {
                indices[i * 2 + 0] = i * 2 + 0;
                indices[i * 2 + 1] = i * 2 + 1;
            }

            IMeshBuffer::InitializeArg init_arg;
            init_arg.mpGraphics         = p_graphics;
            init_arg.mPrimitiveTopology = IMeshBuffer::PrimitiveTopology::Lines;
            init_arg.mpVertexBuffer     = vertices.get();
            init_arg.mVertexBufferSize  = sizeof(DebugMeshVertex) * (101 + 101) * 2;
            init_arg.mVertexStride      = sizeof(DebugMeshVertex);
            init_arg.mpIndexBuffer      = indices.get();
            init_arg.mIndexBufferSize   = sizeof(uint16_t) * (101 + 101) * 2;
            init_arg.mIndexCount        = (101 + 101) * 2;
            init_arg.mIndexBufferFormat = IMeshBuffer::IndexBufferFormat::Uint16;
            if (!mpGridMeshBuffer->initialize(init_arg)) { return; }
        }
    }

    // シェーダーアーカイブ読み込み
    mpShaderArchive = make_unique<ShaderArchive>(nullptr);
    {
        File file;
        if (!file.loadFromFile("Shader\\balShader.bsa")) { return; }
        if (!mpShaderArchive->loadArchiver(std::move(file))) { return; }
    }

    // デスクリプターヒープ
    mpSceneDescriptorHeap = bal::make_unique<bal::DescriptorHeap>(nullptr);
    {
        const bal::IConstantBuffer* p_content_buffers[] = {mpSceneConstantBuffer.get()};
        bal::IDescriptorHeap::InitializeArg init_arg;
        init_arg.mpGraphics         = p_graphics;
        init_arg.mConstantRangeBase = 0;
        init_arg.mNumConstantBuffer = 1;
        init_arg.mpConstantBuffers  = p_content_buffers;
        if (!mpSceneDescriptorHeap->initialize(init_arg)) { return; }
    }

    // 各種パイプラインの作成
    {
        // 最終画面反映
        {
            // 頂点レイアウト
            std::unique_ptr<InputLayout> p_input_layout = make_unique<InputLayout>(nullptr);
            {
                std::unique_ptr<IInputLayout::InputLayoutDesc[]> descs = make_unique<IInputLayout::InputLayoutDesc[]>(nullptr, 2);
                descs[0] = {.mName = "POSITION", .mType = IInputLayout::Type::Vec3, .mOffset = MeshContainer::cOffsetPosition};
                descs[1] = {.mName = "TEXCOORD", .mType = IInputLayout::Type::Vec2, .mOffset = MeshContainer::cOffsetTexcoord};

                IInputLayout::InitializeArg init_arg;
                init_arg.mpGraphics      = p_graphics;
                init_arg.mNumInputLayout = 2;
                init_arg.mpInputLayouts  = descs.get();
                if (!p_input_layout->initialize(init_arg)) { return; }
            }

            // デスクリプターヒープ
            mpPresentDescriptorHeap = make_unique<DescriptorHeap>(nullptr);
            {
                const ITexture* p_textures[] = { mpRenderTargetColor->getTexture() };
                IDescriptorHeap::InitializeArg init_arg;
                init_arg.mpGraphics  = p_graphics;
                init_arg.mNumTexture = 1;
                init_arg.mpTextures  = p_textures;
                if (!mpPresentDescriptorHeap->initialize(init_arg)) { return; }
            }

            // パイプライン
            mpPresentPipeline = make_unique<Pipeline>(nullptr);
            {
                const IDescriptorHeap* p_heaps[] = { mpPresentDescriptorHeap.get() };
                IPipeline::InitializeArg init_arg;
                init_arg.mpGraphics         = p_graphics;
                init_arg.mNumOutput         = 1;
                init_arg.mOutputFormats[0]  = p_graphics->getSwapChainFrameBuffer()->getRenderTargetColors()[0]->getTexture()->getFormat();
                init_arg.mNumDescriptorHeap = 1;
                init_arg.mpDescriptorHeaps  = p_heaps;
                init_arg.mpInputLayout      = p_input_layout.get();

                const ShaderArchive::ShaderContainer& shader_container = mpShaderArchive->getShaderContainer(mpShaderArchive->findProgram("Present"));
                init_arg.mpVertexShaderBuffer    = shader_container.mVertexShader.mBuffer;
                init_arg.mVertexShaderBufferSize = shader_container.mVertexShader.mBufferSize;
                init_arg.mpPixelShaderBuffer     = shader_container.mPixelShader.mBuffer;
                init_arg.mPixelShaderBufferSize  = shader_container.mPixelShader.mBufferSize;
                if (!mpPresentPipeline->initialize(init_arg)) { return; }
            }
        }

        // デバッグライン描画
        {
            // 頂点レイアウト
            std::unique_ptr<InputLayout> p_input_layout = make_unique<InputLayout>(nullptr);
            {
                std::unique_ptr<IInputLayout::InputLayoutDesc[]> descs = make_unique<IInputLayout::InputLayoutDesc[]>(nullptr, 2);
                descs[0] = {.mName = "POSITION", .mType = IInputLayout::Type::Vec3, .mOffset = cDebugMeshVertexPositionOffset};
                descs[1] = {.mName = "COLOR",    .mType = IInputLayout::Type::Vec4, .mOffset = cDebugMeshVertexColorOffset};

                IInputLayout::InitializeArg init_arg;
                init_arg.mpGraphics      = p_graphics;
                init_arg.mNumInputLayout = 2;
                init_arg.mpInputLayouts  = descs.get();
                if (!p_input_layout->initialize(init_arg)) { return; }
            }

            mpDebugMeshPipeline = make_unique<Pipeline>(nullptr);
            {
                const IDescriptorHeap* p_heaps[] = {mpSceneDescriptorHeap.get()};
                IPipeline::InitializeArg init_arg;
                init_arg.mpGraphics         = p_graphics;
                init_arg.mNumOutput         = 1;
                init_arg.mOutputFormats[0]  = mpRenderTargetColor->getTexture()->getFormat();
                init_arg.mNumDescriptorHeap = 1;
                init_arg.mpDescriptorHeaps  = p_heaps;
                init_arg.mpInputLayout      = p_input_layout.get();
                init_arg.mPrimitiveTopology = IMeshBuffer::PrimitiveTopology::Lines;

                init_arg.mDepthSettings.mEnableTest  = true;
                init_arg.mDepthSettings.mEnableWrite = true;

                const ShaderArchive::ShaderContainer& shader_container = mpShaderArchive->getShaderContainer(mpShaderArchive->findProgram("DebugLine"));
                init_arg.mpVertexShaderBuffer    = shader_container.mVertexShader.mBuffer;
                init_arg.mVertexShaderBufferSize = shader_container.mVertexShader.mBufferSize;
                init_arg.mpPixelShaderBuffer     = shader_container.mPixelShader.mBuffer;
                init_arg.mPixelShaderBufferSize  = shader_container.mPixelShader.mBufferSize;
                if (!mpDebugMeshPipeline->initialize(init_arg)) { return; }
            }
        }
    }

    // カメラにはデフォルトの設定を入れておく
    {
        mCamera.setAspect(static_cast<float>(render_buffer_size.getWidth()) / static_cast<float>(render_buffer_size.getHeight()));
        mCamera.setFovy(Degree(50.f));
        mCamera.setNearFar(0.01f, 1000.f);

        mCamera.setLookAt(MathVector3(0.f, 5.f, 10.f), MathVector3(0.f, 0.f, 0.f));

        mCamera.updateMatrix();
    }

    // よく使用するシェイプ形状を初期化する
    MeshContainer::GetInstance().initialize();
    MeshContainer::AddGfxFinalizer();

    // カスタムモジュールを初期化
    if (mpCustomModule)
    {
        mpCustomModule->initialize(arg, *this);
    }
}

// ----------------------------------------------------------------------------

void Module::finalize()
{

}

// ----------------------------------------------------------------------------

void Module::onUpdate(const FrameworkCallback::UpdateArg& arg)
{
    // カスタムモジュール
    if (mpCustomModule)
    {
        mpCustomModule->onUpdate(arg, *this);
    }

    // カメラの決定
    {
        SceneConstantBufferBase* p_cb = mpSceneConstantBuffer->getBufferPtr<SceneConstantBufferBase>();
        p_cb->mProjectionMatrix = mCamera.getProjectionMatrix();
        p_cb->mViewMatrix       = mCamera.getViewMatrix();

        // ライトの仮
        p_cb->mDirectionalLightDir   = MathVector3(-1.f, -1.f, -1.f).calcNormalize();
        p_cb->mDirectionalLightColor = MathVector3(1.f, 1.f, 1.f);
    }
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

        // グリッド描画
        arg.mpCommandList->bindPipeline(*mpDebugMeshPipeline);
        arg.mpCommandList->setDescriptorHeap(*mpSceneDescriptorHeap);
        arg.mpCommandList->setDescriptorTable(0, *mpSceneDescriptorHeap);
        arg.mpCommandList->drawMesh(*mpGridMeshBuffer);

        // カスタムモジュール
        if (mpCustomModule)
        {
            mpCustomModule->onDraw(arg, *this);
        }

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
        arg.mpCommandList->setDescriptorHeap(*mpPresentDescriptorHeap);
        arg.mpCommandList->setDescriptorTable(0, *mpPresentDescriptorHeap);
        arg.mpCommandList->drawMesh(*MeshContainer::GetInstance().getBuffer(MeshContainer::Type::Quad));

        // バリア
        arg.mpCommandList->resourceBarrier(
            *arg.mpSwapChainFrameBuffer->getRenderTargetColors()[0]->getTexture(),
            ICommandListDirect::ResourceBarrierType::RenderTargetColor,
            ICommandListDirect::ResourceBarrierType::Present
        );
    }
}

// ----------------------------------------------------------------------------

size_t Module::getMeshConstantBufferSize() const
{
    return mpCustomModule ? mpCustomModule->getMeshConstantBufferSize() : sizeof(MeshConstantBufferBase);
}

// ----------------------------------------------------------------------------

}
