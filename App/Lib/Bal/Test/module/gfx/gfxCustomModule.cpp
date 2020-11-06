/*!
 * @file   gfxCustomModule.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/archiver/balShaderArchive.h>
#include <io/balFile.h>
#include <engine/module/cntl/balCntlModule.h>
#include <engine/module/cntl/balController.h>
#include <engine/module/gfx/balGfxMeshContainer.h>
#include <engine/module/gfx/balGfxModule.h>
#include <engine/balEngine.h>
// app
#include <module/gfx/gfxCustomModule.h>

namespace app::mod::gfx {

// ----------------------------------------------------------------------------

CustomModule::CustomModule() {}

// ----------------------------------------------------------------------------

CustomModule::~CustomModule() {}

// ----------------------------------------------------------------------------

void CustomModule::initialize(const bal::mod::IModule::InitializeArg& arg, const bal::mod::IModule& module)
{
    // グラフィックスシステム
    bal::IGraphics* p_graphics = bal::Engine::GetInstance().getGraphicsSystem();
    // グラフィックスモジュール
    const bal::mod::gfx::Module& gfx_module = *static_cast<const bal::mod::gfx::Module*>(&module);

    // シェーダー読み込み
    mpShaderArchive = bal::make_unique<bal::ShaderArchive>(nullptr);
    {
        bal::File file;
        if (!file.loadFromFile("Shader\\testShader.bsa")) { return; }
        if (!mpShaderArchive->loadArchiver(std::move(file))) { return; }
    }

    // 環境定数バッファ
    {
        // 定数バッファ
        mpEnvConstantBuffer = bal::make_unique<bal::ConstantBuffer>(nullptr);
        {
            bal::IConstantBuffer::InitializeArg init_arg;
            init_arg.mpGraphics = p_graphics;
            init_arg.mBufferCount = 1;
            init_arg.mBufferSize = sizeof(SampleEnvCB);
            if (!mpEnvConstantBuffer->initialize(init_arg)) { return; }
        }

        // デスクリプターテーブル
        mpEnvDescriptorHeap = bal::make_unique<bal::DescriptorHeap>(nullptr);
        {
            const bal::IConstantBuffer* p_content_buffers[] = { mpEnvConstantBuffer.get() };
            bal::IDescriptorHeap::InitializeArg init_arg;
            init_arg.mpGraphics = p_graphics;
            init_arg.mNumConstantBuffer = 1;
            init_arg.mpConstantBuffers = p_content_buffers;
            if (!mpEnvDescriptorHeap->initialize(init_arg)) { return; }
        }
    }

    // パイプライン構築
    {
        // 球体描画
        {
            // 頂点レイアウト
            std::unique_ptr<bal::InputLayout> p_input_layout = bal::make_unique<bal::InputLayout>(nullptr);
            {
                std::unique_ptr<bal::IInputLayout::InputLayoutDesc[]> descs = bal::make_unique<bal::IInputLayout::InputLayoutDesc[]>(nullptr, 3);
                descs[0] = { .mName = "POSITION", .mType = bal::IInputLayout::Type::Vec3, .mOffset = bal::mod::gfx::MeshContainer::cOffsetPosition };
                descs[1] = { .mName = "NORMAL",   .mType = bal::IInputLayout::Type::Vec3, .mOffset = bal::mod::gfx::MeshContainer::cOffsetNormal };
                descs[2] = { .mName = "TEXCOORD", .mType = bal::IInputLayout::Type::Vec2, .mOffset = bal::mod::gfx::MeshContainer::cOffsetTexcoord };

                bal::IInputLayout::InitializeArg init_arg;
                init_arg.mpGraphics = p_graphics;
                init_arg.mNumInputLayout = 3;
                init_arg.mpInputLayouts = descs.get();
                if (!p_input_layout->initialize(init_arg)) { return; }
            }

            // 定数バッファ
            mpSampleConstantBuffer = bal::make_unique<bal::ConstantBuffer>(nullptr);
            {
                bal::IConstantBuffer::InitializeArg init_arg;
                init_arg.mpGraphics = p_graphics;
                init_arg.mBufferCount = 1;
                init_arg.mBufferSize = sizeof(SampleMeshCB);
                if (!mpSampleConstantBuffer->initialize(init_arg)) { return; }
            }

            // デスクリプターテーブル
            mpSampleDescriptorHeap = bal::make_unique<bal::DescriptorHeap>(nullptr);
            {
                const bal::IConstantBuffer* p_content_buffers[] = { mpSampleConstantBuffer.get() };
                bal::IDescriptorHeap::InitializeArg init_arg;
                init_arg.mpGraphics = p_graphics;
                init_arg.mConstantRangeBase = 1;
                init_arg.mNumConstantBuffer = 1;
                init_arg.mpConstantBuffers = p_content_buffers;
                if (!mpSampleDescriptorHeap->initialize(init_arg)) { return; }
            }

            // パイプライン
            mpSamplePipeline = bal::make_unique<bal::Pipeline>(nullptr);
            {
                const bal::IDescriptorHeap* p_heaps[] = { mpEnvDescriptorHeap.get(), mpSampleDescriptorHeap.get() };
                bal::IPipeline::InitializeArg init_arg;
                init_arg.mpGraphics         = p_graphics;
                init_arg.mNumOutput         = 1;
                init_arg.mOutputFormats[0]  = gfx_module.getDefaultRenderTarget().getTexture()->getFormat();
                init_arg.mNumDescriptorHeap = 2;
                init_arg.mpDescriptorHeaps  = p_heaps;
                init_arg.mpInputLayout      = p_input_layout.get();

                init_arg.mDepthSettings.mEnableTest = true;
                init_arg.mDepthSettings.mEnableWrite = true;

                const bal::ShaderArchive::ShaderContainer& shader_container = mpShaderArchive->getShaderContainer(mpShaderArchive->findProgram("Sample"));
                init_arg.mpVertexShaderBuffer    = shader_container.mVertexShader.mBuffer;
                init_arg.mVertexShaderBufferSize = shader_container.mVertexShader.mBufferSize;
                init_arg.mpPixelShaderBuffer     = shader_container.mPixelShader.mBuffer;
                init_arg.mPixelShaderBufferSize  = shader_container.mPixelShader.mBufferSize;
                if (!mpSamplePipeline->initialize(init_arg)) { return; }
            }
        }
    }
}

// ----------------------------------------------------------------------------

void CustomModule::onUpdate(const bal::FrameworkCallback::UpdateArg& arg, const bal::mod::IModule& module)
{
    // カメラの仮制御
    {
        bal::mod::gfx::PerspectiveCamera& camera = bal::mod::gfx::Module::getModule()->getCamera();

        // 移動軸（カメラ行列は倒置）
        bal::MathVector3 camera_x = camera.getViewMatrix().getCol(0).getXYZ();
        bal::MathVector3 camera_y = camera.getViewMatrix().getCol(1).getXYZ();
        bal::MathVector3 camera_z = camera.getViewMatrix().getCol(2).getXYZ();

        // 入力によって移動させる
        const bal::mod::cntl::Controller& controller = bal::mod::cntl::Module::getModule()->getController(0);
        if (controller.isConnected())
        {
            // Z軸ベクトルを入力によって移動させる
            camera_z -= camera_x * controller.getLeftStick().getX() * 0.05f;
            camera_z -= camera_y * controller.getLeftStick().getY() * 0.05f;
            camera_z.setNormalize();

            // XYベクトルを直行させる
            // Yを固定して捻じれ禁止
            if (camera_y.getY() + camera_z.getY() * controller.getLeftStick().getY() * 0.05f >= 0.f)
            {
                camera_y = bal::MathVector3(0.f, 1.f, 0.f);
            }
            else
            {
                camera_y = bal::MathVector3(0.f, -1.f, 0.f);
            }
            camera_x = camera_y.calcCross(camera_z);
            camera_x.setNormalize();
            camera_y = camera_z.calcCross(camera_x);
            camera_y.setNormalize();

            // カメラの位置
            bal::MathVector3 position = camera_z * 10.f;
            bal::MathVector3 camera_t(-camera_x.calcDot(position), -camera_y.calcDot(position), -camera_z.calcDot(position));

            // 行列を求めてセット
            bal::MathMatrix44 view_matrix;
            view_matrix.setCol(0, bal::MathVector4(camera_x, camera_t.getX()));
            view_matrix.setCol(1, bal::MathVector4(camera_y, camera_t.getY()));
            view_matrix.setCol(2, bal::MathVector4(camera_z, camera_t.getZ()));
            camera.setViewMatrix(view_matrix);
            camera.updateMatrix();
        }
    }

    // 定数バッファ更新
    {
        // モデルのワールド位置は固定
        SampleMeshCB* p_cb = mpSampleConstantBuffer->getBufferPtr<SampleMeshCB>();
        p_cb->mWorldMatrix         .setIdentity();
        p_cb->mWorldMatrixForNormal.setIdentity();
    }
    {
        const bal::mod::gfx::PerspectiveCamera& camera = static_cast<const bal::mod::gfx::Module&>(module).getCamera();

        SampleEnvCB* p_cb = mpEnvConstantBuffer->getBufferPtr<SampleEnvCB>();

        // 行列はカメラから取得
        p_cb->mProjMatrix           = camera.getProjectionMatrix();
        p_cb->mViewMatrix           = camera.getViewMatrix();
        p_cb->mProjectionViewMatrix = camera.getViewProjectionMatrix();

        // ライトは固定
        p_cb->mDirectionalLightDir   = bal::MathVector3(-1.f, -1.f, -1.f).calcNormalize();
        p_cb->mDirectionalLightColor = bal::MathColor(1.f, 1.f, 1.f);
    }
}

// ----------------------------------------------------------------------------

void CustomModule::onDraw(const bal::FrameworkCallback::DrawArg& arg, const bal::mod::IModule& module)
{
    // 環境定数バッファ
    arg.mpCommandList->bindPipeline(*mpSamplePipeline);
    arg.mpCommandList->setDescriptorHeap(0, *mpEnvDescriptorHeap);

    // 仮レンダリング
    arg.mpCommandList->setDescriptorHeap(1, *mpSampleDescriptorHeap);
    arg.mpCommandList->drawMesh(*bal::mod::gfx::MeshContainer::GetInstance().getBuffer(bal::mod::gfx::MeshContainer::Type::Sphere));
}

// ----------------------------------------------------------------------------

}
