/*!
 * @file   balGfxModule.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/balIModule.h>

namespace bal { class IConstantBuffer; }
namespace bal { class IDescriptorHeap; }
namespace bal { class IGraphics; }
namespace bal { class IMeshBuffer; }
namespace bal { class IPipeline; }
namespace bal { class IRenderTargetColor; }
namespace bal { class IRenderTargetDepth; }
namespace bal { class FrameBuffer; }
namespace bal { class ShaderArchive; }
namespace bal::mod::gfx { class ICustomModule; }

// ----------------------------------------------------------------------------
namespace bal::mod::gfx {

class Module : public IModule
{
public:
    /*!
     * コンストラクター
     */
    Module();

    /*!
     * デストラクター
     */
    virtual ~Module();

    /*!
     * カスタムモジュールを登録する
     * @param[in] p_custom_module カスタムモジュール
     */
    virtual void setCustomModule(std::unique_ptr<mod::ICustomModule>&& p_custom_module) override;

    /*!
     * モジュールの初期化を行う関数です
     * @param[in] arg 初期化引数
     */
    virtual void initialize(const InitializeArg& arg) override;

    /*!
     * モジュールの更新を行う関数です
     * @param[in] arg 更新引数
     */
    virtual void onUpdate(const FrameworkCallback::UpdateArg& arg) override;

    /*!
     * モジュールの種類を取得する
     */
    virtual ModuleType getModuleType() const { return ModuleType::Graphics; }

public:
    /*!
     * 描画を行う関数です
     * @param[in] arg 描画引数
     */
    void onDraw(const FrameworkCallback::DrawArg& arg);

private:
    //! Sample Constant Buffer
    struct SampleMeshCB
    {
        MathMatrix44 mWorldMatrix;
        MathMatrix44 mWorldMatrixForNormal;
    };
    struct SampleEnvCB
    {
        MathMatrix44 mViewMatrix;
        MathMatrix44 mProjMatrix;
        MathMatrix44 mProjectionViewMatrix;
        MathVector3  mDirectionalLightDir;
        float        mPadding0;
        MathColor    mDirectionalLightColor;
    };

private:
    //! デバッグメッシュの頂点情報
    struct DebugMeshVertex
    {
        MathVector3 mPosition;
        MathColor   mColor;
    };
    constexpr static size_t cDebugMeshVertexPositionOffset = offsetof(DebugMeshVertex, mPosition);
    constexpr static size_t cDebugMeshVertexColorOffset    = offsetof(DebugMeshVertex, mColor);

private:
    std::unique_ptr<ICustomModule>      mpCustomModule;
    std::unique_ptr<IRenderTargetColor> mpRenderTargetColor;
    std::unique_ptr<IRenderTargetDepth> mpRenderTargetDepth;
    std::unique_ptr<FrameBuffer>        mpFrameBuffer;
    std::unique_ptr<ShaderArchive>      mpShaderArchive;

    std::unique_ptr<IPipeline>          mpPresentPipeline;
    std::unique_ptr<IDescriptorHeap>    mpPresentDescriptorHeap;

    std::unique_ptr<IPipeline>          mpDebugMeshPipeline;
    std::unique_ptr<IMeshBuffer>        mpGridMeshBuffer;
    std::unique_ptr<IMeshBuffer>        mpManipulatorMeshBuffer;

    std::unique_ptr<IConstantBuffer>    mpEnvConstantBuffer;
    std::unique_ptr<IDescriptorHeap>    mpEnvDescriptorHeap;

    std::unique_ptr<IPipeline>          mpSamplePipeline;
    std::unique_ptr<IDescriptorHeap>    mpSampleDescriptorHeap;
    std::unique_ptr<IConstantBuffer>    mpSampleConstantBuffer;
    SampleMeshCB                        mSampleMeshCB;

};

}
