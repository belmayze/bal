/*!
 * @file   balGfxModule.h
 * @brief  グラフィックスモジュール
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/gfx/balGfxPerspectiveCamera.h>
#include <engine/module/balModuleInstance.h>

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

class Module : public ModuleInstance<Module>
{
public:
    /*!
     * シーン用のデフォルト定数バッファ構造体
     * @note 継承して独自の構造体を追加できます
     */
    struct SceneConstantBufferBase
    {
        // カメラ
        bal::MathMatrix44      mViewMatrix;
        bal::MathMatrix44      mProjectionMatrix;
        // ライト
        bal::MathCommonVector3 mDirectionalLightDir;
        float _unused0;
        bal::MathCommonVector3 mDirectionalLightColor;
        float _unused1;
    };
    /*!
     * メッシュの定数バッファ構造体
     * @note 継承して独自の構造体を追加できます
     */
    struct MeshConstantBufferBase
    {
        // ワールド行列
        bal::MathMatrix44 mWorldMatrix;
        bal::MathMatrix44 mNormalWorldMatrix;
    };

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
     * モジュールの終了処理を行う関数です
     */
    virtual void finalize() override;

    /*!
     * モジュールの更新を行う関数です
     * @param[in] arg 更新引数
     */
    virtual void onUpdate(const FrameworkCallback::UpdateArg& arg) override;

    /*!
     * 描画を行う関数です
     * @param[in] arg 描画引数
     */
    virtual void onDraw(const FrameworkCallback::DrawArg& arg) override;

    /*!
     * 最終的に画面に反映させます
     */
    void onPresent(const FrameworkCallback::DrawArg& arg);

public:
    /*!
     * カメラを取得します
     * @note 変更する場合は onUpdate の UBO 決定までに行ってください
     */
    const PerspectiveCamera& getCamera() const { return mCamera; }
          PerspectiveCamera& getCamera()       { return mCamera; }

    /*!
     * bal 用のシェーダーを取得します
     */
    const ShaderArchive& getShaderArchive() const { BAL_ASSERT(mpShaderArchive.get() != nullptr); return *mpShaderArchive; }

    /*!
     * メッシュ用に確保する定数バッファのサイズを取得します
     */
    size_t getMeshConstantBufferSize() const;

    /*!
     * シーン用定数バッファを取得します
     */
    const IConstantBuffer& getSceneConstantBuffer() const { return *mpSceneConstantBuffer; }

public:
    /*!
     * デフォルトのレンダリングターゲットを取得します
     */
    const IRenderTargetColor& getDefaultRenderTarget() const { return *mpRenderTargetColor; }

    /*!
     * 最終レンダリングターゲットを取得します
     */
    const IRenderTargetColor& getFinalRenderTarget() const { return *mpFinalRenderTargetColor; }

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
    std::unique_ptr<IRenderTargetColor> mpFinalRenderTargetColor;
    std::unique_ptr<FrameBuffer>        mpFrameBuffer;
    std::unique_ptr<FrameBuffer>        mpFinalFrameBuffer;
    std::unique_ptr<ShaderArchive>      mpShaderArchive;

    std::unique_ptr<IPipeline>          mpPresentPipeline;
    std::unique_ptr<IDescriptorHeap>    mpPresentDescriptorHeap;

    std::unique_ptr<IPipeline>          mpCopyPipeline;
    std::unique_ptr<IDescriptorHeap>    mpCopyDescriptorHeap;

    std::unique_ptr<IPipeline>          mpDebugMeshPipeline;
    std::unique_ptr<IMeshBuffer>        mpGridMeshBuffer;
    std::unique_ptr<IMeshBuffer>        mpManipulatorMeshBuffer;

    std::unique_ptr<IConstantBuffer>    mpSceneConstantBuffer;
    std::unique_ptr<IDescriptorHeap>    mpSceneDescriptorHeap;

    PerspectiveCamera mCamera;

};

}
